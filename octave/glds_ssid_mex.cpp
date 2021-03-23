#include <ldsCtrlEst_h/mex_c_util.h>

#include <iostream>

using lds::data_t;
using lds::Matrix;
using lds::Vector;

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[]) {
  if (nrhs < 4) {
    mexErrMsgTxt("Not enough input arguments.");
    return;
  }

  // Input Parameters
  lds::UniformMatrixList<lds::kMatFreeDim2> u;
  lds::UniformMatrixList<lds::kMatFreeDim2> z;
  data_t dt(1);
  size_t n_x(1);
  size_t n_h(50);
  lds::Vector d0 = lds::Vector(1).fill(-lds::kInf);
  bool force_unit_norm_c(false);
  size_t which_wt(0);
  std::vector<data_t> t0;
  data_t t_start_ssid(-lds::kInf);
  data_t t_stop_ssid(lds::kInf);

  size_t n_trials = mxGetM(prhs[0]);  // asuming rows only
  if (n_trials != mxGetM(prhs[1])) {
    mexErrMsgTxt("Input/Output data must have the same number of trials.");
    return;
  }

  // get input data
  {
    std::vector<Matrix> tmp(n_trials);
    for (size_t trial = 0; trial < n_trials; trial++) {
      tmp[trial] =
          std::move(armamexc::m2a_mat<data_t>(mxGetCell(prhs[0], trial)));
    }
    u = lds::UniformMatrixList<lds::kMatFreeDim2>(std::move(tmp));
  }

  // get measurement data
  {
    std::vector<Matrix> tmp(n_trials);
    for (size_t trial = 0; trial < n_trials; trial++) {
      tmp[trial] =
          std::move(armamexc::m2a_mat<data_t>(mxGetCell(prhs[1], trial)));
    }
    z = lds::UniformMatrixList<lds::kMatFreeDim2>(std::move(tmp));
  }

  dt = armamexc::m2T_scalar<data_t>(prhs[2]);
  n_x = armamexc::m2T_scalar<size_t>(prhs[3]);

  if (nrhs > 4) {
    n_h = armamexc::m2T_scalar<size_t>(prhs[4]);
  }

  if (nrhs > 5) {
    d0 = armamexc::m2a_mat<data_t>(prhs[5]);
  }

  if (nrhs > 6) {
    which_wt = armamexc::m2T_scalar<size_t>(prhs[6]);
  }

  lds::SSIDWt wt = lds::kSSIDNone;
  switch (which_wt) {
    case 0: {
      wt = lds::kSSIDNone;
    } break;

    case 1: {
      wt = lds::kSSIDMOESP;
    } break;

    case 2: {
      wt = lds::kSSIDCVA;
    } break;
  }

  lds::gaussian::FitSSID ssid(n_x, n_h, dt, std::move(u), std::move(z), d0);

  lds::gaussian::Fit lds_fit;
  lds::Vector sing_vals_fit;
  std::tie(lds_fit, sing_vals_fit) = ssid.Run(wt);

  if (nlhs > 0) {
    mwSize dims[2] = {1, 1};
    const char* keys[10] = {"A", "B", "C", "d", "g", "m", "x0", "P0", "Q", "R"};
    plhs[0] = mxCreateStructArray(2, static_cast<mwSize*>(dims), 10,
                                  static_cast<const char**>(keys));
    mxSetField(plhs[0], 0, "A", armamexc::a2m_mat(lds_fit.A()));
    mxSetField(plhs[0], 0, "B", armamexc::a2m_mat(lds_fit.B()));
    mxSetField(plhs[0], 0, "C", armamexc::a2m_mat(lds_fit.C()));
    mxSetField(plhs[0], 0, "d", armamexc::a2m_mat(lds_fit.d()));
    mxSetField(plhs[0], 0, "g", armamexc::a2m_mat(lds_fit.g()));
    mxSetField(plhs[0], 0, "m", armamexc::a2m_mat(lds_fit.m()));
    mxSetField(plhs[0], 0, "x0", armamexc::a2m_mat(lds_fit.x0()));
    mxSetField(plhs[0], 0, "P0", armamexc::a2m_mat(lds_fit.P0()));
    mxSetField(plhs[0], 0, "Q", armamexc::a2m_mat(lds_fit.Q()));
    mxSetField(plhs[0], 0, "R", armamexc::a2m_mat(lds_fit.R()));
  }

  if (nlhs > 1) {
    mxArray* sing_vals = armamexc::a2m_vec(sing_vals_fit);
    plhs[1] = sing_vals;
  }
}
