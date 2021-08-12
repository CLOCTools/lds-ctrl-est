#include <ldsCtrlEst_h/lds_poisson_sys.h>
#include <ldsCtrlEst_h/lds_sys.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <carma>

#include "ldsutils.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;
namespace plds = lds::poisson;

PYBIND11_MODULE(poisson, m) {
  m.doc() = "Poisson classes from ldsCtrlEst";

  py::class_<plds::System, lds::System>(m, "System")
      .def(py::init<>())  // default constructor
      .def(py::init<std::size_t, std::size_t, std::size_t, lds::data_t,
                    lds::data_t, lds::data_t>(),
           py::arg("n_u"), py::arg("n_x"), py::arg("n_y"), py::arg("dt"),
           py::arg("p0") = lds::kDefaultP0, py::arg("q0") = lds::kDefaultQ0)

      // unlike GLDS, PLDS has no getters/setters not in base System

      // other functions
      // NOTE: this might be unnecessary if functions were virtual,
      // but I'm not touching the base code right now
      .def(
          "Simulate",
          [](plds::System& self, const lds::Vector& u_tm1) {
            return self.Simulate(u_tm1);
          },
          py::return_value_policy::copy);

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}
