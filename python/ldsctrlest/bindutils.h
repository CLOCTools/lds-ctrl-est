#include <ldsCtrlEst_h/lds_fit_em.h>
#include <ldsCtrlEst_h/lds_gaussian_fit_em.h>
#include <ldsCtrlEst_h/lds_poisson_fit_em.h>

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <carma>

#include <functional>
#include <iostream>
#include <string>
#include <typeinfo>

// this allows for the shorthand "my_arg"_a instead of py::arg("my_arg")
using namespace pybind11::literals;
using namespace std;
using namespace lds;

namespace bindutils {

string capture_output(const function<void(void)> &f) {
  ostringstream capture_out;
  streambuf *coutbuf = cout.rdbuf();  // save old buf
  cout.rdbuf(capture_out.rdbuf());    // redirect cout to capture_out

  f();
  string captured_output = capture_out.str();

  cout.rdbuf(coutbuf);  // reset to standard output
  return captured_output;
}

// The FitEM classes are children of abstract EM<Fit> (either Gaussian or
// Poisson) so this function defines the base EM<Fit> functionality to avoid
// repeating it.
template <typename FitType, typename FitEMType>
py::class_<FitEMType, lds::EM<FitType>> define_FitEM_base(py::module &m) {
  return py::class_<FitEMType, lds::EM<FitType>>(m, "FitEM")
      // constructors
      .def(py::init<>())
      .def(py::init<size_t, data_t, UniformMatrixList<kMatFreeDim2> &&,
                    UniformMatrixList<kMatFreeDim2> &&>())
      .def(py::init<const FitType &, UniformMatrixList<kMatFreeDim2> &&,
                    UniformMatrixList<kMatFreeDim2> &&>())

      // functions
      .def("Run", &FitEMType::Run, "calc_dynamics"_a = true, "calc_Q"_a = true,
           "calc_init"_a = true, "calc_output"_a = true,
           "calc_measurement"_a = true, "max_iter"_a = 100, "tol"_a = 1e-2);
}

void println(string message) {
  cerr << message << endl;
}

// again need to use templates, which I can't do in the PYBIND11_MODULE block
template <MatrixListFreeDim D>
py::class_<UniformMatrixList<D>, vector<Matrix>> define_UniformMatrixList(
    py::module& m, string py_class_suffix) {

  string py_class_name = string("UniformMatrixList") + py_class_suffix;
  return py::class_<UniformMatrixList<D>>(m, py_class_name.c_str())
      // had problems binding constructors directly, so I'll do manual conversion
      .def(py::init([](const vector<py::array_t<data_t>>& arrs) {
        vector<Matrix> mats;
        for (auto a : arrs) {
          mats.push_back(carma::arr_to_mat(a)); // should be copying
        }
        return UniformMatrixList<D>(mats);
      }))
      .def("dim", &UniformMatrixList<D>::dim, "n"_a = size_t(0), "gets dimensions of uniformly sized matrices")
      .def_property_readonly("size", py::overload_cast<>(&UniformMatrixList<D>::size), "size of container")
      .def("at", [](UniformMatrixList<D>& self, size_t n) { return carma::to_numpy_view(self.at(n)); }, "n"_a, "gets nth element")
      .def("__getitem__", [](UniformMatrixList<D>& self, size_t n) { return carma::to_numpy_view(self.at(n)); }, "n"_a, "gets nth element")
      .def("Swap", &UniformMatrixList<D>::Swap, "that"_a, "n"_a, "swaps input matrix with n^th matrix of list")
      ;
}

}  // namespace bindutils

