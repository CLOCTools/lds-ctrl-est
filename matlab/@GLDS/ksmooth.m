function [yHat, xHat, P, K] = ksmooth(this, u, z, augmentM, qM)
	% [yHat, xHat, P, K] = ksmooth(this, u, z, augmentM, qM)
	%
	% Perform Kalman smoothing (i.e., estimate state given ALL data, z)
	% Refs: Shumway et Stoffer 1982; Ghahramani et Hinton 1996
	%
	% u: cell array of inputs (nU x nTime)
	% z: cell array of measurements (nY x nTime)
	% augmentM: [bool] whether to augment state with disturbance (m)
	% qM: diagonal elements of disturbance process noise cov
	%

	if (~iscell(u) || ~iscell(z))
		error('Inputs (u) and measurements (z) must be a cell array.')
	end

	if nargin<4
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

		warning('Kalman gain was wrong dimensionality. Solving for steady-state gains.')
		K0 = sys_aug.calcK_steadyState();
		P0 = sys_aug.P0;
	end

	I = eye(nX, nX);

	yHat = cell(size(u));
	xHat = cell(size(u));
	P = cell(size(u));
	K = cell(size(u));
	e = zeros(nY,1);

	for trial=1:numel(u)
		nSamps = size(u{trial}, 2);
		xHat{trial} = zeros(nX, nSamps);
		P{trial} = zeros(nX, nX, nSamps);
		yHat{trial} = zeros(nY, nSamps);
		K{trial} = K0 + zeros(nX, nY, nSamps);

		x_pre = zeros(nX, nSamps);
		P_pre = zeros(nX, nX, nSamps);
		x_post = zeros(nX, nSamps);
		P_post = zeros(nX, nX, nSamps);

		% initial cond
		xHat{trial}(:,1) = x0;
		P{trial}(:,:,1) = P0;
		yHat{trial}(:,1) = C*x0 + d;
		x_pre(:,1) = x0;
		x_post(:,1) = x0;
		P_pre(:,:,1) = P0;
		P_post(:,:,1) = P0;

		% Forward filter
		for k=2:nSamps
			% predict
			x_pre(:,k) = A*x_post(:,k-1) + B*u{trial}(:,k-1) + m;
			P_pre(:,:,k) = A*P_post(:,:,k-1)*A' + Q;
			yHat{trial}(:,k) = C*x_pre(:,k) + d;

			% update
			S = C*P_pre(:,:,k)*C' + R;
			K{trial}(:,:,k) = P_pre(:,:,k) * C' * inv(S);
			e = z{trial}(:,k) - yHat{trial}(:,k);
			x_post(:,k) = x_pre(:,k) + K{trial}(:,:,k)*e;
			P_post(:,:,k) = P_pre(:,:,k) - K{trial}(:,:,k)*C*P_pre(:,:,k);
			yHat{trial}(:,k) = C*x_post(:,k) + d;
		end

		% Backfilt
		J = zeros(nX,nX,nSamps);
		xHat{trial}(:,end) = x_post(:,end);
		P{trial}(:,:,end) = P_post(:,:,end);
		for k=nSamps:-1:2
			J(:,:,k-1) = P_post(:,:,k-1) * A' * inv(P_pre(:,:,k));
			xHat{trial}(:,k-1) = x_post(:,k-1) + J(:,:,k-1) * (xHat{trial}(:,k) - x_pre(:,k));
			P{trial}(:,:,k-1) = P_post(:,:,k-1) + J(:,:,k-1) * (P{trial}(:,:,k) - P_pre(:,:,k)) * J(:,:,k-1)';
		end
		for k=1:nSamps
			yHat{trial}(:,k) = C*xHat{trial}(:,k) + d;
		end

	end %trials loop

end
