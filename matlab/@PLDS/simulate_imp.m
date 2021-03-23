function [logyImpC, tImp] = simulate_imp(this, nSamps)
  % [logyImpC, tImp] = simulate_imp(this, nSamps)

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

  logyImpC = cell(nU,1);
  for whichU = 1:nU
    logyImpC{whichU} = zeros(nY,nSamps);
    x = zeros(nX, 1);
    for k=2:nSamps
      x = this.A*x + this.B(:,whichU)*this.g(whichU).*uImp(k-1);
      logyImpC{whichU}(:,k) = this.C*x;
    end
  end
end
