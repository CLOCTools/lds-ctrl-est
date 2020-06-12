% fit = glds_ssid_mex(u, z, t0, dt, nX, [nH, d0, force_unitNorm_C, whichWt, wtG0, t_startSSID, t_stopSSID])
%
% Fit dynamics of a Gaussian Linear Dynamical System (GLDS) by subspace identification.
%
% INPUTS
% u: input, cellarray(nTrials,1), where each trial is numericarray{double}(nU, nSamps)
% z: measurement, cellarray(nTrials,1), where each trial is numericarray{double}(nY, nSamps)
% t0: initial time for [u; n], numericarray{double}(nTrials,1)
% dt: sample period (double)
% nX: number of states/order (double)
% nH: size of block-hankel matrix (nH=20)
% d0: previously-determined value for output bias
% force_unitNorm_C: whether to force output matrix (C) to be unit-norm (bool)
% whichWt: no weighting [0], MOESP [1], CVA [2]
% wtG0: relative weight (RLS) put on getting DC gain (G0) correct in calculating B.
% t_startSSID: start time for fitting dynamics via SSID (double)
% t_stopSSID: stop time for fitting dynamics via SSID (double)
%
% 2019/05/07 : mfbolus
% 2019/08 : mfbolus
% 2020/03 : mfbolus (added whichWt)
% 2020/04 : mfbolus (added wtG0)
