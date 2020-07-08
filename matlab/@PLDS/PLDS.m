classdef PLDS < LDS
% this = PLDS(dt,A,B,g,m,Q,C,d,x0,P0)

% TODO: should make parameters protected and create set methods.
properties % (SetAccess = protected)
% parameters
C(:,:) double = [];
d(:,1) double = [];
end

properties (Access = protected)
nX uint64 = 0;
nU uint64 = 0;
nY uint64 = 0;
end

methods
function this = PLDS(dt,A,B,g,m,Q,C,d,x0,P0)
  % this = PLDS(dt,A,B,g,m,Q,C,d,x0,P0)
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

  % initial conditions (optional)
  if nargin < 9
    this.x0 = zeros(size(A,1),1);
  else
    this.x0 = x0;
  end
  if nargin < 10
    this.P0 = zeros(size(A));
  else
    this.P0 = P0;
  end

  this.nX = size(A,1);
  this.nU = size(B,2);
  this.nY = size(C,1);
end

function checkDims(this)
  this.nX = size(this.A,1);
  this.nU = size(this.B,2);
  this.nY = size(this.C,1);
end

% methods in other files
[y, x] = simulate(this, u, addNoise);
[yHat, xHat, P, K] = ppfilter(this, u, z, augmentM, qM);
[Kx, KintY, Fx, Fv, Hx] = log_lqr_outputWt(this, qIntY_over_qY, r_over_qY);
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
