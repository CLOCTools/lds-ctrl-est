#ifndef LDS_SYS_HPP
#define LDS_SYS_HPP

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {

// define linear dynamical system (lds) class...
class sys_t {
 public:
  // constructor
  sys_t(std::size_t nU, std::size_t nX, data_t& dt, data_t& p0 = DEFAULT_P0,
        data_t& q0 = DEFAULT_Q0);
  sys_t& operator=(const sys_t& sys);

  // make one-step prediction
  void simPredict();

  // get methods
  size_t getNx() const { return nX; };
  size_t getNu() const { return nU; };
  armaVec getU() const { return u; };
  armaVec getX() { return x; };
  armaVec getG() const { return g; };
  armaVec getM() const { return m; };
  armaMat getA() const { return A; };
  armaMat getB() const { return B; };
  armaMat getQ() const { return Q; };
  armaMat getQ_m() const { return Q_m; };
  armaMat getP() const { return P; };
  armaMat getP_m() const { return P_m; };
  armaVec getX0() const { return x0; };
  armaMat getP0() const { return P0; };
  armaVec getM0() const { return m0; };
  armaMat getP0_m() const { return P0_m; };

  // set methods
  void setDims(std::size_t& nU, std::size_t& nX);
  void setU(stdVec& uVec);
  void setU(armaVec& u);
  void setA(stdVec& aVec);
  void setA(armaMat& A);
  void setB(stdVec& bVec);
  void setB(armaMat& B);
  void setM(stdVec& mVec);
  void setM(armaVec& m);
  void setG(stdVec& gVec);
  void setG(armaVec& g);
  void setQ(stdVec& qVec);
  void setQ(armaMat& Q);
  void setQ_m(stdVec& qmVec);
  void setQ_m(armaMat& Q_m);
  void setX0(stdVec& x0Vec);
  void setX0(armaVec& x0);
  void setP0(stdVec& p0Vec);
  void setP0(armaMat& P0);
  void setP0_m(stdVec& p0mVec);
  void setP0_m(armaMat& P0_m);

  // TODO: these are very redundant.
  // Should be able to use templates in some way to make this less type-specific
  void reassign(armaVec& oldVar, armaVec& newVar, data_t defaultVal = 0);
  void reassign(armaVec& oldVar, stdVec& newVar, data_t defaultVal = 0);
  void reassign(armaSubVec& oldVar, armaVec& newVar, data_t defaultVal = 0);
  void reassign(armaSubVec& oldVar, stdVec& newVar, data_t defaultVal = 0);
  void reassign(armaMat& oldVar, armaMat& newVar, data_t defaultVal = 0);
  void reassign(armaMat& oldVar, stdVec& newVar, data_t defaultVal = 0);
  void reassign(armaSubMat& oldVar, armaMat& newVar, data_t defaultVal = 0);
  void reassign(armaSubMat& oldVar, stdVec& newVar, data_t defaultVal = 0);

  void reset();
  void defaultQ();
  void printSys();

  void limit(stdVec& x, data_t& lb, data_t& ub);
  void limit(armaVec& x, data_t& lb, data_t& ub);
  void limit(armaMat& x, data_t& lb, data_t& ub);
  bool limitReset(armaVec& x, data_t& lb, data_t& ub);

  void checkP();

  bool adaptM;

 protected:
  void predict();

  armaVec u;    // input
  armaVec x;    // state
  armaMat P;    // covar of state
  armaVec m;    // process disturbance
  armaMat P_m;  // cov

  // Parameters:
  armaVec x0;
  armaMat P0;
  armaVec m0;
  armaMat P0_m;

  armaMat A;
  armaMat B;
  armaVec g;  // input gain.
  armaMat Q;
  armaMat Q_m;

  // it should be safe for dt to be a reference. I should not need to control
  // what the set behavior is.
  data_t& dt;
  data_t& q0;
  data_t& p0;

  // dimensions
  std::size_t nX;
  std::size_t nU;
  bool szChanged;

  // max val for elements of P before reset for numerical reasons...
  const data_t plim = 1e2;
};  // sys_t
}  // namespace lds

#endif
