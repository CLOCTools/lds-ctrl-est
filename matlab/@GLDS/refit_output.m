function [y, x] = refit_output(this, u, z, g, tol, maxIter, refit_d)
% [y, x] = refit_output(this, u, z, g, tol, maxIter, refit_d)
%
% Refit output [C, d, R] using Expectation-maximization (EM).
%  * relies on ldsCtrlEst library (mex function).
%
% u : cell array of inputs [nU, nTime], where each trial is a cell
% z : cell array of measurements [nY, nTime], where each trial is a cell
% g : input gain (either cell with one g per trial) or a single [nU,1] vector.
% tol : tolerance for param convergence
% maxIter : max number of iterations
% refit_d : whether to refit the output bias (d)
%

nU = size(this.B,2);
if nargin < 4
  g = this.g;
end

if nargin < 5
  tol = 1e-2;
end

if nargin < 6
  maxIter = 100;
end

if nargin < 7
  refit_d=true;
end
this.checkDims();
if (this.nY~=length(this.d))
  refit_d=true;
end

if (~iscell(g)) && (numel(g)==nU)
  u = cellfun(@(u) g.*u, u, 'uniformoutput', false);
else
  u = cellfun(@(u,g) g.*u, u, g, 'uniformoutput', false);
  g = cell2mat(g)';
end

% already rolled gain into input
this.g = ones(nU,1);

% [yHat, xHat, sum_E_xu_tm1_xu_tm1, sum_E_xu_t_xu_tm1, sum_E_xu_t_xu_t, T] = glds_em_refit_mex(fit, u, z, calcAB, calcQ, calcInitial, calcC, calcd, calcR, maxIter, tol)
[y, x] = glds_em_refit_mex(this, u, z, false, false, false, true, refit_d, true, maxIter, tol);

this.g = mean(g,2);
this.checkDims();
this.calcK_steadyState();
end
