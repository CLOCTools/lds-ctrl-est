function [] = save_params_controller(this, savename, Q_m, Kc, Kc_inty, Kc_u, y_ref)
  % [] = save_params_controller(this, savename, Q_m, Kc, Kc_inty, Kc_u, y_ref)

  dt = this.dt;
  A = this.A;
  B = this.B;
  g = this.g;
  m = this.m;
  C = this.C;
  d = this.d;
  Q = this.Q;
  x0 = this.x0;
  P0 = this.P0;
  n_sys = 1;

  save(savename, '-v7.3', 'dt', 'A', 'B', 'g', 'm', 'C', 'd', 'x0', 'P0', 'Q', 'Kc', 'n_sys');

  if ~isempty(Q_m)
    save(savename, '-v7.3', 'Q_m', '-append');
  end

  if ~isempty(Kc)
    save(savename, '-v7.3', 'Kc', '-append');
  end

  if ~isempty(Kc_inty)
    save(savename, '-v7.3', 'Kc_inty', '-append');
  end

  if ~isempty(Kc_u)
    save(savename, '-v7.3', 'Kc_u', '-append');
  end

  if nargin>6
    save(savename, '-v7.3', 'y_ref', '-append');
  end
end
