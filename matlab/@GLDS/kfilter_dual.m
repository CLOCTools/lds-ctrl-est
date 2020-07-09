function [yHat, xHat, mHat, P, Pm, K, Km] = kfilter_dual(this, u, z, recurseK, adaptM, qM)
	% [yHat, xHat, mHat, P, Pm, K, Km] = kfilter_dual(this, u, z, recurseK, adaptM, qM)
	%
	% Perform Kalman filtering (i.e., estimate state given data, z, up to current time)
	% Refs: Shumway et Stoffer 1982; Ghahramani et Hinton 1996
	%
	% 'Dual' parameter estimation, rather than augmenting state and doing joint.
	%
	% u: cell array of inputs (nU x nTime), where each cell is "trial"
	% z: cell array of measurements (nY x nTime)
	% recurseK: [bool] whether to recursively solve for K at each time point
	% adaptM: [bool] whether to augment state with disturbance (m)
	% qM: diagonal elements of disturbance process noise cov
	%

	if (~iscell(u) || ~iscell(z))
		error('Inputs (u) and measurements (z) must be a cell array.')
	end

	if nargin<4
		recurseK = true;
	end

	if nargin<5
		adaptM = false;
	end

	this.checkDims();

	A = this.A;
	B = this.B * diag(this.g);
	m = this.m;
	x0 = this.x0;
	P0 = this.P0;
	C = this.C;
	d = this.d;

	Q = this.Q;
	R = this.R;
	K0 = this.K;

	nU = this.nU;
	nX = this.nX;
	nY = this.nY;

	if isempty(Q)||isempty(R)
		recurseK = false;
	end
	if isempty(R)
		adaptM = false;
	end

	% if K0 is not correct size, recalculate s-s Kalman gain.
	if sum(abs(size(K0)-double([nX, nY])))>0
		sys_aug = copy(this);
		sys_aug.A = A;
		sys_aug.Q = Q;
		sys_aug.B = B;
		sys_aug.C = C;
		sys_aug.x0 = x0;
		sys_aug.P0 = P0;
		if recurseK
			warning('Kalman gain was wrong dimensionality. Solving for steady-state gains.')
		end
		K0 = sys_aug.calcK_steadyState();
		P0 = sys_aug.P0;
	end

	P0m = qM.*eye(nX);
	K0m = zeros(nX,nY);
	if adaptM
		Qm = qM .* eye(nX);
		sysM = GLDS(this.dt,eye(nX),zeros(nX,nU),this.g,zeros(nX,1),Qm,C,d,R,m,P0m);
		K0m = sysM.calcK_steadyState();
		P0m = sysM.P0;
	end

	I = eye(nX, nX);

	yHat = cell(size(u));
	xHat = cell(size(u));
	P = cell(size(u));
	K = cell(size(u));
	e = zeros(nY,1);

	mHat = cell(size(u));
	Pm = cell(size(u));
	Km = cell(size(u));

	for trial=1:numel(u)
		nSamps = size(u{trial}, 2);

		mHat{trial} = m + zeros(nX, nSamps);
		Pm{trial} = P0m + zeros(nX, nX, nSamps);

		xHat{trial} = zeros(nX, nSamps);
		P{trial} = zeros(nX, nX, nSamps);
		yHat{trial} = zeros(nY, nSamps);

		K{trial} = K0 + zeros(nX, nY, nSamps);
		Km{trial} = K0m + zeros(nX, nY, nSamps);

		% initial cond
		xHat{trial}(:,1) = x0;
		P{trial}(:,:,1) = P0;
		yHat{trial}(:,1) = C*x0 + d;

		m_prev = mHat{trial}(:,1);
		Pm_prev = Pm{trial}(:,:,1);

		for k=2:nSamps
			if adaptM
				% predict and update m_km1
				mHat{trial}(:,k-1) = m_prev;%random walk.
				Pm{trial}(:,:,k-1) =  Pm_prev + Qm;
				S = R + C*Pm{trial}(:,:,k-1)*C';
				Km{trial}(:,:,k-1) = Pm{trial}(:,:,k-1) * C' * inv(S);
				Pm{trial}(:,:,k-1) = (I - Km{trial}(:,:,k-1)*C) * Pm{trial}(:,:,k-1);
			end

			% predict mean
			xHat{trial}(:,k) = A*xHat{trial}(:,k-1) + B*u{trial}(:,k-1) + mHat{trial}(:,k-1);%*predicted* m is same as prev m
			yHat{trial}(:,k) = C*xHat{trial}(:,k) + d;
			e(:,1) = z{trial}(:,k) - yHat{trial}(:,k);

			if adaptM
				% update m
				mHat{trial}(:,k-1) = m_prev + Km{trial}(:,:,k-1)*e;
				m_prev = mHat{trial}(:,k-1);%for next time.
				Pm_prev = Pm{trial}(:,:,k-1);
			end

			if recurseK
				%predict state cov
				P{trial}(:,:,k) = A*P{trial}(:,:,k-1)*A' + Q;
				S = R + C*P{trial}(:,:,k)*C';
				K{trial}(:,:,k) = P{trial}(:,:,k) * C' * inv(S);
				% update cov
				P{trial}(:,:,k) = (I - K{trial}(:,:,k)*C) * P{trial}(:,:,k);
			end

			% update
			xHat{trial}(:,k) = xHat{trial}(:,k) + K{trial}(:,:,k)*e;
			yHat{trial}(:,k) = C*xHat{trial}(:,k) + d;
		end
	end


	% % calculate the time-constants of the system, assuming the final K is at ss.
	% A_est = (eye(nX) - K{end}(:,:,end)*C)*A;
	% eig_est = eig(A_est);
	% tau_est = -this.dt ./ log(abs(eig_est));
	%
	% B_est = K;%steady-state feedback gain.
	% C_est = C;
	% D_est = -eye(nY);
	%
	% sys_z2e = ss(A_est,B_est,C_est,D_est,this.dt);
end
