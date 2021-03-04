#include <ldsCtrlEst_h/mex_cpp_util.h>

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
    size_t whichWt(0);
    lds::data_t wtG0(0);

    lds::data_t t_startSSID(-lds::inf);
    lds::data_t t_stopSSID(lds::inf);

    matlab::data::Array gldsFit0 = std::move(inputs[0]);
    matlab::data::Array pldsFit0 = std::move(inputs[1]);
    matlab::data::CellArray u_matlab = std::move(inputs[2]);
    matlab::data::CellArray z_matlab = std::move(inputs[3]);
    dt = inputs[4][0];
    nX = inputs[5][0];

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
      TypedArray<double> d0_matlab = std::move(inputs[7]);
      d0 = armamexcpp::m2a_vec<double>(d0_matlab);
    }

    if (nInputs > 6) {
      nH = (size_t)inputs[6][0];
    }

    std::vector<glds::armaMat> u =
        std::move(armamexcpp::m2a_cellmat<double>(u_matlab));
    std::vector<glds::armaMat> z =
        std::move(armamexcpp::m2a_cellmat<double>(z_matlab));

    plds::ssidFit_t pldsFit;
    glds::ssidFit_t gldsFit;

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
    std::tie(pldsFit, gldsFit) = plds::ssidFit(u, z, dt, nX, nH, d0, wt, wtG0,
                                               t0, t_startSSID, t_stopSSID);

    TypedArray<double> singVals =
        armamexcpp::a2m_vec<double>(gldsFit.singVals, factory);

    // first do PLDS
    TypedArray<double> A = armamexcpp::a2m_mat<double>(pldsFit.A, factory);
    TypedArray<double> B = armamexcpp::a2m_mat<double>(pldsFit.B, factory);
    TypedArray<double> g = armamexcpp::a2m_vec<double>(pldsFit.g, factory);
    TypedArray<double> m = armamexcpp::a2m_vec<double>(pldsFit.m, factory);
    TypedArray<double> Q = armamexcpp::a2m_mat<double>(pldsFit.Q, factory);
    TypedArray<double> x0 = armamexcpp::a2m_vec<double>(pldsFit.x0, factory);
    TypedArray<double> P0 = armamexcpp::a2m_mat<double>(pldsFit.P0, factory);
    TypedArray<double> C = armamexcpp::a2m_mat<double>(pldsFit.C, factory);
    TypedArray<double> d = armamexcpp::a2m_vec<double>(pldsFit.d, factory);
    matlabPtr->setProperty(pldsFit0, u"dt", factory.createScalar(dt));
    matlabPtr->setProperty(pldsFit0, u"A", A);
    matlabPtr->setProperty(pldsFit0, u"B", B);
    matlabPtr->setProperty(pldsFit0, u"g", g);
    matlabPtr->setProperty(pldsFit0, u"m", m);
    matlabPtr->setProperty(pldsFit0, u"Q", Q);
    matlabPtr->setProperty(pldsFit0, u"C", C);
    matlabPtr->setProperty(pldsFit0, u"d", d);
    matlabPtr->setProperty(pldsFit0, u"x0", x0);
    matlabPtr->setProperty(pldsFit0, u"P0", P0);

    A = armamexcpp::a2m_mat<double>(gldsFit.A, factory);
    B = armamexcpp::a2m_mat<double>(gldsFit.B, factory);
    g = armamexcpp::a2m_vec<double>(gldsFit.g, factory);
    m = armamexcpp::a2m_vec<double>(gldsFit.m, factory);
    Q = armamexcpp::a2m_mat<double>(gldsFit.Q, factory);
    x0 = armamexcpp::a2m_vec<double>(gldsFit.x0, factory);
    P0 = armamexcpp::a2m_mat<double>(gldsFit.P0, factory);
    C = armamexcpp::a2m_mat<double>(gldsFit.C, factory);
    // D = armamexcpp::a2m_mat<double>(gldsFit.D, factory);
    d = armamexcpp::a2m_vec<double>(gldsFit.d, factory);
    TypedArray<double> R = armamexcpp::a2m_mat<double>(gldsFit.R, factory);
    singVals = armamexcpp::a2m_vec<double>(gldsFit.singVals, factory);
    matlabPtr->setProperty(gldsFit0, u"dt", factory.createScalar(dt));
    matlabPtr->setProperty(gldsFit0, u"A", A);
    matlabPtr->setProperty(gldsFit0, u"B", B);
    matlabPtr->setProperty(gldsFit0, u"g", g);
    matlabPtr->setProperty(gldsFit0, u"m", m);
    matlabPtr->setProperty(gldsFit0, u"Q", Q);
    matlabPtr->setProperty(gldsFit0, u"C", C);
    matlabPtr->setProperty(gldsFit0, u"d", d);
    matlabPtr->setProperty(gldsFit0, u"R", R);
    matlabPtr->setProperty(gldsFit0, u"x0", x0);
    matlabPtr->setProperty(gldsFit0, u"P0", P0);

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
    if (inputs.size() < 7) {
      displayError("At least 7 arguments required.");
    }
    if (outputs.size() > 1) {
      displayError("Too many outputs specified.");
    }
  }
};
