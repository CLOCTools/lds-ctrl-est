function [] = save_params_controller_cat_plds(filename, sys_array, Kc_array, y_ref)
  % [] =  save_params_controller_cat_plds(filename, sys_array, Kc_array, y_ref)
  %
  % Saves parameters for multiple PLDS systems in a conCATenated form for switched control applications.
  %
  % filename : name of hdf5 file created
  % sys_array : array of PLDS system objects
  % Kc_array : cell array of feedback controller gains (Kc)
  % y_ref : reference output (n_y x 1)
  %
  % mfbolus 2021/06
  %

  % make sure they are PLDS objects
  if !isa(sys_array(1),'PLDS')
    error('sys_array contents must be PLDS objects.')
  end

  % make sure sys_array, Kc_array are vectors
  if (size(sys_array,1)>1) && (size(sys_array,2)>1)
    error('sys_array must be a vector object array.')
  end

  if (size(Kc_array,1)>1) && (size(Kc_array,2)>1)
    error('Kc_array must be a vector cell array.')
  end

  if (size(y_ref,1)>1) && (size(y_ref,2)>1)
    error('y_ref must be a vector.')
  end

  fcpl = H5P.create('H5P_FILE_CREATE');
  fapl = H5P.create('H5P_FILE_ACCESS');
  fid = H5F.create(filename, 'H5F_ACC_TRUNC',fcpl,fapl);

  dt = sys_array(1).dt;
  h5create(filename, '/dt', 1);
  h5write(filename, '/dt', dt);

  A_cell = {sys_array.A};
  h5write_force_3d(filename, 'A', A_cell);

  B_cell = {sys_array.B};
  h5write_force_3d(filename, 'B', B_cell);

  % assuming g is same across systems...
  h5create(filename, '/g', [length(sys_array(1).g),1]);
  h5write(filename, '/g', sys_array(1).g);

  m_cell = {sys_array.m};
  h5write_force_2d(filename, 'm', m_cell);

  C_cell = {sys_array.C};
  h5write_force_3d(filename, 'C', C_cell);

  d_cell = {sys_array.d};
  h5write_force_2d(filename, 'd', d_cell);

  Q_cell = {sys_array.Q};
  h5write_force_3d(filename, 'Q', Q_cell);

  x0_cell = {sys_array.x0};
  h5write_force_2d(filename, 'x0', x0_cell);

  P0_cell = {sys_array.P0};
  h5write_force_3d(filename, 'P0', P0_cell);

  h5write_force_3d(filename, 'Kc', Kc_array);

  h5create(filename, '/y_ref', [length(y_ref),1]);
  h5write(filename, '/y_ref', y_ref);

  n_sys=length(sys_array);
  h5create(filename, '/n_sys', 1);
  h5write(filename, '/n_sys', n_sys);

  H5F.close(fid);
end

function h5write_force_2d(filename, key, val_cell)
  val = cat(2,val_cell{:});
  sz = size(val);
  h5create(filename, ['/' key], [sz(1),sz(2)]);
  h5write(filename, ['/' key], val);
end

function h5write_force_3d(filename, key, val_cell)
  val = cat(3,val_cell{:});
  sz = size(val);
  if length(sz)<3
    sz = [sz, 1];
  end
  h5create(filename, ['/' key], [sz(1),sz(2),sz(3)]);
  h5write(filename, ['/' key], val);
end
