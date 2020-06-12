#ifndef LDS_SYS_HPP
#define LDS_SYS_HPP

namespace lds {

	// define linear dynamical system (lds) class...
	class sys_t
	{
	public:
		// constructor
		sys_t(std::size_t nU, std::size_t nX, data_t& dt, data_t& p0, data_t& q0);
		sys_t& operator=(const sys_t& sys);

		// Augmentation stuff...
		void augment(std::size_t augmentation);
		void deaugment();

		bool checkIfAugmented(); //reports whether augmentation.
		bool checkIfAugmented(std::size_t augmentationType); //reports whether augmentation.

		// make one-step prediction
		void predict();
		void simPredict();

		// get methods
		std::size_t getAugmentation() const {return augmentation;};
		armaVec getU() const {return u;};
		armaVec getX() {return x.subvec(0,nX-1);};
		armaVec getG() const;
		armaVec getM() const;
		armaMat getA() const {return A.submat(0,0,nX-1,nX-1);};
		armaMat getB() const {return B.submat(0,0,nX-1,nU-1);};
		armaMat getQ() const {return Q;};//.armaSubMat(0,0,nX-1,nX-1);};
		armaMat getP() const {return P;};
		armaVec getX0() const {return x0.subvec(0,nX-1);};
		armaVec getP0() const {return P0.submat(0,0,nX-1,nX-1);};

		// set methods
		void setDims(std::size_t& nU, std::size_t& nX);
		void setU(stdVec& uVec);
		void setU(armaVec& u);
		void setA(stdVec& aVec);
		void setA(armaMat& A);
		void setB(stdVec& bVec);
		void setB(armaMat& B);
		void setM(stdVec& mVec);
		void setM(armaVec& m);
		void setQ(stdVec& qVec);
		void setQ(armaMat& Q);
		void setX0(stdVec& x0Vec);
		void setX0(armaVec& x0);
		void setP0(stdVec& p0Vec);
		void setP0(armaMat& P0);
		void setG(stdVec& gVec);
		void setG(armaVec& g);

		// these are really redundant. Should be able to use templates in some way to make this less type-specific
		// I think I should only need to two definitions.
		void reassign(armaVec& oldVar, armaVec& newVar, data_t defaultVal=0);
		void reassign(armaVec& oldVar, stdVec& newVar, data_t defaultVal=0);
		void reassign(armaSubVec& oldVar, armaVec& newVar, data_t defaultVal=0);
		void reassign(armaSubVec& oldVar, stdVec& newVar, data_t defaultVal=0);
		void reassign(armaMat& oldVar, armaMat& newVar, data_t defaultVal=0);
		void reassign(armaMat& oldVar, stdVec& newVar, data_t defaultVal=0);
		void reassign(armaSubMat& oldVar, armaMat& newVar, data_t defaultVal=0);
		void reassign(armaSubMat& oldVar, stdVec& newVar, data_t defaultVal=0);

		void reset();
		void defaultQ();
		void printSys();

		void limit(stdVec& x, data_t& lb, data_t& ub);
		void limit(armaVec& x, data_t& lb, data_t& ub);
		void limit(armaMat& x, data_t& lb, data_t& ub);
		bool limitReset(armaVec& x, data_t& lb, data_t& ub);

		void checkP();

	protected:
		armaVec u; //input
		armaVec x; //state
		armaMat P; //covar of state

		// Parameters:
		armaVec x0;
		armaMat P0;

		// If augmentation, make sure functions don't call g & m.
		// Really wanted to have a changeable reference to a piece of x, but it's not changeable after initialization.
		armaVec m;
		armaVec g;

		armaMat A;
		armaMat B;
		armaMat Q;

		//it should be safe for dt to be a reference. I should not need to control what the set behavior is.
		data_t& dt;
		data_t& q0;
		data_t& p0;

		std::size_t nX;
		std::size_t nU;
		std::size_t nXaug;

		bool szChanged;
		std::size_t augmentation; //making this potected to ensure that you can't change augmentation without actually augmenting the vars..
		armaMat diag_u;

		// max val for elements of P before reset for numerical reasons...
		const data_t plim = 1e2;
	}; // sys_t
} //end lds

#endif
