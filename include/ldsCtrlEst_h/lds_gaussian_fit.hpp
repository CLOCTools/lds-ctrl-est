#ifndef LDS_GAUSSIAN_FIT_HPP
#define LDS_GAUSSIAN_FIT_HPP

namespace lds {
	namespace gaussian {

		class fit_t : public lds::fit_t {
		public:
			fit_t() {};
			fit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q, armaVec& x0, armaMat& P0, armaMat& C, armaMat& D, armaVec& d, armaMat& R, data_t dt, std::vector<armaMat>& uTrain, std::vector<armaMat>& zTrain);
			fit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q, armaVec& x0, armaMat& P0, armaMat& C, armaVec& d, armaMat& R, data_t dt, std::vector<armaMat>& uTrain, std::vector<armaMat>& zTrain);


			// Output
			armaMat C; // output matrix
			armaMat D; // feedthrough matrix
			armaVec d; // output bias
			armaMat R; // output noise cov

			// input/output training data
			std::vector<armaMat> uTrain;
			std::vector<armaMat> zTrain;
		};
		
	}; //gaussian
}//lds

#endif
