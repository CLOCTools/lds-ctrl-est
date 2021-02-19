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
    bool calcAB = true;
    bool calcQ = true;
    bool calcInitial = true;
    bool calcC = true;
    bool calcd = true;
    bool calcR = true;
    size_t maxIter = 100;
    lds::data_t tol = 1e-3;

    // original fit.
    matlab::data::Array fit0 = std::move(inputs[0]);
    matlab::data::CellArray u_matlab = std::move(inputs[1]);
    matlab::data::CellArray z_matlab = std::move(inputs[2]);
    dt = (matlabPtr->getProperty(fit0, u"dt"))[0];

    std::vector<glds::armaMat> u =
        std::move(armamexcpp::m2a_cellmat<double>(u_matlab));
    std::vector<glds::armaMat> z =
        std::move(armamexcpp::m2a_cellmat<double>(z_matlab));

    if (inputs.size() > 3) {
      calcAB = (bool)inputs[3][0];
    };
    if (inputs.size() > 4) {
      calcQ = (bool)inputs[4][0];
    };
    if (inputs.size() > 5) {
      calcInitial = (bool)inputs[5][0];
    };
    if (inputs.size() > 6) {
      calcC = (bool)inputs[6][0];
    };
    if (inputs.size() > 7) {
      calcd = (bool)inputs[7][0];
    };
    if (inputs.size() > 8) {
      calcR = (bool)inputs[8][0];
    };
    if (inputs.size() > 9) {
      maxIter = (size_t)inputs[9][0];
    };
    if (inputs.size() > 10) {
      tol = (lds::data_t)inputs[10][0];
    };

    // get the model parameters.
    lds::armaMat A_0 =
        armamexcpp::m2a_mat<double>(matlabPtr->getProperty(fit0, u"A"));
    lds::armaMat B_0 =
        armamexcpp::m2a_mat<double>(matlabPtr->getProperty(fit0, u"B"));
    lds::armaVec g_0 =
        armamexcpp::m2a_vec<double>(matlabPtr->getProperty(fit0, u"g"));
    lds::armaVec m_0 =
        armamexcpp::m2a_vec<double>(matlabPtr->getProperty(fit0, u"m"));
    lds::armaMat Q_0 =
        armamexcpp::m2a_mat<double>(matlabPtr->getProperty(fit0, u"Q"));
    lds::armaVec x0_0 =
        armamexcpp::m2a_vec<double>(matlabPtr->getProperty(fit0, u"x0"));
    lds::armaMat P0_0 =
        armamexcpp::m2a_mat<double>(matlabPtr->getProperty(fit0, u"P0"));
    lds::armaMat C_0 =
        armamexcpp::m2a_mat<double>(matlabPtr->getProperty(fit0, u"C"));
    lds::armaVec d_0 =
        armamexcpp::m2a_vec<double>(matlabPtr->getProperty(fit0, u"d"));
    lds::armaMat R_0 =
        armamexcpp::m2a_mat<double>(matlabPtr->getProperty(fit0, u"R"));

    glds::emFit_t ldsFit(A_0, B_0, g_0, m_0, Q_0, x0_0, P0_0, C_0, d_0, R_0, dt,
                         u, z);
    ldsFit.maxIter = maxIter;
    ldsFit.tol = tol;

    ldsFit.runEM(calcAB, calcQ, calcInitial, calcC, calcd, calcR);
    lds::armaMat D0(ldsFit.C.n_rows, ldsFit.B.n_cols, arma::fill::zeros);

    TypedArray<double> A = armamexcpp::a2m_mat<double>(ldsFit.A, factory);
    TypedArray<double> B = armamexcpp::a2m_mat<double>(ldsFit.B, factory);
    TypedArray<double> g = armamexcpp::a2m_vec<double>(ldsFit.g, factory);
    TypedArray<double> m = armamexcpp::a2m_vec<double>(ldsFit.m, factory);
    TypedArray<double> Q = armamexcpp::a2m_mat<double>(ldsFit.Q, factory);
    TypedArray<double> x0 = armamexcpp::a2m_vec<double>(ldsFit.x0, factory);
    TypedArray<double> P0 = armamexcpp::a2m_mat<double>(ldsFit.P0, factory);
    TypedArray<double> C = armamexcpp::a2m_mat<double>(ldsFit.C, factory);
    TypedArray<double> D = armamexcpp::a2m_mat<double>(D0, factory);
    TypedArray<double> d = armamexcpp::a2m_vec<double>(ldsFit.d, factory);
    TypedArray<double> R = armamexcpp::a2m_mat<double>(ldsFit.R, factory);

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
      std::vector<lds::armaMat> yHat = ldsFit.getY();
      size_t nTrials = yHat.size();
      matlab::data::CellArray yHat_m = factory.createCellArray({nTrials, 1});
      for (size_t trial = 0; trial < nTrials; trial++) {
        yHat_m[trial] = std::move(armamexcpp::a2m_mat<double>(yHat[trial], factory));
      }
      outputs[0] = std::move(yHat_m);
    }

    if (outputs.size() > 1) {
      std::vector<lds::armaMat> xHat = ldsFit.getX();
      size_t nTrials = xHat.size();
      matlab::data::CellArray xHat_m = factory.createCellArray({nTrials, 1});
      for (size_t trial = 0; trial < nTrials; trial++) {
        xHat_m[trial] = std::move(armamexcpp::a2m_mat<double>(xHat[trial], factory));
      }
      outputs[1] = std::move(xHat_m);
    }

    if (outputs.size() > 2) {
      lds::armaMat sum_E_xu_tm1_xu_tm1 = ldsFit.get_sum_E_xu_tm1_xu_tm1();
      TypedArray<double> sum_E_xu_tm1_xu_tm1_m =
          armamexcpp::a2m_mat<double>(sum_E_xu_tm1_xu_tm1, factory);
      outputs[2] = std::move(sum_E_xu_tm1_xu_tm1_m);
    }
    if (outputs.size() > 3) {
      lds::armaMat sum_E_xu_t_xu_tm1 = ldsFit.get_sum_E_xu_t_xu_tm1();
      TypedArray<double> sum_E_xu_t_xu_tm1_m =
          armamexcpp::a2m_mat<double>(sum_E_xu_t_xu_tm1, factory);
      outputs[3] = std::move(sum_E_xu_t_xu_tm1_m);
    }
    if (outputs.size() > 4) {
      lds::armaMat sum_E_xu_t_xu_t = ldsFit.get_sum_E_xu_t_xu_t();
      TypedArray<double> sum_E_xu_t_xu_t_m =
          armamexcpp::a2m_mat<double>(sum_E_xu_t_xu_t, factory);
      outputs[4] = std::move(sum_E_xu_t_xu_t_m);
    }
    if (outputs.size() > 5) {
      size_t T = ldsFit.getT();
      TypedArray<double> T_m = factory.createScalar<double>(T);
      outputs[5] = std::move(T_m);
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
    if (outputs.size() > 6) {
      displayError("Too many outputs requested.");
    }
  }
};
