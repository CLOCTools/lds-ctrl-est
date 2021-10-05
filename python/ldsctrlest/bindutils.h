#include <carma>  // must be included before armadillo (included in ldsCtrlEst)
#include <ldsCtrlEst_h/lds_fit_em.h>
#include <ldsCtrlEst_h/lds_gaussian_fit_em.h>
#include <ldsCtrlEst_h/lds_poisson_fit_em.h>
#include <ldsCtrlEst_h/lds_uniform_mats.h>
#include <ldsCtrlEst_h/lds_uniform_systems.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

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
py::class_<FitEMType> define_FitEM_base(py::module &m) {
  return py::class_<FitEMType>(m, "FitEM")
      // constructors
      .def(py::init<>())
      // .def(py::init<size_t, data_t, UniformMatrixList<kMatFreeDim2>&,
      //               UniformMatrixList<kMatFreeDim2>&>())
      // automatic signature matching not working here, with && on mat lists?
      .def(py::init([](size_t n_x, data_t dt,
                       UniformMatrixList<kMatFreeDim2> &u_train,
                       UniformMatrixList<kMatFreeDim2> &z_train) {
        return FitEMType(n_x, dt, move(u_train), move(z_train));
      }))
      // .def(py::init<const FitType &, UniformMatrixList<kMatFreeDim2> &&,
      // UniformMatrixList<kMatFreeDim2> &&>())

      // functions
      .def("Run", &FitEMType::Run, "calc_dynamics"_a = true, "calc_Q"_a = true,
           "calc_init"_a = true, "calc_output"_a = true,
           "calc_measurement"_a = true, "max_iter"_a = 100, "tol"_a = 1e-2);
}

void println(string message) { cerr << message << endl; }

// again need to use templates, which I can't do in the PYBIND11_MODULE block
template <MatrixListFreeDim D>
py::class_<UniformMatrixList<D>> define_UniformMatrixList(
    py::module &m, string py_class_suffix) {
  using UML = UniformMatrixList<D>;
  string py_class_name = string("UniformMatrixList") + py_class_suffix;
  return py::class_<UML>(m, py_class_name.c_str())
      // had problems binding constructors directly, so I'll do manual
      // conversion
      .def(py::init([](const vector<py::array_t<data_t>> &arrs) {
        vector<Matrix> mats;
        for (auto a : arrs) {
          mats.push_back(carma::arr_to_mat(a));  // should be copying
        }
        return UML(mats);
      }))
      .def("dim", &UML::dim, "n"_a = size_t(0),
           "gets dimensions of uniformly sized matrices")
      .def_property_readonly("size", py::overload_cast<>(&UML::size),
                             "size of container")
      // don't know why I get warnings about at(), or why things go wrong when I
      // try to bind it directly, but this is working at least:
      .def(
          "at",
          [](const UML &self, size_t n) {
            return carma::to_numpy_view(self.at(n));
          },
          "n"_a, "gets nth element")
      .def(
          "__getitem__",
          [](const UML &self, size_t n) {
            return carma::to_numpy_view(self.at(n));
          },
          "n"_a, "gets nth element")
      .def("Swap", &UML::Swap, "that"_a, "n"_a,
           "swaps input matrix with n^th matrix of list");
}

template <typename System>
py::class_<UniformSystemList<System>> define_UniformSystemList(py::module &m) {
  using USL = UniformSystemList<System>;

  return py::class_<USL>(m, "UniformSystemList")
      .def(py::init([](const vector<System> &systems) { return USL(systems); }))
      .def_property_readonly("dim", &USL::dim,
                             "gets dimensions of uniformly sized systems")
      .def_property_readonly("size", py::overload_cast<>(&USL::size),
                             "size of container")
      .def("at", py::overload_cast<size_t>(&vector<System>::at), "n"_a,
           "gets n^th system of list")
      .def("__getitem__", py::overload_cast<size_t>(&vector<System>::at), "n"_a,
           "gets n^th system of list")
      .def("Swap", &USL::Swap, "that"_a, "n"_a,
           "swaps input system with n^th system of list");
}

}  // namespace bindutils
