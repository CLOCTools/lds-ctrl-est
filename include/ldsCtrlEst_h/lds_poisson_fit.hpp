#ifndef LDS_POISSON_FIT_HPP
#define LDS_POISSON_FIT_HPP

namespace lds {
	namespace poisson {
		class fit_t : public lds::fit_t {
		public:
			fit_t() { };
			fit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q, armaVec& x0, armaMat& P0, armaMat& C, armaVec& d, data_t dt, std::vector<data_t>& t0, std::vector<armaMat>& uTrain, std::vector<armaMat>& zTrain);

			// Output
			armaMat C; // output matrix
			armaVec d; // output bias

			// debating whether I should put this here.
			// input/output training data
			std::vector<data_t> t0;
			std::vector<armaMat> uTrain;//inputs
			std::vector<armaMat> zTrain;//measurements
		};

		class ssidFit_t : public fit_t {
		public:
			ssidFit_t() : fit_t() {};
			ssidFit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q, armaVec& x0, armaMat& P0, armaMat& C, armaVec& d, data_t dt, data_t t_startSSID, data_t t_stopSSID, armaVec& singVals, std::vector<data_t>& t0, std::vector<armaMat>& uTrain, std::vector<armaMat>& zTrain);
			data_t t_startSSID;
			data_t t_stopSSID;
			armaVec singVals;
		};
	}; //namespace plds
}//namspace lds

#endif
