#include <ldsCtrlEst>

using namespace std;
using namespace plds;

void plds::solveSigma(data_t& sigma, data_t& mu, data_t& p, data_t& alpha) {

	data_t f;
	data_t fprime;

	size_t maxIter = 50;
	for (size_t k=0; k<maxIter+1; k++) {

		f = solveSigma_f(mu, sigma, p);
		fprime = solveSigma_fprime(mu, sigma, f, p);
		sigma += alpha*f/fprime;

		if (f < 0.01)
		break;
	}
}

plds::data_t plds::solveSigma_f(data_t& mu, data_t& sigma, data_t& p) {
	data_t f = -pow(mu,2)/pow(sigma,2)/2;
	f = exp(f);
	f = f / sqrt(2*pi*pow(sigma,2));
	f -= p;
	return f;
}

plds::data_t plds::solveSigma_fprime(data_t& mu, data_t& sigma, data_t& f, data_t& p) {

	data_t fprime = -2*pi*pow((2*pi*pow(sigma,2)),-3/2) * exp(-pow(mu,2)/pow(sigma,2)/2);
	fprime += (f+p)*pow(mu,2)/pow(sigma,3)/4;

	return fprime;
}
