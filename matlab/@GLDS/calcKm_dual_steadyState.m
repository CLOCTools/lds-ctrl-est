function [Km,Pm] = calcK_steadyState(this, qM)
	%  [Km,Pm] = calcK_steadyState(this, qM)
	%
	% Calculates steady state estimator gain for dual estimation of disturbance m (Km)
	%

	this.checkDims();

	if (isempty(this.R))
		error('Cannot calculate Kalman gain without R.')
	end

	Qm = qM .* eye(this.nX);
	Pm = Qm;
	Km = zeros(this.nX,this.nY);

	sysM = GLDS(this.dt,eye(this.nX),zeros(this.nX,this.nU),this.g,zeros(this.nX,1),Qm,this.C,this.d,this.R,zeros(this.nX,1),Pm);
	Km = sysM.calcK_steadyState();
	Pm = sysM.P0;
end
