#ifndef DYNCTRLEST_MEX_HPP
#define DYNCTRLEST_MEX_HPP

#include <ldsCtrlEst>
#include "mex.hpp"
#include "mexAdapter.hpp"

template<class T>
std::vector<arma::Mat<T>> matlabCell2vectorArmaMat(matlab::data::CellArray& matlabCell)
{
	size_t nCells = matlabCell.getNumberOfElements();
	std::vector<arma::Mat<T>> armaMat(nCells, arma::mat(1,1,arma::fill::zeros));
	for (size_t k=0; k<nCells; k++)
	{
		matlab::data::TypedArray<T> matlabMat = matlabCell[k];
		auto dims = matlabMat.getDimensions();
		armaMat[k] = arma::Mat<T>(matlabMat.release().get(), dims[0], dims[1]);
	}
	return armaMat;
};

template<class T>
std::vector<T> matlabVector2vector(matlab::data::TypedArray<T>& matlabArray)
{
	size_t nElem = matlabArray.getNumberOfElements();
	double* ptr=matlabArray.release().get();
	std::vector<T> vec(ptr, ptr+nElem);
	return vec;
};

template<class T>
arma::Col<T> matlabVector2armaVector(matlab::data::TypedArray<T> matlabArray)
{
	size_t nElem = matlabArray.getNumberOfElements();
	T* ptr=matlabArray.release().get();
	arma::Col<T> vec(ptr, nElem);//, false);
	return vec;
};

template<class T>
arma::Mat<T> matlabMat2armaMat(matlab::data::TypedArray<T> matlabArray)
{
	// ArrayDimensions == std::vector<size_t>
	matlab::data::ArrayDimensions dims = matlabArray.getDimensions();
	T* ptr=matlabArray.release().get();

	// n.b., I am pretty sure matlab makes vectors have 2dims still,
	// so going to assume it's safe to look at 0, 1...
	// mat(ptr_aux_mem, n_rows, n_cols, copy_aux_mem = true, strict = false)
	arma::Mat<T> mat(ptr, dims[0], dims[1]);//, false);
	return mat;
};

template<class T>
matlab::data::TypedArray<T> armaMat2matlabMat(arma::Mat<T>& armaMat, matlab::data::ArrayFactory& factory)
{
	const matlab::data::TypedArray<T> matlabMat = factory.createArray<T>({armaMat.n_rows, armaMat.n_cols}, armaMat.memptr(), armaMat.memptr()+armaMat.n_elem);
	return matlabMat;
};

template<class T>
matlab::data::TypedArray<T> armaVec2matlabMat(arma::Col<T>& armaVec, matlab::data::ArrayFactory& factory)
{
	const matlab::data::TypedArray<T> matlabMat = factory.createArray<T>({armaVec.n_elem,1}, armaVec.memptr(), armaVec.memptr()+armaVec.n_elem);
	return matlabMat;
};

template<class T>
matlab::data::TypedArray<T> stdVec2matlabMat(std::vector<T>& stdVec, matlab::data::ArrayFactory& factory)
{
	const matlab::data::TypedArray<T> matlabMat = factory.createArray<T>({stdVec.size(),1}, stdVec.data(), stdVec.data()+stdVec.size());
	return matlabMat;
};


#endif
