#include <ldsCtrlEst_h/mex_cpp_util.hpp>

using matlab::data::Array;
using matlab::data::ArrayFactory;
using matlab::data::TypedArray;
using matlab::mex::ArgumentList;

class MexFunction : public matlab::mex::Function {
 private:
  std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr;

 public:
  /* Constructor for the class. */
  MexFunction() { matlabPtr = getEngine(); }

  void operator()(ArgumentList outputs, ArgumentList inputs) {
    checkArguments(outputs, inputs);
    matlab::data::ArrayFactory factory;

    // Input Parameters
    lds::data_t dt(1);
    size_t nX(1);
    size_t nH(50);
    lds::armaVec d0 = lds::armaVec(1).fill(-lds::inf);
    bool force_unitNormC(false);
    size_t whichWt(0);
    lds::data_t wtG0(0);

    lds::data_t t_startSSID(-lds::inf);
    lds::data_t t_stopSSID(lds::inf);

    matlab::data::Array fit0 = std::move(inputs[0]);
    matlab::data::CellArray u_matlab = std::move(inputs[1]);
    matlab::data::CellArray z_matlab = std::move(inputs[2]);
    dt = inputs[3][0];
    nX = inputs[4][0];

    std::vector<lds::data_t> t0 =
        std::vector<lds::data_t>(z_matlab.getNumberOfElements(), 0.0);

    size_t nInputs = inputs.size();
    if (nInputs > 12) {
      t_stopSSID = (lds::data_t)inputs[12][0];
    }

    if (nInputs > 11) {
      t_startSSID = (lds::data_t)inputs[11][0];
    }

    if (nInputs > 10) {
      TypedArray<double> t0_matlab = std::move(inputs[10]);
      t0 = armamexcpp::m2s_vec<double>(t0_matlab);
    }

    if (nInputs > 9) {
      wtG0 = (lds::data_t)inputs[9][0];
    }

    if (nInputs > 8) {
      whichWt = (size_t)inputs[8][0];
    }

    if (nInputs > 7) {
      force_unitNormC = (bool)inputs[7][0];
    }

    if (nInputs > 6) {
      TypedArray<double> d0_matlab = std::move(inputs[6]);
      d0 = armamexcpp::m2a_vec<double>(d0_matlab);
    }

    if (nInputs > 5) {
      nH = (size_t)inputs[5][0];
    }

    std::vector<glds::armaMat> u =
        std::move(armamexcpp::m2a_cellmat<double>(u_matlab));
    std::vector<glds::armaMat> z =
        std::move(armamexcpp::m2a_cellmat<double>(z_matlab));

    glds::ssidFit_t ldsFit;

    lds::ssidWt wt = lds::NONE;
    switch (whichWt) {
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
    ldsFit = glds::ssidFit(u, z, dt, nX, nH, d0, force_unitNormC, wt, wtG0, t0,
                           t_startSSID, t_stopSSID);

    TypedArray<double> A = armamexcpp::a2m_mat<double>(ldsFit.A, factory);
    TypedArray<double> B = armamexcpp::a2m_mat<double>(ldsFit.B, factory);
    TypedArray<double> g = armamexcpp::a2m_vec<double>(ldsFit.g, factory);
    TypedArray<double> m = armamexcpp::a2m_vec<double>(ldsFit.m, factory);
    TypedArray<double> Q = armamexcpp::a2m_mat<double>(ldsFit.Q, factory);
    TypedArray<double> x0 = armamexcpp::a2m_vec<double>(ldsFit.x0, factory);
    TypedArray<double> P0 = armamexcpp::a2m_mat<double>(ldsFit.P0, factory);
    TypedArray<double> C = armamexcpp::a2m_mat<double>(ldsFit.C, factory);
    TypedArray<double> D = armamexcpp::a2m_mat<double>(ldsFit.D, factory);
    TypedArray<double> d = armamexcpp::a2m_vec<double>(ldsFit.d, factory);
    TypedArray<double> R = armamexcpp::a2m_mat<double>(ldsFit.R, factory);
    TypedArray<double> singVals =
        armamexcpp::a2m_vec<double>(ldsFit.singVals, factory);

    matlabPtr->setProperty(fit0, u"dt", factory.createScalar(dt));
    matlabPtr->setProperty(fit0, u"A", A);
    matlabPtr->setProperty(fit0, u"B", B);
    matlabPtr->setProperty(fit0, u"g", g);
    matlabPtr->setProperty(fit0, u"m", m);
    matlabPtr->setProperty(fit0, u"Q", Q);
    matlabPtr->setProperty(fit0, u"C", C);
    matlabPtr->setProperty(fit0, u"d", d);
    matlabPtr->setProperty(fit0, u"R", R);
    matlabPtr->setProperty(fit0, u"x0", x0);
    matlabPtr->setProperty(fit0, u"P0", P0);
    if (outputs.size() > 0) {
      outputs[0] = std::move(singVals);
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
  void displayError(std::string errorMessage) {
    ArrayFactory factory;
    matlabPtr->feval(u"error", 0,
                     std::vector<Array>({factory.createScalar(errorMessage)}));
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
