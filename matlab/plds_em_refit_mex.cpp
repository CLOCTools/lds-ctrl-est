#include <ldsCtrlEst_h/mex_cpp_util.h>

using matlab::data::Array;
using matlab::data::ArrayFactory;
using matlab::data::TypedArray;
using matlab::mex::ArgumentList;

using lds::data_t;
using lds::Matrix;
using lds::Vector;

class MexFunction : public matlab::mex::Function {
 private:
  std::shared_ptr<matlab::engine::MATLABEngine> matlab_ptr_;

 public:
  /* Constructor for the class. */
  MexFunction() { matlab_ptr_ = getEngine(); }

  void operator()(ArgumentList outputs, ArgumentList inputs) {
    checkArguments(outputs, inputs);
    matlab::data::ArrayFactory factory;

    // Input Parameters
    lds::data_t dt(1);
    bool calc_dynamics = true;
    bool calc_q = true;
    bool calc_initial = true;
    bool calc_output = true;
    bool calc_d = true;
    bool calc_measurement = true;
    size_t max_iter = 100;
    lds::data_t tol = 1e-3;

    // original fit.
    matlab::data::Array fit0 = std::move(inputs[0]);
    matlab::data::CellArray u_matlab = std::move(inputs[1]);
    matlab::data::CellArray z_matlab = std::move(inputs[2]);
    dt = (matlab_ptr_->getProperty(fit0, u"dt"))[0];

    lds::UniformMatrixList<lds::kMatFreeDim2> u;
    lds::UniformMatrixList<lds::kMatFreeDim2> z;
    {
      std::vector<lds::Matrix> tmp =
          std::move(armamexcpp::m2a_cellmat<data_t>(u_matlab));
      u = lds::UniformMatrixList<lds::kMatFreeDim2>(std::move(tmp));
    }
    {
      std::vector<lds::Matrix> tmp =
          std::move(armamexcpp::m2a_cellmat<data_t>(z_matlab));
      z = lds::UniformMatrixList<lds::kMatFreeDim2>(std::move(tmp));
    }

    if (inputs.size() > 3) {
      calc_dynamics = static_cast<bool>(inputs[3][0]);
    };
    if (inputs.size() > 4) {
      calc_q = static_cast<bool>(inputs[4][0]);
    };
    if (inputs.size() > 5) {
      calc_initial = static_cast<bool>(inputs[5][0]);
    };
    if (inputs.size() > 6) {
      calc_output = static_cast<bool>(inputs[6][0]);
    };
    if (inputs.size() > 7) {
      calc_measurement = static_cast<bool>(inputs[7][0]);
    };
    if (inputs.size() > 8) {
      max_iter = static_cast<size_t>(inputs[8][0]);
    };
    if (inputs.size() > 9) {
      tol = static_cast<lds::data_t>(inputs[9][0]);
    };

    // get the model parameters.
    lds::Matrix a_0 =
        armamexcpp::m2a_mat<data_t>(matlab_ptr_->getProperty(fit0, u"A"));
    lds::Matrix b_0 =
        armamexcpp::m2a_mat<data_t>(matlab_ptr_->getProperty(fit0, u"B"));
    lds::Vector g_0 =
        armamexcpp::m2a_vec<data_t>(matlab_ptr_->getProperty(fit0, u"g"));
    lds::Vector m_0 =
        armamexcpp::m2a_vec<data_t>(matlab_ptr_->getProperty(fit0, u"m"));
    lds::Matrix q_0 =
        armamexcpp::m2a_mat<data_t>(matlab_ptr_->getProperty(fit0, u"Q"));
    lds::Vector x0_0 =
        armamexcpp::m2a_vec<data_t>(matlab_ptr_->getProperty(fit0, u"x0"));
    lds::Matrix p0_0 =
        armamexcpp::m2a_mat<data_t>(matlab_ptr_->getProperty(fit0, u"P0"));
    lds::Matrix c_0 =
        armamexcpp::m2a_mat<data_t>(matlab_ptr_->getProperty(fit0, u"C"));
    lds::Vector d_0 =
        armamexcpp::m2a_vec<data_t>(matlab_ptr_->getProperty(fit0, u"d"));

    lds::poisson::Fit lds_fit(b_0.n_cols, a_0.n_rows, c_0.n_rows, dt);
    lds_fit.set_A(a_0);
    lds_fit.set_B(b_0);
    lds_fit.set_g(g_0);
    lds_fit.set_m(m_0);
    lds_fit.set_Q(q_0);
    lds_fit.set_x0(x0_0);
    lds_fit.set_P0(p0_0);
    lds_fit.set_C(c_0);
    lds_fit.set_d(d_0);

    lds::poisson::FitEM em(lds_fit, std::move(u), std::move(z));
    lds_fit = em.Run(calc_dynamics, calc_q, calc_initial, calc_output,
                     calc_measurement, max_iter, tol);

    // TODO(mfbolus): this causes seg fault:
    // std::tie(u, z) = em.ReturnData();

    TypedArray<data_t> a = armamexcpp::a2m_mat<data_t>(lds_fit.A(), factory);
    TypedArray<data_t> b = armamexcpp::a2m_mat<data_t>(lds_fit.B(), factory);
    TypedArray<data_t> g = armamexcpp::a2m_vec<data_t>(lds_fit.g(), factory);
    TypedArray<data_t> m = armamexcpp::a2m_vec<data_t>(lds_fit.m(), factory);
    TypedArray<data_t> q = armamexcpp::a2m_mat<data_t>(lds_fit.Q(), factory);
    TypedArray<data_t> x0 = armamexcpp::a2m_vec<data_t>(lds_fit.x0(), factory);
    TypedArray<data_t> p0 = armamexcpp::a2m_mat<data_t>(lds_fit.P0(), factory);
    TypedArray<data_t> c = armamexcpp::a2m_mat<data_t>(lds_fit.C(), factory);
    TypedArray<data_t> d = armamexcpp::a2m_vec<data_t>(lds_fit.d(), factory);

    matlab_ptr_->setProperty(fit0, u"A", a);
    matlab_ptr_->setProperty(fit0, u"B", b);
    matlab_ptr_->setProperty(fit0, u"g", g);
    matlab_ptr_->setProperty(fit0, u"m", m);
    matlab_ptr_->setProperty(fit0, u"Q", q);
    matlab_ptr_->setProperty(fit0, u"C", c);
    matlab_ptr_->setProperty(fit0, u"d", d);
    matlab_ptr_->setProperty(fit0, u"x0", x0);
    matlab_ptr_->setProperty(fit0, u"P0", p0);

    if (outputs.size() > 0) {
      std::vector<lds::Matrix> y_hat = em.y();
      size_t n_trials = y_hat.size();
      matlab::data::CellArray y_hat_m = factory.createCellArray({n_trials, 1});
      for (size_t trial = 0; trial < n_trials; trial++) {
        y_hat_m[trial] =
            std::move(armamexcpp::a2m_mat<data_t>(y_hat[trial], factory));
      }
      outputs[0] = std::move(y_hat_m);
    }

    if (outputs.size() > 1) {
      std::vector<lds::Matrix> x_hat = em.x();
      size_t n_trials = x_hat.size();
      matlab::data::CellArray x_hat_m = factory.createCellArray({n_trials, 1});
      for (size_t trial = 0; trial < n_trials; trial++) {
        x_hat_m[trial] =
            std::move(armamexcpp::a2m_mat<data_t>(x_hat[trial], factory));
      }
      outputs[1] = std::move(x_hat_m);
    }

    if (outputs.size() > 2) {
      lds::Matrix sum_e_xu_tm1_xu_tm1 = em.sum_E_xu_tm1_xu_tm1();
      TypedArray<data_t> sum_e_xu_tm1_xu_tm1_m =
          armamexcpp::a2m_mat<data_t>(sum_e_xu_tm1_xu_tm1, factory);
      outputs[2] = std::move(sum_e_xu_tm1_xu_tm1_m);
    }
    if (outputs.size() > 3) {
      lds::Matrix sum_e_xu_t_xu_tm1 = em.sum_E_xu_t_xu_tm1();
      TypedArray<data_t> sum_e_xu_t_xu_tm1_m =
          armamexcpp::a2m_mat<data_t>(sum_e_xu_t_xu_tm1, factory);
      outputs[3] = std::move(sum_e_xu_t_xu_tm1_m);
    }
    if (outputs.size() > 4) {
      lds::Matrix sum_e_x_t_x_t = em.sum_E_x_t_x_t();
      TypedArray<data_t> sum_e_x_t_x_t_m =
          armamexcpp::a2m_mat<data_t>(sum_e_x_t_x_t, factory);
      outputs[4] = std::move(sum_e_x_t_x_t_m);
    }
    if (outputs.size() > 5) {
      size_t t = em.n_t_tot();
      TypedArray<data_t> t_m = factory.createScalar<data_t>(t);
      outputs[5] = std::move(t_m);
    }
  }

  /* Helper function to print output string on MATLAB command prompt. */
  // void displayOnMATLAB(std::ostringstream stream)
  // {
  //   ArrayFactory factory;
  //   matlabPtr->feval(u"fprintf", 0, std::vector<Array>
  //           ({ factory.createScalar(stream.str())}));
  // }

  /* Helper function to generate an error message from given string,
   * and display it over MATLAB command prompt.
   */
  void displayError(const std::string& error_message) {
    ArrayFactory factory;
    matlab_ptr_->feval(
        u"error", 0, std::vector<Array>({factory.createScalar(error_message)}));
  }

  void checkArguments(ArgumentList outputs, ArgumentList inputs) {
    if (inputs.size() < 6) {
      displayError("At least 6 arguments required.");
    }
    if (outputs.size() > 6) {
      displayError("Too many outputs requested.");
    }
  }
};
