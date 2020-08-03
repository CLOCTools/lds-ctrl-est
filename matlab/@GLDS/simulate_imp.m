function [yImpC, tImp] = simulate_imp(this, nSamps)
  % [yImpC, tImp] = lds_sim_impulse(this, nSamps)

  if (nargin<2)
    nSamps = 100;
  end

  this.checkDims();

  tImp = (0:(nSamps-1))' * this.dt;
  uImp = zeros(nSamps,1);
  uImp(1) = 1;

  nU = size(this.B,2);
  nX = size(this.A,1);
  nY = size(this.C,1);

  yImpC = cell(nU,1);
  for whichU = 1:nU
    yImpC{whichU} = zeros(nY,nSamps);
    x = this.x0;%zeros(nX, 1);
    for k=2:nSamps
      x = this.A*x + this.B(:,whichU)*this.g(whichU).*uImp(k-1);
      yImpC{whichU}(:,k) = this.C*x;
    end
  end
end
