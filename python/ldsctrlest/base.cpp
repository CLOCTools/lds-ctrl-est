#include <ldsCtrlEst_h/lds_sys.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <carma>

#include "ldsutils.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

PYBIND11_MODULE(base, m) {
  m.doc() = "Base classes from ldsCtrlEst";  // optional module docstring

  py::class_<lds::System>(m, "System")
      // don't need to wrap constructor; base class not used directly

      // getters only
      .def_property_readonly("n_u", &lds::System::n_u)
      .def_property_readonly("n_x", &lds::System::n_x)
      .def_property_readonly("n_y", &lds::System::n_y)
      .def_property_readonly("dt", &lds::System::dt)
      // .def_property_readonly("P", &lds::System::P)
      .def_property_readonly(
          "P", [](const lds::System &self) { return self.P(); },
          py::return_value_policy::copy)
      // .def_property_readonly("P_m", &lds::System::P_m)
      .def_property_readonly(
          "P_m", [](const lds::System &self) { return self.P_m(); },
          py::return_value_policy::copy)
      // .def_property_readonly("cx", &lds::System::cx)
      .def_property_readonly(
          "cx", [](const lds::System &self) { return self.cx(); },
          py::return_value_policy::copy)
      // .def_property_readonly("y", &lds::System::y)
      .def_property_readonly(
          "y", [](const lds::System &self) { return self.y(); },
          py::return_value_policy::copy)
      // .def_property_readonly("Ke", &lds::System::Ke)
      .def_property_readonly(
          "Ke", [](const lds::System &self) { return self.Ke(); },
          py::return_value_policy::copy)
      // .def_property_readonly("Ke_m", &lds::System::Ke_m)
      .def_property_readonly(
          "Ke_m", [](const lds::System &self) { return self.Ke_m(); },
          py::return_value_policy::copy)

      // // getters/setters
      // .def_property("x", &lds::System::x, &lds::System::set_x)
      .def_property(
          "x", [](const lds::System &self) { return self.x(); },
          &lds::System::set_x, py::return_value_policy::copy)
      // .def_property("x0", &lds::System::x0, &lds::System::set_x0)
      .def_property(
          "x0", [](const lds::System &self) { return self.x0(); },
          &lds::System::set_x0, py::return_value_policy::copy)
      // .def_property("m", &lds::System::m, &lds::System::set_m)
      // since set_m takes 2 args we can't use it directly in property
      .def("set_m", &lds::System::set_m, py::arg("m"), py::arg("do_force_assign") = false)
      .def_property(
          "m", [](const lds::System &self) { return self.m(); },
          [](lds::System &self, const lds::Vector &m) { self.set_m(m); },
          py::return_value_policy::copy)
      // .def_property("A", &lds::System::A, &lds::System::set_A)
      .def_property(
          "A", [](const lds::System &self) { return self.A(); },
          &lds::System::set_A, py::return_value_policy::copy)
      // .def_property("B", &lds::System::B, &lds::System::set_B)
      .def_property(
          "B", [](const lds::System &self) { return self.B(); },
          &lds::System::set_B, py::return_value_policy::copy)
      // .def_property("g", &lds::System::g, &lds::System::set_g)
      .def_property(
          "g", [](const lds::System &self) { return self.g(); },
          &lds::System::set_g, py::return_value_policy::copy)
      // .def_property("C", &lds::System::C, &lds::System::set_C)
      .def_property(
          "C", [](const lds::System &self) { return self.C(); },
          &lds::System::set_C, py::return_value_policy::copy)
      // .def_property("d", &lds::System::d, &lds::System::set_d)
      .def_property(
          "d", [](const lds::System &self) { return self.d(); },
          &lds::System::set_d, py::return_value_policy::copy)
      // // public field, no get/set functions
      .def_readwrite("do_adapt_m", &lds::System::do_adapt_m)

      // // setters only
      .def_property("Q", nullptr, &lds::System::set_Q)
      .def_property("Q_m", nullptr, &lds::System::set_Q_m)
      .def_property("P0", nullptr, &lds::System::set_P0)
      .def_property("P0_m", nullptr, &lds::System::set_P0_m)

      // other functions: shouldn't be called on abstract base class
      // but since they're not overridden I bind them here
      .def("Reset", &lds::System::Reset)
      // .def("Simulate", &lds::System::Simulate)
      .def("Filter", &lds::System::Filter)
      .def("f", &lds::System::f)

      .def("Print", &lds::System::Print)
      .def("__str__", [](lds::System &system) {
        return ldsutils::capture_output([&system]() { system.Print(); });
      });

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}
