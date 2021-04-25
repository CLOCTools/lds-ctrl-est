function [u, z, yTrue, yHat, xHat, mHat, uRef, xRef, P, Pm, K, Km] = fbCtrl_steadyState_plds_adaptM_dual(this, plds, r, Kfb_x, Kfb_intY, Kfb_v, qM, ctrlGate, sigma_u_noise, recurseK, adaptSetPoint, uLims)
	% [u, z, yTrue, yHat, xHat, mHat, uRef, xRef, P, Pm, K, Km] = fbCtrl_steadyState_plds_adaptM_dual(this, plds, r, Kfb_x, Kfb_intY, Kfb_v, qM, ctrlGate, recurseK, adaptSetPoint, uLims)
	%
	% Feedback control of a spiking PLDS model with adaptive re-estimation of process disturbance, m.
	% Using steady state control solution (e.g., inf horizon lqr).
	%
	% plds : PLDS object to be controlled
	% r : [nY, nTime] reference/target **output** (not state)
	% Kfb_x : [nU, nX] state feedback controller gains
	% Kfb_intY : [nU, nY] integral feedback controller gains
	% Kfb_v : [nU, nU] or [0,0] input feedback controller gains. If not empty, assumes control designed around updating time-change in u (du).
	% qM : diagonal elements of assumed process noise for disturbance variation
	% ctrlGate : [1, nTime] logical gate for whether control is enabled
	% sigma_u_noise : [deafult=0] standard deviation (sigma) of zero-mean Gaussian noise added to control signal
	% recurseK : [bool] Whether to recursively calculate K (Kalman gain)
	% adaptSetPoint : [bool] Use adaptively re-estimated disturbance when calculating state-control setpoint.
	% uLims : [0 5] lower and upper limits on control signal

	if nargin < 9
		sigma_u_noise = 0;
	end

	if nargin < 10
		recurseK = true;
	end

	if nargin < 11
		adaptSetPoint = false;
	end

	if nargin < 12
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
	K0 = this.K;

	Qm = qM .* eye(nX);
	P0m = Qm;
	K0m = zeros(nX,nY);

	dt = this.dt;

	if (isempty(Q)||isempty(R)) && recurseK
		error('Cannot recursively calculate Kalman gain without Q, R.')
	end

	if ~(isempty(Q)||isempty(R))
		sys0 = copy(this);
		K0 = sys0.calcK_steadyState();
		P0 = sys0.P0;
	end
	sysM = GLDS(dt,eye(nX),zeros(nX,nU),g,zeros(nX,1),Qm,C,d,R,m,P0m);
	K0m = sysM.calcK_steadyState();
	P0m = sysM.P0;

	fn_r_mu_2_xvlam = get_steadyState_ctrlFn(this);

	% true sytem: plds
	nX_true = length(plds.x0);
	A_true = plds.A;
	B_true = plds.B * (plds.g*eye(size(plds.B,2)));
	m_true = plds.m;
	x0_true = plds.x0; %inv(eye(nX_true)-A_true)*m_true;
	C_true = plds.C;
	d_true = plds.d;

	% signals
	nSamps = size(r, 2);
	if nargin < 7
		ctrlGate = ones(nSamps,1,'logical');
	end
	xTrue = x0_true + zeros(nX_true, nSamps);
	xHat = x0 + zeros(nX, nSamps);
	mHat = m + zeros(nX, nSamps);
	P = P0 + zeros(nX, nX, nSamps);
	Pm = P0m + zeros(nX, nX, nSamps);

	yTrue = zeros(nY, nSamps);
	yHat = zeros(nY, nSamps);

	u = zeros(nU, nSamps);
	intE = zeros(nY, 1);

	% rng(13)
	z = zeros(nY, nSamps);
	uRef = zeros(nU, nSamps);
	xRef = zeros(nX, nSamps);

	K = K0 + zeros(nX,nY,nSamps);
	Km = K0m + zeros(nX,nY,nSamps);

	% Initial conditions
	xTrue(:,1) = x0_true;
	yTrue(:,1) = exp(C_true * x0_true + d_true);
	yHat(:,1) = C * x0 + d;

	u_prev = zeros(nU,1);
	m_prev = m;
	Pm_prev = P0m;

	function [] = simPLDS(k)
		xTrue(:,k) = A_true * xTrue(:,k-1) + B_true * u(:,k-1) + m_true;
		yTrue(:,k) = exp(C_true * xTrue(:,k) + d_true);
		z(:,k) = poissrnd(yTrue(:,k));
	end

	function [] = predictLDS(k)
		mHat(:,k-1) = m_prev;
		% xHat(:,k) = A*xHat(:,k-1) + B*u(:,k-1) + mHat(:,k-1);
		xHat(:,k) = A*xHat(:,k-1) + B*u_prev + mHat(:,k-1);
		yHat(:,k) = C*xHat(:,k) + d;
	end

	function [] = updateLDS(k)
		if recurseK
			% cov
			Pm(:,:,k-1) = Pm_prev + Qm;
			S = R + C*Pm(:,:,k-1)*C';
			Km(:,:,k-1) = Pm(:,:,k-1) * C' * inv(S);
			Pm(:,:,k-1) = Pm(:,:,k-1) - Km(:,:,k-1) * C * Pm(:,:,k-1);
			Pm_prev = Pm(:,:,k-1);

			P(:,:,k) = A * P(:,:,k-1) * A' + Q;
			S = C*P(:,:,k)*C' + R;
			K(:,:,k) = P(:,:,k) * C' * inv(S);
			P(:,:,k) = P(:,:,k) - K(:,:,k) * C * P(:,:,k);
		end

		% update
		e = z(:,k) - yHat(:,k);

		mHat(:,k-1) = mHat(:,k-1) + Km(:,:,k-1)*e;
		m_prev = mHat(:,k-1);%for next time.

		xHat(:,k) = xHat(:,k) + K(:,:,k)*e;
		yHat(:,k) = C*xHat(:,k) + d;
	end

	function [] = updateCtrl(k)
		if ctrlGate(k)
			if adaptSetPoint
				r_mu = [r(:,k); mHat(:,k-1)];
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

			% only integrate error if not saturated?
			if ((u(:,k-1) > uLims(1)) && (u(:,k-1) < uLims(2)))
				intE = intE + (yHat(:,k)-r(:,k))*dt;
			end

			% calc control in physical units ("v")
			if isempty(Kfb_v)
				v = v_ss;
				v = v - Kfb_x*(xHat(:,k) - xRef(:,k));
				v = v - Kfb_intY*intE;
			else %if control designed around dv
				% at s-s, dv will be zero
				dv = zeros(nU,1);
				% n.b., line below is essentially first order difference eq
				% (i.e., LP filter ...)
				% dv = dv - Kfb_v*g*(u(:,k-1) - uRef(:,k-1));
				dv = dv - Kfb_v*(g.*u_prev - v_ss);

				dv = dv - Kfb_x*(xHat(:,k) - xRef(:,k));
				dv = dv - Kfb_intY*intE;
				v = g.*u_prev + dv;
			end

			% convert back to driver control voltage
			u(:,k) = v ./ g;

			% TODO(mfbolus): should controller "know" about the added noise?
			% If not, uncomment the below and commount out last line of if statement.
			u(u(:,k)>uLims(2),k) = uLims(2);
			u(u(:,k)<uLims(1),k) = uLims(1);
			u_prev = u(:,k);

			% add noise
			u(:,k) = u(:,k) + sigma_u_noise .* randn(nU,1);
			u(u(:,k)>uLims(2),k) = uLims(2);
			u(u(:,k)<uLims(1),k) = uLims(1);

			% TODO(mfbolus) : see note above
			% u_prev = u(:,k);
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
