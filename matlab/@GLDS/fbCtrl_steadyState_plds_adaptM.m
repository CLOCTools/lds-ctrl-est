function [u, z, yTrue, yHat, xHat, uRef, xRef, P, K] = fbCtrl_steadyState_plds_adaptM(this, plds, r, Kfb_x, Kfb_intY, qMu, ctrlGate, recurseK, adaptSetPoint, uLims)
	% [u, z, yTrue, yHat, xHat, uRef, xRef, P, K] = fbCtrl_steadyState_plds_adaptM(this, plds, r, Kfb_x, Kfb_intY, qMu, ctrlGate, recurseK, adaptSetPoint, uLims)
	%
	% Feedback control of a spiking PLDS model with adaptive re-estimation of process disturbance, m.
	% Using steady state control solution (e.g., inf horizon lqr).
	%
	% plds : PLDS object to be controlled
	% r : [nY, nTime] reference/target **output** (not state)
	% Kfb_x : [nU, nX] state feedback controller gains. May be either time varying or not.
	% Kfb_intY : [nU, nY] state feedback controller gains. May be either time varying or not.
	% qMu : diagonal elements of assumed process noise for disturbance variation
	% ctrlGate : logical gate for whether control is enabled
	% recurseK : [bool] Whether to recursively calculate K (Kalman gain)
	% adaptSetPoint : [bool] Use adaptively re-estimated disturbance when calculating state-control setpoint.
	% uLims : [0 5] lower and upper limits on control signal

	if nargin < 8
		recurseK = true;
	end

	if nargin < 9
		adaptSetPoint = false;
	end

	if nargin < 10
		uLims = [0 5];
	end

	this.checkDims();
	nU = this.nU;
	nX = this.nX;
	nY = this.nY;

	dt = this.dt;
	A = this.A;
	g = this.g;
	B = this.B * (g*eye(size(this.B,2)));
	x0 = this.x0;
	P0 = this.P0;
	C = this.C;
	m = this.m;
	d = this.d;
	Q = this.Q;
	R = this.R;
	dt = this.dt;

	fn_r_mu_2_xvlam = get_steadyState_ctrlFn(this);

	% augment system with m
	A_est = [[A; zeros(nX, nX)], zeros(nX+nX, nX)];
	A_est(1:nX, nX+1:end) = eye(nX);
	A_est(nX+1:end, nX+1:end) = eye(nX);
	B_est = [B; zeros(nX, nU)];
	Q_est = [[Q; zeros(nX, nX)], zeros(nX+nX, nX)];
	Q_est(nX+1:end, nX+1:end) = qMu .* eye(nX);
	x0_est = [x0; m];
	P0_est = [[P0; zeros(nX, nX)], zeros(nX+nX, nX)];
	P0_est(nX+1:end, nX+1:end) = qMu .* eye(nX);
	C_est = [C zeros(nY, nX)];
	I_est = eye(nX+nX, nX+nX);

	% true sytem: plds
	nX_true = length(plds.x0);
	A_true = plds.A;
	B_true = plds.B * (plds.g*eye(size(plds.B,2)));
	m_true = plds.m;
	x0_true = inv(eye(nX_true)-A_true)*m_true;
	C_true = plds.C;
	d_true = plds.d;

	% signals
	nSamps = size(r, 2);
	if nargin < 7
		ctrlGate = ones(nSamps,1,'logical');
	end
	xTrue = zeros(nX_true, nSamps);
	xHat = zeros(nX+nX, nSamps);
	P = zeros(nX+nX, nX+nX, nSamps);

	yTrue = zeros(nY, nSamps);
	yHat = zeros(nY, nSamps);

	u = zeros(nU, nSamps);
	intE = zeros(nY, 1);

	% rng(13)
	chance = rand(nY, nSamps);
	z = zeros(nY, nSamps);
	uRef = zeros(nU, nSamps);
	xRef = zeros(nX, nSamps);

	K = zeros(nX+nX,nY,nSamps);

	sys_aug = copy(this);
	sys_aug.A = A_est;
	sys_aug.Q = Q_est;
	sys_aug.B = B_est;
	sys_aug.C = C_est;
	sys_aug.x0 = x0_est;
	sys_aug.P0 = P0_est;
	K0 = sys_aug.calcK_steadyState();
	P0_est = sys_aug.P0;
	if ~recurseK
		K = K + K0;
	end

	% Initial conditions
	xTrue(:,1) = x0_true;
	yTrue(:,1) = exp(C_true * x0_true + d_true);

	xHat(:,1) = x0_est;
	P(:,:,1) = P0_est;
	yHat(:,1) = C_est * x0_est + d;

	function [] = simPLDS(k)
		xTrue(:,k) = A_true * xTrue(:,k-1) + B_true * u(:,k-1) + m_true;
		yTrue(:,k) = exp(C_true * xTrue(:,k) + d_true);
		z(:,k) = chance(:,k) < yTrue(:,k);
	end

	function [] = predictLDS(k)
		xHat(:,k) = A_est * xHat(:,k-1) + B_est * u(:,k-1);
		P(:,:,k) = A_est * P(:,:,k-1) * A_est' + Q_est;
		yHat(:,k) = C_est * xHat(:,k) + d;
	end

	function [] = updateLDS(k)
		% update
		e = z(:,k) - yHat(:,k);
		if recurseK
			S = C_est*P(:,:,k)*C_est' + R;
			K(:,:,k) = P(:,:,k) * C_est' * inv(S);
		end
		xHat(:,k) = xHat(:,k) + K(:,:,k)*e;

		% Stengel:
		% P(:,:,k) = inv( inv(P(:,:,k)) + C_est'*inv(R)*C_est );
		% Hinton, Gaghramani 1995
		P(:,:,k) = P(:,:,k) - K(:,:,k) * C_est * P(:,:,k);

		yHat(:,k) = C_est*xHat(:,k) + d;
	end

	function [] = updateCtrl(k)
		if ctrlGate(k)
			if adaptSetPoint
				r_mu = [r(:,k); xHat(nX+1:end,k)];
				xvlam = fn_r_mu_2_xvlam(r_mu);
				xRef(:,k) = xvlam(1:nX);
				v_ss = xvlam(nX+1:nX+nU);
			else
				r_mu = [r(:,k); m];
				xvlam = fn_r_mu_2_xvlam(r_mu);
				xRef(:,k) = xvlam(1:nX);
				v_ss = xvlam(nX+1:nX+nU);
			end
			uRef(:,k) = v_ss./g;

			% TODO: the only realizable output. Comment out to use actual ref.
			% r(:,k) = C*xRef(:,k) + d;

			% calc control in intensity ("v")
			v = v_ss;
			v = v - Kfb_x*(xHat(1:nX,k) - xRef(:,k));

			% only integrate error if not saturated?
			if ((u(:,k-1) > uLims(1)) && (u(:,k-1) < uLims(2)))
				intE = intE + (yHat(:,k)-r(:,k))*dt;
			end
			v = v - Kfb_intY*intE;

			% convert back to driver control voltage
			u(:,k) = v ./ g;

			u(u(:,k)>uLims(2),k) = uLims(2);
			u(u(:,k)<uLims(1),k) = uLims(1);
		else
			intE = 0*r(:,k);
		end
	end

	for k=2:nSamps
		predictLDS(k);
		simPLDS(k);
		updateLDS(k);
		updateCtrl(k);
	end
end

function [fn_r_mu_2_xvlam] = get_steadyState_ctrlFn(this)
	% [fn_r_mu_2_xvlam] = get_steadyState_ctrlFn(this)
	%
	% Calculate LS solution for control/state trajectory for output reference (r), constrained to be at steady-state.
	%
	% Steady state is a linear constraint in this case. Solved linearly constrained least squares by lagrange multipliers. See Stephen Boyd (Stanford) and/or L. Vandenberghe 2018, Intro to Applied Lin Alg
	%

	nX = size(this.A,1);
	nU = size(this.B,2);
	nY = size(this.C,1);

	A = [this.C zeros(nY,nU)];
	% b = [r-this.d];
	C = [this.A-eye(nX) this.B];
	d = -this.m;

	phi = [2*A'*A C'; C zeros(size(C,1))];
	inv_phi = inv(phi);
	% xvlam = inv_phi * [2*A'*[r-this.d]; d];
	fn_r_mu_2_xvlam = @(r_mu) inv_phi * [2*A'*[r_mu(1:nY)-this.d]; -r_mu(nY+1:end)];
end
