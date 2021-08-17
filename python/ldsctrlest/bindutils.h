#include <ldsCtrlEst_h/lds_fit_em.h>
#include <ldsCtrlEst_h/lds_gaussian_fit_em.h>
#include <ldsCtrlEst_h/lds_poisson_fit_em.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <carma>
#include <functional>
#include <iostream>
#include <string>

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

// again need to use templates, which I can't do in the PYBIND11_MODULE block
template <MatrixListFreeDim D>
py::class_<UniformMatrixList<D>, vector<Matrix>> define_UniformMatrixList(
    py::module& m, string py_class_suffix) {

  string py_class_name = string("UniformMatrixList") + py_class_suffix;
  return py::class_<UniformMatrixList<D>, vector<Matrix>>(m, py_class_name.c_str())
      .def(py::init<const vector<Matrix>&>())
      .def(py::init<vector<Matrix>&&>())
      .def(py::init<const UniformMatrixList<D>&>())
      .def(py::init<UniformMatrixList<D>&&>())

      ;
}

}  // namespace bindutils