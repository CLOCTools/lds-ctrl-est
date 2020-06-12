#ifndef LDS_POISSON_SOLVESIGMA_HPP
#define LDS_POISSON_SOLVESIGMA_HPP

namespace lds {
	namespace poisson {
		// given the difference between the mean and the up/lower limit (mu)
		// solve for the standard dev of noise at the probability level of interest.
		void solveSigma(data_t& sigma, data_t& mu, data_t& p, data_t& alpha);
		data_t solveSigma_f(data_t& mu, data_t& sigma, data_t& p);
		data_t solveSigma_fprime(data_t& mu, data_t& sigma, data_t& f, data_t& p);
	}
}

#endif
