#include <ldsCtrlEst_h/lds_gaussian_sys.h>
#include <ldsCtrlEst_h/lds_sys.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <carma>

#include "ldsutils.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;
namespace glds = lds::gaussian;

PYBIND11_MODULE(gaussian, m) {
  m.doc() = "Gaussian classes from ldsCtrlEst";  // optional module docstring

  py::class_<glds::System, lds::System>(m, "System")
      .def(py::init<>())  // default constructor
      .def(py::init<std::size_t, std::size_t, std::size_t, lds::data_t,
                    lds::data_t, lds::data_t, lds::data_t>(),
           py::arg("n_u"), py::arg("n_x"), py::arg("n_y"), py::arg("dt"),
           py::arg("p0") = lds::kDefaultP0, py::arg("q0") = lds::kDefaultQ0,
           py::arg("r0") = lds::kDefaultR0)

      // getters/setters
      // .def_property("R", &glds::System::R, &glds::System::set_R)
      // .def_property("Ke", &glds::System::Ke, &glds::System::set_Ke)
      // .def_property("Ke_m", &glds::System::Ke_m, &glds::System::set_Ke_m)

      // other functions
      // NOTE: this might be unnecessary if functions were virtual,
      // but I'm not touching the base code right now
      // .def("Simulate", &glds::System::Simulate)
      .def(
          "Simulate",
          [](glds::System& self, const lds::Vector& u_tm1) {
            return self.Simulate(u_tm1);
          },
          py::return_value_policy::copy)

      .def("Print", &glds::System::Print)
      .def("__str__", [](glds::System& system) {
        return ldsutils::capture_output([&system]() { system.Print(); });
      });

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}
