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
    size_t n_x(1);
    size_t n_h(25);
    lds::Vector d0 = lds::Vector(1).fill(-lds::kInf);
    bool force_unit_norm_c(false);
    size_t which_wt(0);

    lds::data_t t_start_ssid(-lds::kInf);
    lds::data_t t_stop_ssid(lds::kInf);

    matlab::data::Array fit0 = std::move(inputs[0]);
    matlab::data::CellArray u_matlab = std::move(inputs[1]);
    matlab::data::CellArray z_matlab = std::move(inputs[2]);
    dt = inputs[3][0];
    n_x = inputs[4][0];

    size_t n_inputs = inputs.size();
    if (n_inputs > 5) {
      n_h = static_cast<size_t>(inputs[5][0]);
    }
    if (n_inputs > 6) {
      TypedArray<data_t> d0_matlab = std::move(inputs[6]);
      d0 = armamexcpp::m2a_vec<data_t>(d0_matlab);
    }
    if (n_inputs > 7) {
      which_wt = static_cast<size_t>(inputs[7][0]);
    }

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

    lds::gaussian::FitSSID ssid(n_x, n_h, dt, std::move(u),
                                                    std::move(z), d0);

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

    lds::gaussian::Fit lds_fit;
    lds::Vector sing_vals_fit;
    std::tie(lds_fit, sing_vals_fit) = ssid.Run(wt);

    TypedArray<data_t> a = armamexcpp::a2m_mat<data_t>(lds_fit.A(), factory);
    TypedArray<data_t> b = armamexcpp::a2m_mat<data_t>(lds_fit.B(), factory);
    TypedArray<data_t> g = armamexcpp::a2m_vec<data_t>(lds_fit.g(), factory);
    TypedArray<data_t> m = armamexcpp::a2m_vec<data_t>(lds_fit.m(), factory);
    TypedArray<data_t> q = armamexcpp::a2m_mat<data_t>(lds_fit.Q(), factory);
    TypedArray<data_t> x0 = armamexcpp::a2m_vec<data_t>(lds_fit.x0(), factory);
    TypedArray<data_t> p0 = armamexcpp::a2m_mat<data_t>(lds_fit.P0(), factory);
    TypedArray<data_t> c = armamexcpp::a2m_mat<data_t>(lds_fit.C(), factory);
    TypedArray<data_t> d = armamexcpp::a2m_vec<data_t>(lds_fit.d(), factory);
    TypedArray<data_t> r = armamexcpp::a2m_mat<data_t>(lds_fit.R(), factory);

    matlab_ptr_->setProperty(fit0, u"dt", factory.createScalar(dt));
    matlab_ptr_->setProperty(fit0, u"A", a);
    matlab_ptr_->setProperty(fit0, u"B", b);
    matlab_ptr_->setProperty(fit0, u"g", g);
    matlab_ptr_->setProperty(fit0, u"m", m);
    matlab_ptr_->setProperty(fit0, u"Q", q);
    matlab_ptr_->setProperty(fit0, u"C", c);
    matlab_ptr_->setProperty(fit0, u"d", d);
    matlab_ptr_->setProperty(fit0, u"R", r);
    matlab_ptr_->setProperty(fit0, u"x0", x0);
    matlab_ptr_->setProperty(fit0, u"P0", p0);
    if (outputs.size() > 0) {
      TypedArray<data_t> sing_vals =
          armamexcpp::a2m_vec<data_t>(sing_vals_fit, factory);
      outputs[0] = std::move(sing_vals);
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
    if (outputs.size() > 1) {
      displayError("Too many outputs specified.");
    }
  }
};
