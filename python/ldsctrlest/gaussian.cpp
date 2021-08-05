#include <ldsCtrlEst_h/lds_gaussian_sys.h>
#include <ldsCtrlEst_h/lds_sys.h>
#include <pybind11/pybind11.h>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;
namespace glds = lds::gaussian;

PYBIND11_MODULE(gaussian, m) {
  m.doc() = "Gaussian classes from ldsCtrlEst";  // optional module docstring

  py::class_<glds::System, lds::System>(m, "System")
      .def(py::init<>())
      .def(py::init<std::size_t, std::size_t, std::size_t, lds::data_t,
                    lds::data_t, lds::data_t, lds::data_t>(),
           py::arg("n_u"), py::arg("n_x"), py::arg("n_y"), py::arg("dt"),
           py::arg("p0") = lds::kDefaultP0, py::arg("q0") = lds::kDefaultQ0,
           py::arg("r0") = lds::kDefaultR0)

      // .def(py::init([](
      //         std::size_t n_u, std::size_t n_x, std::size_t &n_y, lds::data_t
      //         dt, lds::data_t p0, lds::data_t q0, lds::data_t r0
      //         // py::arg("p0") = lds::kDefaultP0, py::arg("q0") =
      //         lds::kDefaultQ0,
      //         // py::arg("r0") = lds::kDefaultR0
      //     ) {
      //         return new glds::System(n_u, n_x, n_y, dt, p0, q0,
      //         r0);
      //     }, py::arg("p0") = lds::kDefaultP0)
      // )

      // getters/setters
      .def_property("R", &glds::System::R,
                    &glds::System::set_R)
      .def_property("Ke", &glds::System::Ke,
                    &glds::System::set_Ke)
      .def_property("Ke_m", &glds::System::Ke_m,
                    &glds::System::set_Ke_m)

      // other functions
      // NOTE: this should be unnecessary if functions were virtual,
      // but I'm not touching the base code right now
      .def("Reset", &glds::System::Reset)
      .def("Simulate", &glds::System::Simulate)
      .def("Filter", &glds::System::Filter)
      .def("f", &glds::System::f)

      .def("Print", &glds::System::Print)
      .def("__str__", [](glds::System &system) {
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
