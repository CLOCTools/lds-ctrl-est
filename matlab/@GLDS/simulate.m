function [y, x, z] = simulate(this, u, addNoise)
	% [y, x, z] = simulate(this, u, addNoise)

	if ~iscell(u)
		error('Inputs (u) must be a cell array.')
	end

	this.checkDims();

	if nargin < 3
		addNoise = false;
	end

	A = this.A;
	B = this.B * diag(this.g);
	m = this.m;
	C = this.C;
	d = this.d;

	nU = this.nU;
	nX = this.nX;
	nY = this.nY;

	if addNoise
		noiseX = cellfun(@(u) mvnrnd(zeros(nX,1), this.Q, size(u,2))', u, 'uniformoutput', false);
	end
	noiseY = cellfun(@(u) mvnrnd(zeros(nY,1), this.R, size(u,2))', u, 'uniformoutput', false);

	x0 = this.x0;

	z = cell(size(u));
	y = cell(size(u));
	x = cell(size(u));
	for trial=1:numel(u)
		nSamps = size(u{trial}, 2);
		x{trial} = zeros(nX, nSamps);
		y{trial} = zeros(nY, nSamps);

		% initial cond
		x{trial}(:,1) = x0;
		y{trial}(:,1) = C*x0 + d;

		for k=2:nSamps
			x{trial}(:,k) = A * x{trial}(:,k-1) + B * u{trial}(:,k-1) + m;
			if addNoise
				x{trial}(:,k) = x{trial}(:,k) + noiseX{trial}(:,k-1);
			end

			y{trial}(:,k) = C*x{trial}(:,k) + d;
			z{trial}(:,k) = y{trial}(:,k) + noiseY{trial}(:,k);
		end
	end
end

function X = sqrtmat_svd(X2)
	[U,S,V] = svd(X2);
	X = U*sqrtm(S)*V';
end
