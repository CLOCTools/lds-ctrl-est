function [u_ss, x_ss, y_ss] = calcCtrl_steadyState(this, r)
  % [u_ss, x_ss, y_ss] = calcCtrl_steadyState(this, r)
  %
  % Calculate LS solution for control/state trajectory for output reference (r), constrained to be at steady-state.
  %
  % Steady state is a linear constraint in this case. Solved linearly constrained least squares by lagrange multipliers. See Stephen Boyd (Stanford) and/or L. Vandenberghe 2018, Intro to Applied Linear Algebra
  %

  this.checkDims();

  nX = this.nX;
  nU = this.nU;
  nY = this.nY;

  A = [this.C zeros(nY,nU)];
  b = [r-this.d];
  C = [this.A-eye(nX) this.B];
  d = -this.m;

  phi = [2*A'*A C'; C zeros(size(C,1))];
  xvlam = inv(phi) * [2*A'*b; d];

  x_ss = xvlam(1:nX);
  v_ss = xvlam(nX+1:nX+nU);
  u_ss = v_ss./this.g;

  y_ss = this.C*x_ss + this.d;
end
