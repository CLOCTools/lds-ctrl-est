#include <ldsCtrlEst_h/lds_poisson_ctrl.h>
#include <ldsCtrlEst_h/lds_poisson_fit.h>
#include <ldsCtrlEst_h/lds_poisson_fit_em.h>
#include <ldsCtrlEst_h/lds_poisson_fit_ssid.h>
#include <ldsCtrlEst_h/lds_poisson_sctrl.h>
#include <ldsCtrlEst_h/lds_poisson_sys.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include <carma>  // must come before armadillo

#include "bindutils.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

// this allows for the shorthand "my_arg"_a instead of py::arg("my_arg")
using namespace pybind11::literals;
using namespace lds;
using namespace std;

namespace py = pybind11;
namespace plds = lds::poisson;

PYBIND11_MODULE(poisson, m) {
  m.doc() = "Poisson classes from ldsCtrlEst";

  /*
  ---------------- Poisson System class ---------------------
  */
  bindutils::define_System<plds::System>(m)
      // unlike GLDS, PLDS has no getters/setters not in base System
      // the only unique thing is the constructor
      .def(py::init<size_t, size_t, size_t, data_t, data_t, data_t>(),
           py::arg("n_u"), py::arg("n_x"), py::arg("n_y"), py::arg("dt"),
           py::arg("p0") = kDefaultP0, py::arg("q0") = kDefaultQ0)
      // construct from fit for convience
      .def(py::init([](const plds::Fit& fit) {
        plds::System sys(fit.n_u(), fit.n_x(), fit.n_y(), fit.dt());
        sys.set_A(fit.A());
        sys.set_B(fit.B());
        sys.set_C(fit.C());
        sys.set_g(fit.g());
        sys.set_Q(fit.Q());
        sys.set_x0(fit.x0());
        sys.set_P0(fit.P0());
        sys.set_d(fit.d());
        return sys;
      }));

  /*
  ---------------- Poisson Fit class ---------------------
  */
  py::class_<plds::Fit, lds::Fit>(m, "Fit")
      // constructors
      .def(py::init<>())
      .def(py::init<size_t, size_t, size_t, data_t>(), "n_u"_a, "n_x"_a,
           "n_y"_a, "dt"_a);

  /*
  ---------------- UniformSystemList class ---------------------
  */
  bindutils::define_UniformSystemList<plds::System>(m);

  /*
  ---------------- EM Fit class ---------------------
  */
  bindutils::define_FitEM<plds::Fit, plds::FitEM>(m);

  /*
  ---------------- SSID Fit class ---------------------
  */
  bindutils::define_FitSSID<plds::Fit, plds::FitSSID>(m);

  /*
  ---------------- base Controller class ---------------------
  */
  // inherited by Controller and SwitchedController
  bindutils::define_BaseController<plds::System>(m);

  /*
  ---------------- Controller class ---------------------
  */
  bindutils::define_Controller<plds::Controller, plds::System>(m);

  /*
  ---------------- Switched Controller class ---------------------
  */
  bindutils::define_SwitchedController<plds::SwitchedController, plds::System>(
      m);

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}
