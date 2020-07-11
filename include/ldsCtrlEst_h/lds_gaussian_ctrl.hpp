#ifndef LDS_GAUSSIAN_CTRL_HPP
#define LDS_GAUSSIAN_CTRL_HPP

namespace lds {
	namespace gaussian {
		// FB controller functionality
		class ctrl_t : public sys_t {
		public:
			ctrl_t(std::size_t nU, std::size_t nX, std::size_t nY, data_t& uLB, data_t& uUB, data_t& dt, data_t& p0=DEFAULT_P0, data_t& q0=DEFAULT_Q0, data_t& r0=DEFAULT_R0, size_t controlType=0);
			ctrl_t& operator=(const ctrl_t& sys);

			// These are the workhorse functions
			// most flexible, but user has to supply xRef, uRef
			void fbCtrl(armaVec& z, bool& gateCtrl=TRUE, bool& gateLock=FALSE, data_t& sigma_softStart=DEFAULT_SOFTSTART, data_t& sigma_uNoise=DATA_T_ZERO, bool& resetAtCtrlOnset=TRUE, bool& doRecurse_Ke=TRUE);

			// steady-state control: calculates steady state vals for xRef, uRef
			void steadyState_fbCtrl(armaVec& z, bool& gateCtrl=TRUE, bool& gateEst=TRUE, bool& gateLock=FALSE, data_t& sigma_softStart=DEFAULT_SOFTSTART, data_t& sigma_uNoise=DATA_T_ZERO, bool& resetAtCtrlOnset=TRUE, bool& doRecurse_Ke=TRUE);

			// calculate steady-state setpoint [xRef; uRef]
			void calc_ssSetPt();

			void printSys();
			void setDims(std::size_t& nU, std::size_t& nX, std::size_t& nY);

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

			void setKc_x(stdVec& Kc_x_vec);
			void setKc_x(armaVec& Kc_x);

			void setKc_u(stdVec& Kc_u_vec);
			void setKc_u(armaVec& Kc_u);

			void setKc_inty(stdVec& Kc_inty_vec);
			void setKc_inty(armaVec& Kc_inty);

			void setControlType(size_t controlType);
			void setTauAntiWindup(data_t& tau);

			armaVec getURef() const {return uRef;};
			armaVec getIntE() const {return intE;};

			void reset();
			void antiWindup();

		protected:
			armaVec gDesign;//the input gain of the system used for controller design

			//  reference signals
			armaVec uRef;
			armaVec uRef_prev; //create no set method for this!
			armaVec xRef; //reference state
			armaVec yRef; //reference output

			// Controller gains
			armaMat Kc_x; //on instantaneous state error
			armaMat Kc_u; //control around min deltaU
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

			//whether the g of system has become inverted from what you think it is (gainRef)
			bool uSaturated;

			// I think it should be safe to have references here bc there is nothing I need to do (like reset vars) when it changes...
			data_t& uLB;
			data_t& uUB;

			// antiwindup backcalculation timeconstant
			data_t tauAntiWindup;
			data_t kAntiWindup;

			data_t t_since_ctrl_onset;
			size_t controlType;

		private:
			void calc_ctrl(bool& gateCtrl=TRUE, bool& gateLock=FALSE, data_t& sigma_softStart=DEFAULT_SOFTSTART, data_t& sigma_uNoise=DATA_T_ZERO, bool& resetAtCtrlOnset=TRUE);
		};
	}//end gaussian
} //end lds

#endif
