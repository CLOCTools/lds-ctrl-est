#ifndef LDS_POISSON_DATAIO_HPP
#define LDS_POISSON_DATAIO_HPP

namespace lds {
	namespace poisson {
		std::tuple<std::vector<armaMat>, std::vector<armaMat>, data_t, stdVec> loadData_mat(std::string filename);

		std::tuple<std::vector<armaMat>, std::vector<armaMat>, data_t, stdVec> getDefaultData(std::vector<armaMat>& z, std::vector<armaMat>& u, data_t& dt, stdVec& t0);
	}//namespace poisson
}//namspace lds

#endif
