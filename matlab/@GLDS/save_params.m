function [] = save_params(this, savename, qM)
  % [] = save_params(this, savename, qM)

  dt = this.dt;
  A = this.A;
  B = this.B;
  g = this.g;
  m = this.m;
  C = this.C;
  d = this.d;
  x0 = this.x0;

  % calculate the steady state Kalman gains.
  if isempty(this.K)
    this.calcK_steadyState();
  end
  Ke = this.K;
  Ke_m = this.calcKm_dual_steadyState(qM);

  save(savename, '-v7.3', 'dt', 'A', 'B', 'g', 'm', 'C', 'd', 'x0', 'Ke', 'Ke_m');
end
