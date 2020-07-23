function [yImp,d] = estimate_simo_yImp_ls(u,z,ord)
  % [yImp,d] = estimate_simo_yImp_ls(u,z,ord)
  %
  % Estimate a non-parametric impulse response by ordinary least squares.
  % Works for single-input, multi-output data (SIMO).
  %
  % INPUTS:
  % u : vector of input time series data.
  % z : matrix of measured (multi-)output data.
  % ord : order of the impulse response (i.e., number of taps)
  %
  % OUTPUTS:
  % yImp : Impulse response (nY x nTime)
  % d : Output offset/bias (nY x 1)
  %
  % TODO: Estimate covariance of measurement noise (R). Filter stimulus with yImp, calculate the residuals, and calculate their covariance (nY x nY matrix).

  % Originally written with time in first dimension, so make sure to transpose if necessary.
  if isrow(u)
    u = u';
    z = z';
  end

  if ~iscolumn(u)
    error('This only works for single-input data.')
  end

  % Create stimulus lag-matrix
  % (Should probably use some variant of convmtx(u,ord) for better efficiency.)
  U = zeros(length(u), ord);
  buff = [zeros(1,ord-1), u(1)]; %flip( u(1:ord,1)' );
  U(1,:) = buff;
  for i = 2:length(u)
    buff = [buff(2:(end)), u(i)];
    U(i,:) = buff;
  end

	% add extra ones for output bias est.
  U = [ones(length(u),1), U];

  % Do linear regression.
  yImp = pinv(U) * z;
  d = yImp(1,:)';
  yImp = flip(yImp(2:end,:))';
end %end fn
