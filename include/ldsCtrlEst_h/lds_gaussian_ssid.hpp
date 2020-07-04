#ifndef LDS_GAUSSIAN_SSID_HPP
#define LDS_GAUSSIAN_SSID_HPP

namespace lds {
	namespace gaussian {

		class ssidFit_t : public fit_t {
		public:
			ssidFit_t() : fit_t() {};
			ssidFit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q, armaVec& x0, armaMat& P0, armaMat& C, armaMat& D, armaVec& d, armaMat& R, data_t dt, data_t t_startSSID, data_t t_stopSSID, armaVec& singVals, std::vector<data_t>& t0, std::vector<armaMat>& uTrain, std::vector<armaMat>& zTrain);
			std::vector<data_t> t0;
			data_t t_startSSID;
			data_t t_stopSSID;
			armaVec singVals;
		};

		/**
		Fit LDS model with Gaussian observations by subspace identification (SSID)

		SSID step is carried out following van Overschee (1996). Either N4SID or "Robust" algorithm.

		fit_t ssidFit(std::vector<armaMat>& u, std::vector<armaMat>& n, std::vector<armaMat>& t, data_t dt, size_t nX, size_t nH=10, data_t t_startSSID=-inf, data_t t_stopSSID=inf);

		@param u: Input
		@param z: Measurements
		@param t0: Time of initial data sample
		@param dt: Sample period
		@param nX: Number of states (i.e., model order)
		@param nH: Hannkel matrix size
		@param t_startSSID: Start time for SSID
		@param t_stopSSID: Start time for SSID

		@return ssidFit_t,
		*/
		ssidFit_t ssidFit(std::vector<armaMat>& u, std::vector<armaMat>& z, data_t dt, size_t nX, size_t nH=25, armaVec d0=armaVec(1).fill(-inf), bool force_unitNormC=false, ssidWt wt=NONE, data_t wtG0=0.0, std::vector<data_t>& t0=DEFAULT_T0, data_t t_startSSID=-std::numeric_limits<data_t>::infinity(), data_t t_stopSSID=std::numeric_limits<data_t>::infinity());

		armaVec calcD_silence(std::vector<armaMat>& n, std::vector<armaMat>& u, data_t dt, data_t tSilence=0.1, data_t threshSilence=0.001);

		void createHankelMat_n4sid(armaMat& u, armaMat& z, std::size_t& nH, armaMat& D);
		void ssid_n4sid(std::size_t& nX, std::size_t& nU, std::size_t& nY, std::size_t& nH, armaMat& uSSID, armaMat& zSSID, armaMat& A, armaMat& B, armaMat& C, armaMat& D, armaMat& Q, armaMat& R, armaVec& m, armaVec& d, armaVec& s, bool force_unitNormC, ssidWt wt, data_t wtG0);
		void ssid_n4sid_vanOverschee(std::size_t& nX, std::size_t& nU, std::size_t& nY, std::size_t& nH, armaMat& uSSID, armaMat& zSSID, armaMat& A, armaMat& B, armaMat& C, armaMat& D, armaMat& Q, armaMat& R, armaVec& s, ssidWt wt);
		void recompute_extObs(armaMat& extObs, armaMat& A, armaMat& C, std::size_t& nY, std::size_t& nH);

		void lq(armaMat& L, armaMat& Qt, armaMat& X);
		armaMat calcCov(armaMat& A, armaMat& B);

		/**
		Approximate sqrtmat by singular value decomposition.

		svd(A) --> U * sqrt(s) * V' ~= sqrtmat(A)

		std::tuple<armaMat& ,armaVec& ,armaMat& > sqrtmat_svd(armaMat& A);

		@param X: armaMat
		@return sqrtX: armaMat
		*/
		armaMat sqrtmat_svd(armaMat& X);
	} //gaussian
} //lds
#endif
