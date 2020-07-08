function [plds, y, x] = fit_plds_output_mle(this, u, z, g, rescaleOnly, wG0)
% [plds, y, x] = refit_plds_output(this, u, z, g, d, rescaleOnly, wG0)

% n.b., B was plds in terms of intensity, v
if iscell(u)
  nU = size(this.B,2);
  if (~iscell(g)) && (numel(g)==nU)
    u = cellfun(@(u) g.*u, u, 'uniformoutput', false);
  else
    u = cellfun(@(u,g) g.*u, u, g, 'uniformoutput', false);
    g = cell2mat(g)';
  end

  u = cell2mat( cellfun(@(x) x', u(:), 'uniformoutput',false) )';
  z = cell2mat( cellfun(@(x) x', z(:), 'uniformoutput',false) )';
else
  u = g .* u;
end

this.checkDims();

dt = this.dt;
A = this.A;
B = this.B;

nSamps = size(u,2);
nY = size(z,1);
nX = size(A,1);
nU = size(B,2);

if nargin<5
  rescaleOnly=true;
end
if (nY ~= this.nY)
  rescaleOnly = false;
end

if nargin<6
  % how much weight to put on G0 u->z?
  wG0 = 0;
end
if wG0<0
  wG0 = 0;
end

x = zeros(nX, nSamps);
y = zeros(nY, nSamps);

% simulate forward (assuming x0=0 and m=0)
for k=2:nSamps
  x(:,k) = A*x(:,k-1) + B*u(:,k-1);
end

if rescaleOnly
  C = this.C;
  d = this.d;
else  
  % get initial guess in the linear regression sense.
  % TODO: not sure of right threshold here.
  ubiSilence = sum(abs(u),1) < 0.1;
  d = mean(z(:,ubiSilence),2);
  z = z-d;
  if wG0 == 0
    % OLS
    C = z * pinv(x);
  else
    g0_u2y = z * pinv(u);
    g0_u2x = inv(eye(nX)-A)*B;

    % WLS : Make sure to get gain u->y
    % (more important than mapping to x?)
    % TODO: use lagrange multipliers to make linear constraint?
    z_guz = [z, g0_u2y];
    x_gux = [x, g0_u2x];
    nObs = size(x_gux,2);
    W = speye(nObs);
    W(end-nX*nU+1:end,end-nX*nU+1:end) = wG0*nSamps*speye(nX*nU);
    invX = inv(x_gux * W' * x_gux')*x_gux*W';
    C = z_guz * invX';
  end
end

tol = 1e-3;
itersAllowed = 100;

sumZ = sum(z,2);
cx = zeros(nY,nSamps);
cx_p = zeros(1,nSamps);
r_p = zeros(1,nSamps);

function calc_cx()
  for k=1:nSamps
    cx(:,k) = C*x(:,k);
  end
  cx(cx<(-10)) = -10;
  cx(cx>(10)) = 10;
end

% analytically solve for d
function solveD_mle()
  sumR = sum(exp(cx),2);

  d_new = log(sumZ) - log(sumR);
  d_new(d_new < -10) = -10;
  d_new(d_new > 10) = 10;

  d(:) = d_new;
end

% numerically solve for C
function newtonSolveC_mle()
  for p=1:nY
    deltaG = 100;
    l = 0;
    gC_p = 1;
    gC_p_prev = 1;
    cx_p(:,:) = cx(p,:);
    while (deltaG>tol)&&(l<itersAllowed)
      % with new scalar, recalc rate.
      r_p(:,:) = exp(gC_p*cx_p + d(p));
      r_p(r_p<1e-10) = 1e-10;
      r_p(r_p>10) = 10;

      f = 0;
      fprime = 0;

      % get f and f' for newton step
      for k=1:nSamps
        f = f + r_p(:,k)*cx_p(:,k) - z(p,k)*cx_p(:,k);
        fprime = fprime + r_p(:,k)*cx_p(:,k)*cx_p(:,k)';
      end
      f_over_fprime = inv(fprime)*f;
      gC_p = gC_p - f_over_fprime;%*0.1;

      deltaG = abs((gC_p - gC_p_prev)./gC_p_prev);
      gC_p_prev = gC_p;
      l = l+1;
    end%iterations
    C(p,:) = gC_p * C(p,:);
  end%output loop
end%newtonSolveC_mle

% get initial estimates.
d = log(d);
calc_cx();
newtonSolveC_mle();

deltaTheta = 100;
iter = 0;
theta = [d(:); C(:)];
theta_prev = theta;
while (max(deltaTheta)>tol)&&(iter<itersAllowed)
  calc_cx();
  solveD_mle();
  newtonSolveC_mle();

  theta = [d(:); C(:)];
  deltaTheta = abs((theta-theta_prev)./theta_prev);

  fprintf('Finished iteration %d: C[0] = %f ... \n', iter, C(1));
  iter = iter+1;
  theta_prev = theta;
end

% g0 = C * inv(eye(nX)-A) * B;

% simulate the final output.
calc_cx();
y = exp(cx+d);

plds = PLDS(dt,A,B,mean(g,2),zeros(nX,1),zeros(nX),C,d,zeros(nX,1), zeros(nX));
end

function [yImp,d0] = estimate_yImp(x,y,ord)
  % [yImp,d0] = estimate_yImp(x,y,ord)
  %
  % Fitting a non-parametric impulse response by least squares.
  % Both x and y must be column vectors.

  % Create stimulus lag-matrix
  % (Should probably use some variant of convmtx(x,ord) for better efficiency.)
  X = zeros(length(x), ord);
  buff = [zeros(1,ord-1), x(1)]; %flip( x(1:ord,1)' );
  X(1,:) = buff;
  for i = 2:length(x)
    buff = [buff(2:(end)), x(i)];
    X(i,:) = buff;
  end

	% add extra ones for output bias est.
  X = [ones(length(x),1), X];

  % Do linear regression.
  yImp = pinv(X) * y;
  d0 = yImp(1,:)';
  yImp = flip(yImp(2:end,:))';

	% g0_yImp = sum(yImp)
end %end fn
