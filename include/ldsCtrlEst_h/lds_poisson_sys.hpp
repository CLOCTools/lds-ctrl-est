#ifndef LDS_POISSON_SYS_HPP
#define LDS_POISSON_SYS_HPP

namespace lds {
	namespace poisson {
		// define plds (poisson LDS) system class...
		class sys_t : public lds::sys_t
		{
		public:
			void filter(armaVec& u_tm1, armaVec& z_t);
			void filter(armaVec& z);
			void simMeasurement(armaVec& z);

			sys_t(std::size_t nU, std::size_t nX, std::size_t nY, data_t& dt, data_t& p0=DEFAULT_P0, data_t& q0=DEFAULT_Q0);
			sys_t& operator=(const sys_t& sys);

			// get methods
			size_t getNy() const {return nY;};
			armaMat getC() const {return C;};
			armaVec getD() const {return d;};
			armaVec getY() const {return y;};
			armaVec getZ() const {return z;};

			// set methods
			void setDims(std::size_t& nU, std::size_t& nX, std::size_t& nY);
			void setC(stdVec& cVec);
			void setC(armaMat& C);
			void setD(stdVec& dVec);
			void setD(armaVec& d);
			void setZ(stdVec& zVec);
			void setZ(armaVec& z);

			void reset();
			void printSys();

		protected:
			void predict();
			void h();//output nonlinearity

			// output-specific stuff
			std::size_t nY;
			armaMat C;
			armaVec d;
			armaVec y;
			armaVec logy;
			armaVec z;

			armaMat diag_y;
			armaVec chance; //for rolling dice if producing spikes...
		}; // sys_t
	} //poisson
} //lds

#endif
