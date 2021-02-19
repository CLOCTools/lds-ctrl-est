#ifndef LDS_GAUSSIAN_SYS_HPP
#define LDS_GAUSSIAN_SYS_HPP

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace gaussian {
// define glds (gaussian LDS) system class...
class sys_t : public lds::sys_t {
 public:
  void filter(
      armaVec& u_tm1, armaVec& z_t,
      bool doRecurse_Ke = true);  // predict+update estimate given measurement
  void filter(
      armaVec& z,
      bool doRecurse_Ke = true);  // predict+update estimate given measurement
  void recurse_Ke();              // recursively estimate Ke
  void simMeasurement(armaVec& z);

  sys_t(std::size_t nU, std::size_t nX, std::size_t nY, data_t& dt,
        data_t& p0 = DEFAULT_P0, data_t& q0 = DEFAULT_Q0,
        data_t& r0 = DEFAULT_R0);
  sys_t& operator=(const sys_t& sys);

  // get methods
  size_t getNy() const { return nY; };
  armaMat getC() const { return C; };
  armaVec getD() const { return d; };
  armaVec getR() const { return R; };
  armaVec getY() const { return y; };
  armaVec getZ() const { return z; };
  armaMat getKe() const { return Ke; };
  armaMat getKe_m() const { return Ke_m; };

  // set methods
  void setDims(std::size_t& nU, std::size_t& nX, std::size_t& nY);
  void setC(stdVec& cVec);
  void setC(armaMat& C);
  void setD(stdVec& dVec);
  void setD(armaVec& d);
  void setR(stdVec& rVec);
  void setR(armaMat& R);
  void setZ(stdVec& zVec);
  void setZ(armaVec& z);
  void setKe(stdVec& keVec);
  void setKe(armaMat& Ke);
  void setKe_m(stdVec& kemVec);
  void setKe_m(armaMat& Ke_m);

  void reset();
  void defaultR();
  void printSys();

 protected:
  void predict();
  void h();  // output

  // output-specific stuff
  std::size_t nY;
  armaMat C;
  armaVec d;
  armaMat R;
  data_t& r0;
  armaVec y;
  armaVec z;

  armaMat Ke;
  armaMat Ke_m;
};  // sys_t
}  // namespace gaussian
}  // namespace lds

#endif
