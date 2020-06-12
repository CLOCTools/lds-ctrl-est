#ifndef LDS_GAUSSIAN_SYS_HPP
#define LDS_GAUSSIAN_SYS_HPP

namespace lds {
	namespace gaussian {
		// define glds (gaussian LDS) system class...
		class sys_t : public lds::sys_t
		{
		public:
			void h();//output
			void update(armaVec& z, bool doRecurse_Ke=true); //update estimate given measurement
			void recurse_Ke(); //recursively estimate Ke
			void simMeasurement(armaVec& z);

			sys_t(std::size_t nU, std::size_t nX, std::size_t nY, data_t& dt, data_t& p0, data_t& q0, data_t& r0, std::size_t augmentation=0);
			sys_t& operator=(const sys_t& sys);

			// get methods
			armaMat getC() const {return C.submat(0,0,nY-1,nX-1);};
			armaVec getD() const {return d;};
			armaVec getY() const {return y;};
			armaVec getZ() const {return z;};
			armaVec getKe() const {return Ke;};

			// set methods
			void setDims(std::size_t& nU, std::size_t& nX, std::size_t& nY);
			void setC(stdVec& cVec);
			void setC(armaMat& C);
			void setD(stdVec& dVec);
			void setD(armaVec& d);
			void setR(stdVec& rVec);
			void setR(armaMat& R);
			void setZ(stdVec& zVec);
			void setZ(armaVec& z);
			void setKe(stdVec& keVec);
			void setKe(armaMat& Ke);

			void augment(std::size_t augmentation);

			void reset();
			void defaultR();
			void printSys();

		protected:
			// output-specific stuff
			std::size_t nY;

			armaMat C;
			armaVec d;
			armaMat R;
			data_t& r0;
			armaVec y;
			armaVec z;

			armaMat Ke;
		}; // sys_t
	} //gaussian
} //lds

#endif
