function [] = save_params_rtxi_pldsSimulator(this, savename)
  % [] = save_params_rtxi_pldsSimulator(this, savename)

  dt = this.dt;
  A = this.A;
  B = this.B;
  g = this.g;
  m = this.m;
  C = this.C;
  d = this.d;
  x0 = this.x0;

  % just insert small values if Q is not assigned.
  Q = this.Q;
  P0 = this.P0;
  if isempty(this.Q)
    Q = 1e-10*eye(size(x0,1));
    P0 = zeros(size(x0,1));
  end

  save(savename, 'dt', 'A', 'B', 'g', 'm', 'C', 'd', 'x0', 'P0', 'Q');
end
