% [yHat, xHat, sum_E_xu_tm1_xu_tm1, sum_E_xu_t_xu_tm1, sum_E_xu_t_xu_t, T] = glds_em_refit_mex(fit, u, z, calcAB, calcQ, calcInitial, calcC, calcd, calcR, maxIter, tol)
%
% Refit a Gaussian Linear Dynamical System (GLDS) by expectation-maximization.
%
% INPUTS
% fit: initial fit being refit
% u: input, cellarray(nTrials,1), where each trial is numericarray{double}(nU, nSamps)
% z: measurement, cellarray(nTrials,1), where each trial is numericarray{double}(nY, nSamps)
% t0: initial time for [u; n], numericarray{double}(nTrials,1)
% dt: sample period (double)
% calcAB, calcQ, calcInitial, calcC, calcd, calcR: whether to calculate new values for respective parameters
% maxIter: max number of interations of E-M alg
% tol: parameter convergence tolerance
%
% 2020/04 : mfbolus
