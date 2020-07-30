#ifndef LDS_POISSON_CTRL_HPP
#define LDS_POISSON_CTRL_HPP

namespace lds {
	namespace poisson {
		// FB controller functionality
		class ctrl_t : public sys_t {
		public:
			ctrl_t(std::size_t nU, std::size_t nX, std::size_t nY, data_t& uLB, data_t& uUB, data_t& dt, data_t& p0=DEFAULT_P0, data_t& q0=DEFAULT_Q0, size_t controlType=0);
			ctrl_t& operator=(const ctrl_t& sys);

			// These are the workhorse functions
			// log-linear control
			void logLin_fbCtrl(armaVec& z, bool& gateCtrl=TRUE, bool& gateLock=FALSE, data_t& sigma_softStart=DEFAULT_SOFTSTART, data_t& sigma_uNoise=DATA_T_ZERO, bool& resetAtCtrlOnset=TRUE);

			// Steady-state solution to log-linear control problem. Given user-supplied yRef --> steady state sol'z [xRef, uRef] --> control
			void steadyState_logLin_fbCtrl(armaVec& z, bool& gateCtrl=TRUE, bool& gateEst=TRUE, bool& gateLock=FALSE, data_t& sigma_softStart=DEFAULT_SOFTSTART, data_t& sigma_uNoise=DATA_T_ZERO, bool& resetAtCtrlOnset=TRUE);

			// calculate steady-state setpoint
			void calc_ssSetPt();

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
			void setKc_x(armaMat& Kc_x);

			void setKc_u(stdVec& Kc_uVec);
			void setKc_u(armaMat& Kc_u);

			void setKc_inty(stdVec& Kc_intyVec);
			void setKc_inty(armaMat& Kc_inty);

			void setControlType(size_t controlType);
			void setTauAntiWindup(data_t& tau);

			armaMat getKc_u() const {return Kc_u;};
			armaMat getKc_x() const {return Kc_x;};
			armaMat getKc_inty() const {return Kc_inty;};
			armaVec getGDesign() const {return gDesign;};
			armaVec getURef() const {return uRef;};
			armaVec getXRef() const {return xRef;};
			armaVec getLogYRef() const {return logyRef;};
			armaVec getYRef() const {return yRef;};
			armaVec getLogY() const {return logy;};
			armaVec getIntE() const {return intE;};
			size_t getControlType() const {return controlType;};

			void reset();
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
			armaMat Kc_u; //control around min deltaU
			// armaMat Kc_y; //on instantaneous output error
			armaMat Kc_inty; //on integrated output error

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

			// I think it should be safe to have references here bc there is nothing I need to do (like reset vars) when it changes...
			bool uSaturated;
			data_t& uLB;
			data_t& uUB;

			// antiwindup backcalculation timeconstant
			data_t tauAntiWindup;
			data_t kAntiWindup;

			// for soft start
			data_t t_since_ctrl_onset;
			data_t yRefLB;

			size_t controlType;

		private:
			void calc_logLinCtrl(bool& gateCtrl, bool& gateEst, bool& gateLock, data_t& sigma_softStart, data_t& sigma_uNoise, bool& resetAtCtrlOnset);
		};
	}//end poisosn
} //end lds

#endif
