function [K_est, eig_z2e, sys_z2e, sys_aug] = calcK_steadyState(this, augmentM, qM)
	% [K_est, eig_z2e, sys_z2e, sys_aug] = calcK_steadyState(this, augmentM, qM)
	%
	% Calculates steady state estimator gain (K_est) as well as eigenvalues and estimation error system (ignoring input).
	%
	% If `augmentM` true, augments state with process disturbance whose process noise is qM*I.
	%

	this.checkDims();

	if (isempty(this.Q)||isempty(this.R))
		warning('Cannot calculate Kalman gain without Q, R.')
		K_est = [];
		eig_z2e = [];
		sys_aug = [];
		return;
	end

	tol = 1e-4;%1e-12;
	maxRecursion = ceil(1e2/this.dt);%surely will get to s-s by 1000 seconds!!!

	if nargin<2
		augmentM=false;
	end

	dt = this.dt;
	A = this.A;
	B = this.B;% * (this.g*eye(size(this.B,2)));
	Q = this.Q;
	C = this.C;
	R_est = this.R;

	nU = this.nU;
	nX = this.nX;
	nY = this.nY;

	if augmentM
		% augment system with m
		A_est = [[A; zeros(nX, nX)], zeros(nX+nX, nX)];
		A_est(1:nX, nX+1:end) = eye(nX);
		A_est(nX+1:end, nX+1:end) = eye(nX);

		Q_est = [[Q; zeros(nX, nX)], zeros(nX+nX, nX)];
		Q_est(nX+1:end, nX+1:end) = qM*eye(nX);

		B_est = [B; zeros(nX, nU)];
		x0_est = [this.x0; this.m];

		C_est = [C zeros(nY, nX)];

		nX = nX+nX;
	else
		A_est = A;
		C_est = C;
		Q_est = Q;
		B_est = B;
		x0_est = [this.x0];
	end


	% first try to solve using matlab's idare solver.
	% n.b., What *matlab* calls the state estimation gain is L=A*K, where K is the `Kalman gain`.
	%
	% https://www.mathworks.com/help/control/ref/kalman.html
	% x_{k+1|k} = A*x_{k|k-1} + B*u_{k} + L*(z_{k} - C*x_{k|k-1} - D*u_{k})
	%
	% which is equal to ...
	% x_{k+1|k} = A*x_{k|k-1} + B*u_{k} + A*K*(z_{k} - C*x_{k|k-1} - D*u_{k})
	%
	% where K is the discrete time Kalman gain as the (E)KF is normally implemented. Therefore,
	% K = inv(A)*L
	%  ... obviously this means A must be invertible. Should probably try-catch this, but won't for now.
	[Pinf_t,L_t] = idare(A_est',C_est',Q_est',R_est',[],[],'noscaling');
	K_est = L_t';
	Pinf = Pinf_t';%TODO: not sure if this is correct/same as P below.

	% solve recursively yourself until convergence.
	if isempty(K_est)
		% fprintf('WARNING: idare() could not give solution. Trying numerical recursion.\n')
		[K_est, Pinf] = recurseK(A_est,C_est,Q_est,R_est,tol,maxRecursion);
	else
		K_est = inv(A_est) * K_est;
		% check consistency:
		% [K_est_recurs, Pinf_recurs] = recurseK(A_est,C_est,Q_est,R_est,tol,maxRecursion);
		% K_est
		% K_est_recurs
		%
		% Pinf
		% Pinf_recurs
		% error break
	end

	% assign K
	this.K = K_est(1:this.nX, 1:this.nY);
	this.P0 = Pinf(1:this.nX, 1:this.nX);

	% get measurement --> error system.
	A = (eye(nX) - K_est*C_est)*A_est;
	eig_z2e = eig(A);
	% tau_est = -dt./log(abs(eig_est));
	B = K_est;%steady-state feedback gain.
	C = C_est;
	D = -eye(nY);
	sys_z2e = ss(A,B,C,D,this.dt);

	% return copy of augmented system.
	sys_aug = copy(this);
	sys_aug.A = A_est;
	sys_aug.Q = Q_est;
	sys_aug.B = B_est;
	sys_aug.C = C_est;
	sys_aug.x0 = x0_est; %zeros(size(sys_aug.x0,1),1);
	sys_aug.m = zeros(size(sys_aug.x0));%zeros(size(sys_aug.m,1),1);
	sys_aug.P0 = Pinf;%zeros(size(sys_aug.x0,1));
	sys_aug.K = K_est;
end

function [K, P] = recurseK(A,C,Q,R,tol,maxRecursion)
	P = zeros(size(A));
	K = zeros(size(C,2), size(C,1));
	Kprev = K;

	for l = 1:maxRecursion
		P = A*P*A' + Q;
		S = C*P*C' + R;
		K = P*C'*inv(S);
		P = P-K*C*P;

		if (l==maxRecursion)
			fprintf('WARNING: Failed to find s-s Kalman gain after %d recursions.\n', maxRecursion)
			break;
		end

		if (l>100)
			dK = abs(K-Kprev)./Kprev;
			if max(dK(:))<tol
				break;
			end
		end
		Kprev = K;
	end

end
