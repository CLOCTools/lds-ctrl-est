function [yHat, xHat, mHat, P, Pm, K, Km] = kfilter_joint(this, u, z, recurseK, augmentM, qM)
	% [yHat, xHat, mHat, P, Pm, K, Km] = kfilter_joint(this, u, z, recurseK, augmentM, qM)
	%
	% Perform Kalman filtering (i.e., estimate state given data, z, up to current time)
	% Refs: Shumway et Stoffer 1982; Ghahramani et Hinton 1996
	%
	% u: cell array of inputs (nU x nTime), where each cell is "trial"
	% z: cell array of measurements (nY x nTime)
	% recurseK: [bool] whether to recursively solve for K at each time point
	% augmentM: [bool] whether to augment state with disturbance (m)
	% qM: diagonal elements of disturbance process noise cov
	%

	if (~iscell(u) || ~iscell(z))
		error('Inputs (u) and measurements (z) must be a cell array.')
	end

	if nargin<4
		recurseK = true;
	end

	if nargin<5
		augmentM = false;
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
		augmentM = false;
	end

	if augmentM
		% augment system with m
		% m = m + d*pinv(C*inv(eye(nX)-A));
		% x0 = inv(eye(nX)-A)*m;
		A = [A eye(nX); zeros(nX,nX) eye(nX)];
		B = [B; zeros(nX, nU)];
		Q = [[Q; zeros(nX, nX)], zeros(nX+nX, nX)];
		Q(nX+1:end, nX+1:end) = qM .* eye(nX);
		C = [C zeros(nY, nX)];

		x0 = [x0; m];
		P0 = [[P0; zeros(nX, nX)], zeros(nX+nX, nX)];
		m = zeros(2*nX,1);

		nU = size(B,2);
		nX = size(A,1);
		nY = size(C,1);
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

		mHat{trial} = this.m + zeros(this.nX, nSamps);
		Pm{trial} = qM.*eye(this.nX) + zeros(this.nX, this.nX, nSamps);

		xHat{trial} = zeros(nX, nSamps);
		P{trial} = zeros(nX, nX, nSamps);
		yHat{trial} = zeros(nY, nSamps);

		K{trial} = K0 + zeros(nX, nY, nSamps);
		Km{trial} = zeros(nX, nY, nSamps);

		% initial cond
		xHat{trial}(:,1) = x0;
		P{trial}(:,:,1) = P0;
		yHat{trial}(:,1) = C*x0 + d;

		for k=2:nSamps
			% predict mean
			xHat{trial}(:,k) = A*xHat{trial}(:,k-1) + B*u{trial}(:,k-1) + m;
			yHat{trial}(:,k) = C*xHat{trial}(:,k) + d;

			if recurseK
				%predict state cov
				P{trial}(:,:,k) = A*P{trial}(:,:,k-1)*A' + Q;
				S = R + C*P{trial}(:,:,k)*C';
				K{trial}(:,:,k) = P{trial}(:,:,k) * C' * inv(S);
				% update cov
				P{trial}(:,:,k) = (I - K{trial}(:,:,k)*C) * P{trial}(:,:,k);
			end

			% update
			e(:,1) = z{trial}(:,k) - yHat{trial}(:,k);
			xHat{trial}(:,k) = xHat{trial}(:,k) + K{trial}(:,:,k)*e;
			yHat{trial}(:,k) = C*xHat{trial}(:,k) + d;
		end

		if augmentM
			mHat{trial} = xHat{trial}(this.nX+1:end, :);
			xHat{trial} = xHat{trial}(1:this.nX, :);
			Pm{trial} = P{trial}(this.nX+1:end,this.nX+1:end,:);
			P{trial} = P{trial}(1:this.nX,1:this.nX,:);
			Km{trial} = K{trial}(this.nX+1:end,:,:);
			K{trial} = K{trial}(1:this.nX,:,:);
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
