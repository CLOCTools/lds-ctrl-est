#ifndef LDS_GAUSSIAN_EM_HPP
#define LDS_GAUSSIAN_EM_HPP

// in case LDS_GAUSSIAN_SSID_HPP imported after this...
#include "lds_gaussian_ssid.hpp"

namespace lds {
	namespace gaussian {

		class emFit_t : public fit_t {
		public:
			emFit_t() : fit_t() {};
			emFit_t(armaMat& A, armaMat& B, armaVec& g, armaVec& m, armaMat& Q, armaVec& x0, armaMat& P0, armaMat& C, armaVec& d, armaMat& R, data_t dt, std::vector<armaMat>& uTrain, std::vector<armaMat>& zTrain);
			emFit_t(data_t dt, std::vector<armaMat>& uTrain, std::vector<armaMat>& zTrain);
			emFit_t(ssidFit_t& fit0);

			void reset(void);

			void runEM(bool calcAB=true, bool calcQ=true, bool calcInitial=true, bool calcC=true, bool calcd=true, bool calcR=true);

			data_t tol = 1e-3;
			size_t maxIter = 100;

			// get methods
			std::vector<armaMat> getX() {return xHat;};
			std::vector<armaMat> getY() {return yHat;};
			armaMat get_sum_E_xu_tm1_xu_tm1() {return sum_E_xu_tm1_xu_tm1;};
			armaMat get_sum_E_xu_t_xu_tm1() {return sum_E_xu_t_xu_tm1;};
			armaMat get_sum_E_xu_t_xu_t() {return sum_E_xu_t_xu_t;};
			size_t getT() {return T;};

			void setY(std::vector<armaMat> yHat) {this->yHat=yHat;};

			// TODO: EM could be good way to solve for Qaug in data-driven way.
			// // Augmentation stuff...
			// void augment(std::size_t augmentation);
			// void deaugment();
			// std::size_t getAugmentation() const {return augmentation;};
		protected:
			armaVec getTheta();
			void kalmanFilt();
			void kalmanSmooth(bool forceCommonInitial); //estimate x_t|T by kalman filtering --> backwards filtering.
			void Estep(bool forceCommonInitial=false);//force all trials to have common initial conditions?
			void Mstep(bool calcAB=true, bool calcQ=true, bool calcInitial=false, bool calcC=false, bool calcd=false, bool calcR=true);

			std::vector<armaMat> xHat;
			std::vector<arma::Cube<data_t>> P;
			std::vector<armaMat> yHat;
			std::vector<arma::Cube<data_t>> P_t_tm1;

			armaMat sum_E_xu_t_xu_t;
			armaMat sum_E_xu_tm1_xu_tm1;
			armaMat sum_E_xu_t_xu_tm1;
			size_t T;

			std::size_t augmentation = 0;
		};

		emFit_t emFit_x_equals_y(std::vector<armaMat>& uTrain, std::vector<armaMat>& zTrain, data_t dt, std::size_t maxIter=100, data_t tol=1e-3, data_t q0=1e-6, bool calcAB=true, bool calcQ=true, bool calcR=true);

		// std::tuple<emFit_t, armaVec> emFit_x_equals_y_qM(std::vector<armaMat>& uTrain, std::vector<armaMat>& zTrain,  std::vector<data_t>& t0, data_t dt, std::size_t maxIter=100, data_t tol=1e-3);
	}; //gaussian
}//lds

#endif
