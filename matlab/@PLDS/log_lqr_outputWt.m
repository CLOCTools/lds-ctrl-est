function [Kx, KintY, Fx, Fv, Hx] = log_lqr_outputWt(this, qIntY_over_qY, r_over_qY)
	% [Kx, KintY, Fx, Fv, Hx] = log_lqr_outputWt(this, qIntY_over_qY, r_over_qY)
	%
	% Output-weighted LQR for linear dynamical system, including integral action.
  % For PLDS, this in LQR in log-linear space (i.e., output before exponentiation)
	%
	% n.b.,
	% 	x_t = A*x_{t-1} + B*v_{t-1} + m + process_noise
	% 	logy_t = C*x_t + d + measurement_noise
	%
	% 	where v_t = g * u_t = input in units model fit to (e.g., mW/mm2)
	% 				g = conversion factor for physical units of model fit -> control voltage (e.g., 5 mW/mm2/V)
	% 				u_t = control signal sent to actuator ([=] V)
	% 				m = disturbance
	%
	% returns feedback gains, K = [Kx KintY], for the following control policy
	% 	v = v* - K*(xAug - xAug*)
	%
	% 	where v*, xAug* is the set point around which a regulator is operating.
	%
	% K represents the solution to discrete-time steady-state solution to the LQR problem:
	% 	J(K) = ∑((logy-logy*)*Q*(logy-logy*)' + (v-v*)*R*(v-v*)')
	% 	K = argmin J(K)
	%
	% INPUTS
	% 	this: model fit struct at *least* including A, B, C, and dt
	% 	qIntY_over_qY: relative weighting on integral error vs. proportional
	% 	r_over_qY: relative weighting on control deviation vs. proportional error
	%
	% OUTPUTS
	% 	Kx: feedback gains on x
	%		KintY: feedback gains on integral of Y

  this.checkDims();

	if (nargin < 2)
		qIntY_over_qY = 1;%1e2;
	end

	if (nargin < 3)
		r_over_qY = 1;%1e-3;%1e-2
	end

	nX = this.nX;
	nU = this.nU;
	nY = this.nY;

	if qIntY_over_qY>0
		dt = this.dt;
		Fx = zeros(nX+nY);
		Fx(1:nX,1:nX) = this.A;
		Fx((nX+1):end, 1:nX) = this.C*dt; %integrated output
		Fx((nX+1):end, (nX+1):end) = eye(nY);

		% design around v (e.g.  light intensity)
		Fv = zeros(nX+nY,nU);
		Fv(1:nX,:) = this.B;

		Hx = zeros(2*nY,nX+nY);
		Hx(1:nY,1:nX) = this.C;
		Hx(nY+1:end,nX+1:end) = eye(nY);

		Qctrl = eye(2*nY);
		Qctrl(nY+1:end, nY+1:end) = Qctrl(nY+1:end, nY+1:end) .* qIntY_over_qY;
		Rctrl = eye(nU) .* r_over_qY;

		K = try_dlqr(Fx, Fv, Hx'*Qctrl*Hx, Rctrl);
		Kx = K(:,1:nX);
		KintY = K(:,nX+1:end);
	else
		Fx = this.A;
		Fv = this.B;
		Hx = this.C;

		Qctrl = eye(nY);
		Rctrl = eye(nU) .* r_over_qY;
		Kx = try_dlqr(Fx, Fv, Hx'*Qctrl*Hx, Rctrl);
		KintY = zeros(nU,nY);
	end
end

function K = try_dlqr(A, B, Q, R)
	try
		K = dlqr(A, B, Q, R);
	catch
		warning('Solving matrix RE recursively. If this a multi-output system with integral augmentation, P will *NOT* reach a steady-state, but K should converge.')
		itersAllowed = 10000;
		tol = 1e-10;

		P = Q;
		K = zeros(size(B));
		Kprev = K;
		for t=itersAllowed:-1:1
			K = inv(R+B'*P*B)*B'*P*A;
			Pprev = A'*P*A + Q - A'*P*B*K;

			% P should *NOT* change at steady state.
			critP = max(abs(Pprev-P)./abs(P),[],'all');
			% fprintf('dP/P = %f\n',critP);

			% At very least make sure K has converged.
			critK = max(abs(Kprev-K)./abs(K),[],'all');
      % fprintf('dK/K = %f\n',critK);
			if critK<tol
				break;
			end
			P = Pprev;
			Kprev = K;
		end
	end
end
