#include <ldsCtrlEst_h/lds_gaussian_ctrl.h>
#include <ldsCtrlEst_h/lds_gaussian_fit.h>
#include <ldsCtrlEst_h/lds_gaussian_fit_em.h>
#include <ldsCtrlEst_h/lds_gaussian_fit_ssid.h>
#include <ldsCtrlEst_h/lds_gaussian_sctrl.h>
#include <ldsCtrlEst_h/lds_gaussian_sys.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <carma>  // must be included before armadillo (included in ldsCtrlEst)

#include "bindutils.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

// this allows for the shorthand "my_arg"_a instead of py::arg("my_arg")
using namespace pybind11::literals;
using namespace std;
using namespace lds;

namespace py = pybind11;
namespace glds = lds::gaussian;

PYBIND11_MODULE(gaussian, m) {
  m.doc() = "Gaussian classes from ldsCtrlEst";  // optional module docstring

  /*
  ---------------- Gaussian System class ---------------------
  */
  bindutils::define_System<glds::System>(m)
      .def(py::init<size_t, size_t, size_t, data_t, data_t, data_t, data_t>(),
           "n_u"_a, "n_x"_a, "n_y"_a, "dt"_a, "p0"_a = kDefaultP0,
           "q0"_a = kDefaultQ0, "r0"_a = kDefaultR0)
      // construct from fit for convience
      .def(py::init([](const glds::Fit& fit) {
        glds::System sys(fit.n_u(), fit.n_x(), fit.n_y(), fit.dt());
        sys.set_A(fit.A());
        sys.set_B(fit.B());
        sys.set_C(fit.C());
        sys.set_g(fit.g());
        sys.set_Q(fit.Q());
        sys.set_R(fit.R());
        sys.set_x0(fit.x0());
        sys.set_P0(fit.P0());
        sys.set_d(fit.d());
        return sys;
      }))

      // getters/setters not in base System
      .def_property("R", &glds::System::R, &glds::System::set_R)
      .def_property("Ke", &glds::System::Ke, &glds::System::set_Ke)
      .def_property("Ke_m", &glds::System::Ke_m, &glds::System::set_Ke_m);

  /*
  ---------------- Gaussian Fit class ---------------------
  */
  py::class_<glds::Fit, lds::Fit>(m, "Fit")
      // constructors
      .def(py::init<>())
      .def(py::init<size_t, size_t, size_t, data_t>(), "n_u"_a, "n_x"_a,
           "n_y"_a, "dt"_a);

  /*
  ---------------- UniformSystemList class ---------------------
  */
  bindutils::define_UniformSystemList<glds::System>(m);

  /*
  ---------------- EM Fit class ---------------------
  */
  bindutils::define_FitEM<glds::Fit, glds::FitEM>(m);

  /*
  ---------------- SSID Fit class ---------------------
  */
  bindutils::define_FitSSID<glds::Fit, glds::FitSSID>(m);

  /*
  ---------------- Base Controller class ---------------------
  */
  // inherited by both Controller and SwitchedController
  bindutils::define_BaseController<glds::System>(m);

  /*
  ---------------- Controller class ---------------------
  */
  bindutils::define_Controller<glds::Controller, glds::System>(m);

  /*
  ---------------- Switched Controller class ---------------------
  */
  bindutils::define_SwitchedController<glds::SwitchedController, glds::System>(
      m);

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}
