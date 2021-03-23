classdef GLDS < LDS
% this = GLDS(dt,A,B,g,m,Q,C,d,R,x0,P0)

% TODO: should make parameters protected and create set methods that check dims correct.
properties % (SetAccess = protected)
% parameters
C(:,:) double = [];
d(:,1) double = [];
R(:,:) double = [];

% steady-state KF gain.
K(:,:) double = [];
end

properties (Access = protected)
nX uint64 = 0;
nU uint64 = 0;
nY uint64 = 0;
end

methods
function this = GLDS(dt,A,B,g,m,Q,C,d,R,x0,P0)
  % this = GLDS(dt,A,B,g,m,Q,C,d,R,x0,P0)
  %

  if nargin<1
    return;
  end

  this.dt = dt;

  % dynamics
  this.A = A;
  this.B = B;
  this.g = g;
  this.m = m;
  this.Q = Q;

  % output
  this.C = C;
  this.d = d;
  this.R = R;

  % initial conditions (optional)
  if nargin < 10
    this.x0 = zeros(size(A,1),1);
  else
    this.x0 = x0;
  end
  if nargin < 11
    this.P0 = zeros(size(A));
  else
    this.P0 = P0;
  end

  this.nX = size(A,1);
  this.nU = size(B,2);
  this.nY = size(C,1);

  this.K = zeros(this.nX, this.nY);
end

function checkDims(this)
  this.nX = size(this.A,1);
  this.nU = size(this.B,2);
  this.nY = size(this.C,1);
end

% methods in other files:
matlabSS_to_this(this,sys,g,d);

[y, x, z] = simulate(this, u, addNoise);
[yImpC, tImp] = simulate_imp(this, nSamps);

[K_est, eig_z2e, sys_z2e, sys_aug] = calcK_steadyState(this, augmentM, qM);
[yHat, xHat, mHat, P, Pm, K, Km] = kfilter_joint(this, u, z, recurseK, augmentM, qM);
[yHat, xHat, mHat, P, Pm, K, Km] = kfilter_dual(this, u, z, recurseK, adaptM, qM);
[yHat, xHat, mHat, P, Pm, K, Km] = ksmooth_joint(this, u, z, augmentM, qM);

[u_ss, x_ss, y_ss] = calcCtrl_steadyState(this, r);
[Kx, KintY, Fx, Fv, Hx] = lqr_outputWt(this, qIntY_over_qY, r_over_qY);
[u, z, yTrue, yHat, xHat, mHat, uRef, xRef, P, Pm, K, Km] = fbCtrl_steadyState_plds_adaptM_joint(this, plds, r, Kfb_x, Kfb_intY, qM, ctrlGate, recurseK, adaptSetPoint, uLims);
[u, z, yTrue, yHat, xHat, mHat, uRef, xRef, P, Pm, K, Km] = fbCtrl_steadyState_plds_adaptM_dual(this, plds, r, Kfb_x, Kfb_intY, qM, ctrlGate, recurseK, adaptSetPoint, uLims);

[plds, y, x] = fit_plds_output_mle(this, u, z, g, rescaleOnly, wG0);
[y, x] = refit_output(this, u, z, g, tol, maxIter, refit_d);

[] = save_params_controller(this, savename, qM, Kc, Kc_inty, yRef);
end%end methods

end%classdef

function passed = contentsAreChar(c)
  passed = cellfun(@(x) ischar(x), c);
  passed = sum(passed)==numel(c);
  if ~passed
    error('Must be type char.')
  end
end

function passed = contentsAreDouble(c)
  passed = cellfun(@(x) isa(x,'double'), c);
  passed = sum(passed)==numel(c);
  if ~passed
    error('Must be type double.')
  end
end
