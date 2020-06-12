#ifndef LDS_GAUSSIAN_HPP
#define LDS_GAUSSIAN_HPP

namespace lds {
	namespace gaussian {
		typedef lds::data_t data_t;
		typedef lds::stdVec stdVec;
		typedef lds::armaVec armaVec;
		typedef lds::armaMat armaMat;
		typedef lds::armaSubVec armaSubVec;
		typedef lds::armaSubMat armaSubMat;

		namespace fill = arma::fill;

		// AUGMENTATION BIT MASKS
		static const std::size_t AUGMENT_G = lds::AUGMENT_G;
		static const std::size_t AUGMENT_M = lds::AUGMENT_M;
		static const std::size_t AUGMENT_U = lds::AUGMENT_U;
		static const std::size_t AUGMENT_INTY = lds::AUGMENT_INTY;
		static const std::size_t AUGMENT_DY = lds::AUGMENT_DY;

		// Ideally these should be const, but trying to pass reference which means its val implicitly must be allowed to change.
		// hopefully no one will try to reassign their vals...
		static data_t DATA_T_ZERO = (data_t) 0;
		static data_t DATA_T_ONE = (data_t) 1;
		static data_t DEFAULT_SOFTSTART = (data_t) 0.1;//100 ms
		static bool FALSE = false;
		static bool TRUE = true;

		// static const data_t inf = lds::inf;
		static const data_t pi = lds::pi;
		static data_t inf = lds::inf;
		static data_t neginf = lds::neginf;
	}//gaussian
} //lds

#endif
