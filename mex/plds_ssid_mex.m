% pldsFit, gldsFit = plds_ssid_mex(u, z, t0, dt, nX, [nH, d0, whichWt, wtG0, t_startSSID, t_stopSSID])
%
% Fit dynamics of a Gaussian Linear Dynamical System (GLDS) by subspace identification. Then refit output by Poisson MLE using Newton's method, yielding a Poisson LDS (PLDS).
%
% INPUTS
% u: input, cellarray(nTrials,1), where each trial is numericarray{double}(nU, nSamps)
% z: measurement, cellarray(nTrials,1), where each trial is numericarray{double}(nY, nSamps)
% t0: initial time for [u; n], numericarray{double}(nTrials,1)
% dt: sample period (double)
% nX: number of states/order (double)
% nH: size of block-hankel matrix (nH=20)
% d0: previously-determined value for output bias
% whichWt: no weighting [0], MOESP [1], CVA [2]
% wtG0: relative weight (WLS) put on getting DC gain (G0) correct when calculating B.
% t_startSSID: start time for fitting dynamics via SSID (double)
% t_stopSSID: stop time for fitting dynamics via SSID (double)
%
% 2019/12 : mfbolus
% 2020/03 : mfbolus (added whichWt)
% 2020/04 : mfbolus (added wtG0)
