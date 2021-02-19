% [fit, sing_vals] = glds_ssid_mex(u, z, dt, n_x, [n_h, d0, force_unit_norm_C, which_wt, wt_g0, t0, t_startSSID, t_stopSSID])
% Fit dynamics of a Gaussian Linear Dynamical System (GLDS) by subspace identification.
%
% INPUTS
% fit: GLDS object being fit.
% u: input, cellarray(nTrials,1), where each trial is numericarray{double}(nU, nSamps)
% z: measurement, cellarray(nTrials,1), where each trial is numericarray{double}(nY, nSamps)
% dt: sample period (double)
% n_x: number of states/order (double)
% n_h: size of block-hankel matrix (nH=20)
% d0: previously-determined value for output bias
% force_unit_norm_C: whether to force output matrix (C) to be unit-norm (bool)
% which_wt: no weighting [0], MOESP [1], CVA [2]
% wt_g0: relative weight (RLS) put on getting DC gain (G0) correct in calculating B.
% t0: initial time for [u; n], numericarray{double}(nTrials,1)
% t_startSSID: start time for fitting dynamics via SSID (double)
% t_stopSSID: stop time for fitting dynamics via SSID (double)
%
% 2019/05/07 : mfbolus
% 2019/08 : mfbolus
% 2020/03 : mfbolus (added which_wt)
% 2020/04 : mfbolus (added wt_g0)
% 2021/02 : mfbolus (added octave mex C api)
