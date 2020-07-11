#ifndef LDS_FIT_HPP
#define LDS_FIT_HPP

namespace lds {

	class fit_t {
	public:
		fit_t() {};
		fit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q, armaVec& x0, armaMat& P0, data_t& dt);

		// Dynamics
		armaMat A; // state transition matrix
		armaMat B; // input matrix
		armaVec g; // input gain
		armaVec m; // process noise mean
		armaMat Q; // process noise cov
		armaVec x0; // initial state
		armaMat P0; // initial covar

		// sample period
		data_t dt;
	};
	
}//lds
#endif
