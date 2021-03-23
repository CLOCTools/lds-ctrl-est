% [y_hat, x_hat, sum_E_xu_tm1_xu_tm1, sum_E_xu_t_xu_tm1, sum_E_xu_t_xu_t, T] = glds_em_refit_mex(fit, u, z, calc_dynamics, calc_Q, calc_init, calc_output, calc_measurement, max_iter, tol)
%
% Refit a Gaussian Linear Dynamical System (GLDS) by expectation-maximization.
%
% INPUTS
% fit: initial fit being refit
% u: input, cellarray(nTrials,1), where each trial is numericarray{double}(nU, nSamps)
% z: measurement, cellarray(nTrials,1), where each trial is numericarray{double}(nY, nSamps)
% dt: sample period (double)
% calc_dynamics, calc_Q, calc_init, calc_output, calc_measurement: whether to calculate new values for respective parameters
% max_iter: max number of interations of E-M alg
% tol: parameter convergence tolerance
%
% 2020/04 (mfbolus)
% 2021/03 (mfbolus): removed unused parameters
