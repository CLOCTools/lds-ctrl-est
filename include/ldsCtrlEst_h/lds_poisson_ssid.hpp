#ifndef LDS_POISSON_SSID_HPP
#define LDS_POISSON_SSID_HPP

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

namespace lds {
namespace poisson {
/**
Fit PLDS model by subspace identification (SSID). System matrices (A,B,C) are
fit by SSID assuming gaussian observations. Iteratively refits output function
parameters (C, d) by Poisson MLE iteratively using Newton's method.

SSID step is carried out following Katayama (2005), *Subspace Methods for System
Identification*, Ch. 10 "Subspace Identification (2): CCA". Springer.

fit_t ssidFit(std::vector<armaMat>& u, std::vector<armaMat>& n,
std::vector<armaMat>& t, data_t dt, size_t nX, size_t nH=25, data_t
t_startSSID=-inf, data_t t_stopSSID=inf);

@param u: Input
@param n: Measurements (in *counts*)
@param t0: Time of initial data sample
@param dt: Sample period
@param nX: Number of states (i.e., model order)
@param nH: Hannkel matrix size
@param t_startSSID: Start time for SSID
@param t_stopSSID: Start time for SSID
@param assumeGaussian: Whether to assume the input -> output relationship is
linear for SSID

@return tuple<ssidFit_t,lds::gaussian::ssidFit_t>
*/
std::tuple<ssidFit_t, lds::gaussian::ssidFit_t> ssidFit(
    std::vector<armaMat>& u, std::vector<armaMat>& n, data_t dt, size_t nX,
    size_t nH = 25, armaVec d0 = armaVec(1).fill(-inf), ssidWt wt = NONE,
    data_t wtG0 = 0.0, std::vector<data_t>& t0 = DEFAULT_T0,
    data_t t_startSSID = -std::numeric_limits<data_t>::infinity(),
    data_t t_stopSSID = std::numeric_limits<data_t>::infinity(),
    bool assumeGaussian = true);

void newtonSolveC_mle(armaMat& C, armaVec& d, std::vector<armaMat>& x,
                      std::vector<armaMat>& n, data_t& dt);
data_t newtonSolve_rescaleC_mle(armaMat& C, armaVec& d, std::vector<armaMat>& x,
                                std::vector<armaMat>& n, data_t& dt);

void calcD_mle(armaMat& C, armaVec& d, std::vector<armaMat>& x,
               std::vector<armaMat>& n, data_t& dt);

data_t calcD_newtonSolve_rescaleC_mle(armaMat& C, armaVec& d,
                                      std::vector<armaMat>& x,
                                      std::vector<armaMat>& n, data_t& dt);

void fitOutput_mle(armaMat& C, armaVec& d, std::vector<armaMat>& x,
                   std::vector<armaMat>& n, data_t& dt);
}  // namespace poisson
}  // namespace lds
#endif
