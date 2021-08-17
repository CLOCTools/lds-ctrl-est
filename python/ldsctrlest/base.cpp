#include <ldsCtrlEst_h/lds_fit.h>
#include <ldsCtrlEst_h/lds_sys.h>
#include <ldsCtrlEst_h/lds_fit_em.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <carma>

#include "bindutils.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;
using namespace pybind11::literals;
using namespace std;
using namespace lds;

PYBIND11_MODULE(base, m) {
  m.doc() = "Base classes from ldsCtrlEst";  // optional module docstring

  /*
  ---------------- System class ---------------------
  */
  py::class_<System>(m, "System")
      // don't need to wrap constructor; base class not used directly

      // getters only
      .def_property_readonly("n_u", &System::n_u)
      .def_property_readonly("n_x", &System::n_x)
      .def_property_readonly("n_y", &System::n_y)
      .def_property_readonly("dt", &System::dt)
      // note: there's a way to get read-only views out, but I don't
      // think it's worth the verbosity here. e.g.,:
    //   .def_property_readonly(
    //       "P", [](const System &self) { return carma::to_numpy_view(self.P()); })
      // see https://github.com/RUrlus/carma/issues/83
      .def_property_readonly("P", &System::P)
      .def_property_readonly("P_m", &System::P_m)
      .def_property_readonly("cx", &System::cx)
      .def_property_readonly("y", &System::y)
      .def_property_readonly("Ke", &System::Ke)
      .def_property_readonly("Ke_m", &System::Ke_m)

      // getters/setters
      .def_property("x", &System::x, &System::set_x)
      .def_property("x0", &System::x0, &System::set_x0)
      // since set_m takes 2 args we can't use it directly in property
      .def("set_m", &System::set_m, py::arg("m"),
           "do_force_assign"_a = false)
      .def_property(
          "m", &System::m,
          [](System &self, const Vector &m) { self.set_m(m); })
      .def_property("A", &System::A, &System::set_A)
      .def_property("B", &System::B, &System::set_B)
      .def_property("g", &System::g, &System::set_g)
      .def_property("C", &System::C, &System::set_C)
      .def_property("d", &System::d, &System::set_d)
      // // public field, no get/set functions
      .def_readwrite("do_adapt_m", &System::do_adapt_m)

      // // setters only
      .def_property("Q", nullptr, &System::set_Q)
      .def_property("Q_m", nullptr, &System::set_Q_m)
      .def_property("P0", nullptr, &System::set_P0)
      .def_property("P0_m", nullptr, &System::set_P0_m)

      // other functions: shouldn't be called on abstract base class
      // but since they're not overridden I bind them here
      .def("Reset", &System::Reset)
      // .def("Simulate", &System::Simulate)
      .def("Filter", &System::Filter)
      .def("f", &System::f)

      .def("Print", &System::Print)
      .def("__str__", [](System &system) {
        return bindutils::capture_output([&system]() { system.Print(); });
      });

  /*
  ---------------- Fit class ---------------------
  */
  py::class_<Fit>(m, "Fit")
      // don't bind constructors since this is an abstract class

      // get-only properties
      // don't need to use lambdas here because return type isn't const
      .def_property_readonly("n_u", &Fit::n_u)
      .def_property_readonly("n_x", &Fit::n_x)
      .def_property_readonly("n_y", &Fit::n_y)
      .def_property_readonly("dt", &Fit::dt)

      // get/set properties
      .def_property("A", &Fit::A, &Fit::set_A)
      .def_property("B", &Fit::B, &Fit::set_B)
      .def_property("g", &Fit::g, &Fit::set_g)
      .def_property("m", &Fit::m, &Fit::set_m)
      .def_property("Q", &Fit::Q, &Fit::set_Q)
      .def_property("x0", &Fit::x0, &Fit::set_x0)
      .def_property("P0", &Fit::P0, &Fit::set_P0)
      .def_property("C", &Fit::C, &Fit::set_C)
      .def_property("d", &Fit::d, &Fit::set_d)
      // pure virtual: override seems to be working as expected
      .def_property("R", &Fit::R, &Fit::set_R, "measurement noise (not used in PLDS)")

      // system functions
    //   .def("f",
    //        py::overload_cast<Matrix &, const Matrix &, size_t>(
    //            &Fit::f),
    //        "system dynamics function")
      // x should be changed: borrow memory
      // TODO: is this even possible? maybe not
      .def("f", [](Fit& self, py::array_t<data_t>& x, const Matrix& u, size_t t) {
          Matrix x_arma = carma::arr_to_mat(x, false);
          Vector result = Vector(self.f(x_arma, u, t));
        //   cout << x_arma;
        //   x_arma = x_arma.zeros();
        //   cout << x_arma;
          return carma::col_to_arr(result, true);
      })
      .def("f", [](Fit& self, Matrix& x_pre, const Matrix& x_post, const Matrix& u, size_t t) {
          return Vector(self.f(x_pre, x_post, u, t));
      })
      .def("h", [](Fit& self, Matrix& y, const Matrix& x, size_t t) {
          return Vector(self.h(y, x, t));
      });
    
  /*
  ---------------- Uniform mat/vec classes ---------------------
  */
  // need to define for each template parameter (Dth dimension free to vary)
  bindutils::define_UniformMatrixList<kMatFreeDimNone>(m, "FreeDimNone");
#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}
