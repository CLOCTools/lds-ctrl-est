#ifndef LDS_POISSON_HPP
#define LDS_POISSON_HPP

namespace lds {
	namespace poisson {
		typedef lds::data_t data_t;
		typedef lds::stdVec stdVec;
		typedef lds::armaVec armaVec;
		typedef lds::armaMat armaMat;
		typedef lds::armaSubVec armaSubVec;
		typedef lds::armaSubMat armaSubMat;

		namespace fill = arma::fill;

		// CONTROL BIT MASKS
		static const std::size_t CONTROL_TYPE_U = lds::CONTROL_TYPE_U;
		static const std::size_t CONTROL_TYPE_INTY = lds::CONTROL_TYPE_INTY;
		static const std::size_t CONTROL_TYPE_ADAPT_M = lds::CONTROL_TYPE_ADAPT_M;

		// Ideally these should be const, but trying to pass reference which means its val implicitly must be allowed to change.
		// hopefully no one will try to reassign their vals...
		static data_t DATA_T_ZERO = (data_t) 0;
		static data_t DATA_T_ONE = (data_t) 1;
		static data_t DEFAULT_SOFTSTART = (data_t) 0;
		static bool FALSE = false;
		static bool TRUE = true;

		// static const data_t inf = lds::inf;
		static const data_t pi = lds::pi;
		static data_t inf = lds::inf;
		static data_t neginf = lds::neginf;
	}//poisson
} //lds

#endif
