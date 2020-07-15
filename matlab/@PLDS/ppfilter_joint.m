function [yHat, xHat, P, K] = ppfilter_joint(this, u, z, augmentM, qM)
	% [yHat, xHat, P, K] = ppfilter_joint(this, u, z, augmentM, qM)
	%
	% Perform point-process filtering (i.e., estimate state given data, z, up to current time)
	% Refs: Eden et al. 2004 and related papers
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

	nU = this.nU;
	nX = this.nX;
	nY = this.nY;

	if augmentM
		% augment system with m
		A = [A eye(nX); zeros(nX,nX) eye(nX)];
		B = [B; zeros(nX, nU)];
		Q = [[Q; zeros(nX, nX)], zeros(nX+nX, nX)];
		Q(nX+1:end, nX+1:end) = qM .* eye(nX);
		C = [C zeros(nY, nX)];

		x0 = [x0; m];
		P0 = [[P0; zeros(nX, nX)], zeros(nX+nX, nX)];
		P0(nX+1:end, nX+1:end) = qM .* eye(nX);%TODO: a fudge.
		m = zeros(2*nX,1);

		nU = size(B,2);
		nX = size(A,1);
		nY = size(C,1);
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
		K{trial} = zeros(nX, nY, nSamps);

		% initial cond
		xHat{trial}(:,1) = x0;
		P{trial}(:,:,1) = P0;
		yHat{trial}(:,1) = exp(C*x0 + d);

		for k=2:nSamps
			% predict
			xHat{trial}(:,k) = A*xHat{trial}(:,k-1) + B*u{trial}(:,k-1) + m;
			P{trial}(:,:,k) = A*P{trial}(:,:,k-1)*A' + Q;
			yHat{trial}(:,k) = exp(C*xHat{trial}(:,k) + d);

			% update
			diagY = diag(yHat{trial}(:,k));
			e(:,1) = z{trial}(:,k) - yHat{trial}(:,k);
			P{trial}(:,:,k) = inv( inv(P{trial}(:,:,k)) + C'*diagY*C );%log(y) is linear wrt x, so no hessian.
			K{trial}(:,:,k) = P{trial}(:,:,k)*C';
			xHat{trial}(:,k) = xHat{trial}(:,k) + K{trial}(:,:,k)*e;
			yHat{trial}(:,k) = exp( C*xHat{trial}(:,k) + d );
		end
	end

end
