#ifndef LDS_GAUSSIAN_SWITCHED_CTRL_HPP
#define LDS_GAUSSIAN_SWITCHED_CTRL_HPP

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace gaussian {
// FB controller functionality
class sctrl_t : public ctrl_t {
 public:
  sctrl_t(std::size_t nSys, std::size_t nU, std::size_t nX, std::size_t nY,
          data_t& uLB, data_t& uUB, data_t& dt, data_t& p0 = DEFAULT_P0,
          data_t& q0 = DEFAULT_Q0, data_t& r0 = DEFAULT_R0,
          size_t controlType = 0);

  // set system using already-defined ones
  // make sure there is a check in place for dimensions.
  void setSystem(size_t sysIdx, ctrl_t& sys);

  // Will reassign the parameters at switch.
  // This is wasteful but simple, so will try it first.
  // Would be faster to toggle between ctrl_t's and replace wrapper u, x, P, m,
  // P_m however, I would have to override most functionality.
  void switchSystem(size_t sysIdx);

  size_t getSysIdx() const { return sysIdx; };

 protected:
  size_t nSys;
  std::vector<ctrl_t> systems;
  size_t sysIdx;  // current system index.

 private:
  // make all parameter set* methods private and define them to do nothing.
  // force users to use setSystem, with pre-defined systems.
  // from lds_t
  void setDims(std::size_t& nU, std::size_t& nX){};
  void setU(stdVec& uVec){};
  void setU(armaVec& u){};
  void setA(stdVec& aVec){};
  void setA(armaMat& A){};
  void setB(stdVec& bVec){};
  void setB(armaMat& B){};
  void setM(stdVec& mVec){};
  void setM(armaVec& m){};
  // void setG(stdVec& gVec) {};
  // void setG(armaVec& g) {};
  void setQ(stdVec& qVec){};
  void setQ(armaMat& Q){};
  void setQ_m(stdVec& qmVec){};
  void setQ_m(armaMat& Q_m){};
  void setX0(stdVec& x0Vec){};
  void setX0(armaVec& x0){};
  void setP0(stdVec& p0Vec){};
  void setP0(armaMat& P0){};
  void setP0_m(stdVec& p0mVec){};
  void setP0_m(armaMat& P0_m){};

  // from sys_t
  void setC(stdVec& cVec){};
  void setC(armaMat& C){};
  void setD(stdVec& dVec){};
  void setD(armaVec& d){};
  void setR(stdVec& rVec){};
  void setR(armaMat& R){};
  void setKe(stdVec& keVec){};
  void setKe(armaMat& Ke){};
  void setKe_m(stdVec& kemVec){};
  void setKe_m(armaMat& Ke_m){};

  // from ctrl_t
  // void setGDesign(stdVec& gVec) {};
  // void setGDesign(armaVec& g) {};
  void setKc_x(stdVec& Kc_xVec){};
  void setKc_x(armaVec& Kc_x){};
  void setKc_u(stdVec& Kc_uVec){};
  void setKc_u(armaVec& Kc_u){};
  void setKc_inty(stdVec& Kc_intyVec){};
  void setKc_inty(armaVec& Kc_inty){};
  void setControlType(size_t controlType);  // {};

  // // assuming I *want* to maintain same references
  // void setURef(stdVec& uRefVec) {};
  // void setURef(armaVec& uRef) {};
  // void setXRef(stdVec& xRefVec) {};
  // void setXRef(armaVec& xRef) {};
  // void setYRef(stdVec& yRefVec) {};
  // void setYRef(armaVec& yRef) {};
};
}  // namespace gaussian
}  // namespace lds

#endif
