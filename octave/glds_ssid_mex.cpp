#include <iostream>
#include <ldsCtrlEst_h/mex_c_util.hpp>

///[fit, sing_vals] = glds_ssid_mex(u, z, dt, n_x, [n_h, d0, force_unit_norm_C,
///which_wt, wt_g0, t0, t_startSSID, t_stopSSID])
///
/// Fit dynamics of a Gaussian Linear Dynamical System (GLDS) by subspace
/// identification.
///
void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  if (nrhs < 4) {
    mexErrMsgTxt("Not enough input arguments.");
    return;
  }

  // Input Parameters
  std::vector<lds::armaMat> u;
  std::vector<lds::armaMat> z;
  double dt(1);
  size_t n_x(1);
  size_t n_h(50);
  lds::armaVec d0 = lds::armaVec(1).fill(-lds::inf);
  bool force_unit_norm_c(false);
  size_t which_wt(0);
  double wt_g0(0);
  std::vector<double> t0;
  double t_start_ssid(-lds::inf);
  double t_stop_ssid(lds::inf);

  size_t n_trials = mxGetM(prhs[0]);  // asuming rows only
  u = std::vector<arma::mat>(n_trials);
  for (size_t trial = 0; trial < n_trials; trial++) {
    u[trial] = std::move(armamexc::m2a_mat<double>(mxGetCell(prhs[0], trial)));
  }

  if (n_trials != mxGetM(prhs[1])) {
    mexErrMsgTxt("Input/Output data must have the same number of trials.");
    return;
  }

  z = std::vector<arma::mat>(n_trials);
  for (size_t trial = 0; trial < n_trials; trial++) {
    z[trial] = std::move(armamexc::m2a_mat<double>(mxGetCell(prhs[1], trial)));
  }

  dt = armamexc::m2T_scalar<double>(prhs[2]);
  n_x = armamexc::m2T_scalar<size_t>(prhs[3]);

  if (nrhs > 4) {
    n_h = armamexc::m2T_scalar<size_t>(prhs[4]);
  }

  if (nrhs > 5) {
    d0 = armamexc::m2a_mat<double>(prhs[5]);
  }

  if (nrhs > 6) {
    force_unit_norm_c = armamexc::m2T_scalar<bool>(prhs[6]);
  }

  if (nrhs > 7) {
    which_wt = armamexc::m2T_scalar<size_t>(prhs[7]);
  }

  if (nrhs > 8) {
    wt_g0 = armamexc::m2T_scalar<double>(prhs[8]);
  }

  t0 = std::vector<double>(n_trials, 0.0);
  if (nrhs > 9) {
    // TODO(mfbolus): run size check
    double* ptr = mxGetPr(prhs[9]);
    for (size_t trial = 0; trial < n_trials; trial++) {
      t0[trial] = ptr[trial];
    }
  }

  if (nrhs > 10) {
    t_start_ssid = armamexc::m2T_scalar<double>(prhs[10]);
    ;
  }

  if (nrhs > 11) {
    t_stop_ssid = armamexc::m2T_scalar<double>(prhs[11]);
  }

  lds::ssidWt wt = lds::NONE;
  switch (which_wt) {
    case 0: {
      wt = lds::NONE;
    } break;

    case 1: {
      wt = lds::MOESP;
    } break;

    case 2: {
      wt = lds::CVA;
    } break;
  }

  lds::gaussian::ssidFit_t lds_fit;
  lds_fit = lds::gaussian::ssidFit(u, z, dt, n_x, n_h, d0, force_unit_norm_c,
                                   wt, wt_g0, t0, t_start_ssid, t_stop_ssid);

  if (nlhs > 0) {
    mwSize dims[2] = {1, 1};
    const char* keys[10] = {"A", "B", "C", "D", "g", "m", "x0", "P0", "Q", "R"};
    plhs[0] = mxCreateStructArray(2, static_cast<mwSize*>(dims), 10,
                                  static_cast<const char**>(keys));
    mxSetField(plhs[0], 0, "A", armamexc::a2m_mat(lds_fit.A));
    mxSetField(plhs[0], 0, "B", armamexc::a2m_mat(lds_fit.B));
    mxSetField(plhs[0], 0, "C", armamexc::a2m_mat(lds_fit.C));
    mxSetField(plhs[0], 0, "D", armamexc::a2m_mat(lds_fit.D));
    mxSetField(plhs[0], 0, "g", armamexc::a2m_mat(lds_fit.g));
    mxSetField(plhs[0], 0, "m", armamexc::a2m_mat(lds_fit.m));
    mxSetField(plhs[0], 0, "x0", armamexc::a2m_mat(lds_fit.x0));
    mxSetField(plhs[0], 0, "P0", armamexc::a2m_mat(lds_fit.P0));
    mxSetField(plhs[0], 0, "Q", armamexc::a2m_mat(lds_fit.Q));
    mxSetField(plhs[0], 0, "R", armamexc::a2m_mat(lds_fit.R));
  }

  if (nlhs > 1) {
    mxArray* sing_vals = armamexc::a2m_vec(lds_fit.singVals);
    plhs[1] = sing_vals;
  }
}
