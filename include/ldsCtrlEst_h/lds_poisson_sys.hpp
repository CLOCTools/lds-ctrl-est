#ifndef LDS_POISSON_SYS_HPP
#define LDS_POISSON_SYS_HPP

namespace lds {
	namespace poisson {
		// define plds (poisson LDS) system class...
		class sys_t : public lds::sys_t
		{
		public:
			void h();//output nonlinearity
			void update(armaVec& z); //update estimate given measurement
			void simMeasurement(armaVec& z);

			sys_t(std::size_t nU, std::size_t nX, std::size_t nY, data_t& dt, data_t& p0, data_t& q0, std::size_t augmentation=0);
			sys_t& operator=(const sys_t& sys);

			// get methods
			armaMat getC() const {return C.submat(0,0,nY-1,nX-1);};
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

			void augment(std::size_t augmentation);

			void reset();
			void printSys();

		protected:
			// output-specific stuff
			std::size_t nY;

			armaMat C;
			armaVec d;
			armaVec y;
			armaVec logy;
			armaVec z;

			armaMat diag_y;

			std::string nlType; //making this protected b/c currently only should use exponential nonlinearity.
			armaVec chance; //for rolling dice if producing spikes...
		}; // sys_t
	} //poisson
} //lds

#endif
