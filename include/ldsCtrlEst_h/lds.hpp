#ifndef LDS_HPP
#define LDS_HPP

#ifndef LDSCTRLEST
#include <ldsCtrlEst>
#endif

// LINEAR DYNAMICAL SYSTEM
// (i.e., no observations)
namespace lds {
// if need different precision, change definition of data_t
typedef double data_t;
typedef std::vector<data_t> stdVec;
typedef arma::Col<data_t> armaVec;
typedef arma::Mat<data_t> armaMat;
typedef arma::Cube<data_t> armaCube;
typedef arma::subview_col<data_t> armaSubVec;
typedef arma::subview<data_t> armaSubMat;

namespace fill = arma::fill;

// CONTROL BIT MASKS
static const std::size_t CONTROL_TYPE_U =
    0x1;  // for cases where control was designed to penalize deltaU, the state
          // is technically augmented with u
static const std::size_t CONTROL_TYPE_INTY =
    CONTROL_TYPE_U
    << 1;  // for cases where there will be integral action for control
static const std::size_t CONTROL_TYPE_ADAPT_M = CONTROL_TYPE_U
                                                << 2;  // adapt setpoint with m.

static data_t inf = std::numeric_limits<data_t>::infinity();
static data_t neginf = -inf;
static const data_t pi = arma::datum::pi;

// Filler vals for diag elements of elements in case
// there is need to resolve dimensionality mismatch:
static data_t DEFAULT_P0 = 1e-6;
static data_t DEFAULT_Q0 = 1e-6;
static data_t DEFAULT_R0 = 1e-2;

// Weights for SSID.
enum ssidWt { NONE, MOESP, CVA };
static std::vector<data_t> DEFAULT_T0 = std::vector<data_t>(1, 0.0);
}  // namespace lds

#endif
