#ifndef LDS_POISSON_CTRL_HPP
#define LDS_POISSON_CTRL_HPP

namespace lds {
	namespace poisson {
		// FB controller functionality
		class ctrl_t : public sys_t {
		public:
			ctrl_t(std::size_t nU, std::size_t nX, std::size_t nY, data_t& uLB, data_t& uUB, data_t& dt, data_t& p0, data_t& q0, std::size_t augmentation=0);
			ctrl_t& operator=(const ctrl_t& sys);

			// These are the workhorse functions
			// feedback control (update estimate --> update control --> predict)
			void piCtrl(armaVec& z, bool& gateCtrl=TRUE, bool& gateLock=FALSE, data_t& sigma_softStart=DEFAULT_SOFTSTART, data_t& sigma_uNoise=DATA_T_ZERO, bool& resetAtCtrlOnset=TRUE);

			// most generic. Given user-supplied [xRef, uRef, yRef] --> control
			void fbCtrl(armaVec& z, bool& gateCtrl=TRUE, bool& gateLock=FALSE, data_t& sigma_softStart=DEFAULT_SOFTSTART, data_t& sigma_uNoise=DATA_T_ZERO, bool& resetAtCtrlOnset=TRUE);

			// log-linear control
			void logLin_fbCtrl(armaVec& z, bool& gateCtrl=TRUE, bool& gateLock=FALSE, data_t& sigma_softStart=DEFAULT_SOFTSTART, data_t& sigma_uNoise=DATA_T_ZERO, bool& resetAtCtrlOnset=TRUE);

			// Steady-state solution to log-linear control problem. Given user-supplied yRef --> steady state sol'z [xRef, uRef] --> control
			void steadyState_logLin_fbCtrl(armaVec& z, bool& gateCtrl=TRUE, bool& gateEst=TRUE, bool& gateLock=FALSE, data_t& sigma_softStart=DEFAULT_SOFTSTART, data_t& sigma_uNoise=DATA_T_ZERO, bool& resetAtCtrlOnset=TRUE);

			// calculate steady-state setpoint
			void calc_ssSetPt();

			void augment(std::size_t augmentation = AUGMENT_INTY, bool gateCtrl = false);
			void deaugment();

			void printSys();

			// set methods
			// make sure I am overriding setU behavior inherited from sys_t!
			void setU(stdVec& uVec);
			void setU(armaVec& u);

			void setG(stdVec& gVec);
			void setG(armaVec& g);

			void setGDesign(stdVec& gVec);
			void setGDesign(armaVec& g);

			void setURef(stdVec& uRefVec);
			void setURef(armaVec& uRef);

			void setXRef(stdVec& xRefVec);
			void setXRef(armaVec& xRef);

			void setYRef(stdVec& yRefVec);
			void setYRef(armaVec& yRef);

			void setKc_x(stdVec& Kc_xVec);
			void setKc_x(armaVec& Kc_x);

			void setKc_u(stdVec& Kc_uVec);
			void setKc_u(armaVec& Kc_u);

			void setKc_y(stdVec& Kc_yVec);
			void setKc_y(armaVec& Kc_y);

			void setKc_inty(stdVec& Kc_intyVec);
			void setKc_inty(armaVec& Kc_inty);

			void setKc_dy(stdVec& Kc_dyVec);
			void setKc_dy(armaVec& Kc_dy);

			void setTauAntiWindup(data_t& tau);

			armaVec getURef() const {return uRef;};
			armaVec getLogYRef() const {return logyRef;};
			armaVec getLogY() const {return logy;};
			armaVec getIntE() const {return intE;};

			void checkGainInversion();
			void reset();

			void update_expFilt();
			data_t tauEst;//time constant of 1st order filter estimator

			void antiWindup();

		protected:
			armaVec gDesign;//the input gain of the system used for controller design

			//  reference signals
			armaVec uRef;
			armaVec uRef_prev; //create no set method for this!
			armaVec xRef;
			armaVec logyRef; //log(yRef)
			armaVec yRef; //reference output

			// Controller gains
			armaMat Kc_x; //on instantaneous state error
			armaMat Kc_u; //if designed control around min deltaU
			// armaMat Km; //adaptive control
			armaMat Kc_y; //on instantaneous output error
			armaMat Kc_inty; //on integrated output error
			armaMat Kc_dy; //on deriv output error

			// control after g inversion (partial fb linearization)
			// do not need set methods for these.
			armaVec duRef;
			armaVec dvRef;
			armaVec vRef;
			armaVec dv;
			armaVec v;

			// integral error
			// do not need set method for this
			armaVec intE;
			armaVec intE_awuAdjust;//anti-windup adjustment to intE
			armaVec uSat; // for antiWindup

			bool gateCtrl_prev;
			bool gateLock_prev;

			//whether the g of system has become inverted from what you think it is (gainRef)
			bool gInverted;
			bool uSaturated;

			// I think it should be safe to have references here bc there is nothing I need to do (like reset vars) when it changes...
			data_t& uLB;
			data_t& uUB;

			// for exponential smoothing...
			data_t aEst;
			data_t bEst;

			// antiwindup backcalculation timeconstant
			data_t tauAntiWindup;
			data_t kAntiWindup;

			// for soft start
			data_t t_since_ctrl_onset;

			data_t yRefLB;

		private:
			void calc_logLinCtrl(bool& gateCtrl, bool& gateLock, data_t& sigma_softStart, data_t& sigma_uNoise, bool& resetAtCtrlOnset);
		};
	}//end poisosn
} //end lds

#endif
