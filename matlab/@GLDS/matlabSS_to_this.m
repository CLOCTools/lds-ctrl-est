function [] = matlabSS_to_this(this,sys,g,d)
% [] = matlabSS_to_this(this,sys,g,d)
%
% Given matlab state-space (ss) system object (sys), reassign appropriate variables of the current GLDS object (this).
%
% sys : matlab state space system object
% g : input gain (e.g., conversion factor from control voltage to physical units)
% d : [nY,1] output offset
%

this.dt = sys.Ts;

% dynamics
this.A = sys.A;
this.B = sys.B;
if nargin < 3
  this.g = ones(size(this.B,2),1);
else
  this.g = g;
end

this.m = zeros(size(this.A,1),1);
this.Q = [];%matlab doesn't give this in general.

% output
this.C = sys.C;
if nargin < 4
  this.d = zeros(size(this.C,1),1);
else
  this.d = d;
end
this.R = [];%matlab doesn't give this in general.

% Initial conditions
this.x0 = sys.x0;
this.P0 = zeros(size(this.A));

% matlab predictor gain is A*K,
% where K is Kalman gain as implemented here.
this.K = inv(this.A) * sys.K;

this.checkDims();
end
