#ifndef LDS_HPP
#define LDS_HPP

// LINEAR DYNAMICAL SYSTEM
// (i.e., no observations)
namespace lds {
	// in future, if need different precision or integer math, change definition of data_t?
	typedef double data_t;
	typedef std::vector<data_t> stdVec;
	typedef arma::Col<data_t> armaVec;
	typedef arma::Mat<data_t> armaMat;
	typedef arma::Cube<data_t> armaCube;
	typedef arma::subview_col<data_t> armaSubVec;
	typedef arma::subview<data_t> armaSubMat;

	namespace fill = arma::fill;

	// AUGMENTATION BIT MASKS
	static const std::size_t AUGMENT_G = 0x1;
	static const std::size_t AUGMENT_M = AUGMENT_G<<1;
	static const std::size_t AUGMENT_U = AUGMENT_G<<2;//for cases where control was designed to penalize deltaU, the state is technically augmented with u
	static const std::size_t AUGMENT_INTY = AUGMENT_G<<3;//for cases where there will be integral action for control
	static const std::size_t AUGMENT_DY = AUGMENT_G<<4;//for cases where there will be derivative action for control
	// static const data_t inf = std::numeric_limits<data_t>::infinity();//arma::datum::inf;
	static data_t inf = std::numeric_limits<data_t>::infinity();//arma::datum::inf;
	static data_t neginf = -inf;//arma::datum::inf;
	static const data_t pi = arma::datum::pi;

	// Weights for SSID.
	enum ssidWt { NONE, MOESP, CVA };
} //end lds

#endif
