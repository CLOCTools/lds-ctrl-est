#include <ldsCtrlEst_h/lds_sys.h>
#include <pybind11/pybind11.h>

#include <iostream>
#include <string>

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
      .def_property_readonly("P", &lds::System::P)
      .def_property_readonly("P_m", &lds::System::P_m)
      .def_property_readonly("cx", &lds::System::cx)
      .def_property_readonly("y", &lds::System::y)
      .def_property_readonly("Ke", &lds::System::Ke)
      .def_property_readonly("Ke_m", &lds::System::Ke_m)

      // getters/setters
      .def_property("x", &lds::System::x, &lds::System::set_x)
      .def_property("x0", &lds::System::x0, &lds::System::set_x0)
      .def_property("m", &lds::System::m, &lds::System::set_m)
      .def_property("A", &lds::System::A, &lds::System::set_A)
      .def_property("B", &lds::System::B, &lds::System::set_B)
      .def_property("g", &lds::System::g, &lds::System::set_g)
      .def_property("C", &lds::System::C, &lds::System::set_C)
      .def_property("d", &lds::System::d, &lds::System::set_d)
      // public field, no get/set functions
      .def_readwrite("do_adapt_m", &lds::System::do_adapt_m)

      // setters only
      .def_property("Q", nullptr, &lds::System::set_Q)
      .def_property("Q_m", nullptr, &lds::System::set_Q_m)
      .def_property("P0", nullptr, &lds::System::set_P0)
      .def_property("P0_m", nullptr, &lds::System::set_P0_m)

      // other functions
      .def("Reset", &lds::System::Reset)
      .def("Simulate", &lds::System::Simulate)
      .def("Filter", &lds::System::Filter)
      .def("f", &lds::System::f)

      .def("Print", &lds::System::Print)
      .def("__str__", [](lds::System &system) {
        std::ostringstream capture_out;
        std::streambuf *coutbuf = std::cout.rdbuf();  // save old buf
        std::cout.rdbuf(
            capture_out.rdbuf());  // redirect std::cout to capture_out

        system.Print();
        std::string captured_output = capture_out.str();

        std::cout.rdbuf(coutbuf);  // reset to standard output
        return captured_output;
      });

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}
