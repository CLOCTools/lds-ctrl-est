#include <ldsCtrlEst_h/lds_ctrl.h>
#include <ldsCtrlEst_h/lds_fit_em.h>
#include <ldsCtrlEst_h/lds_gaussian_fit_em.h>
#include <ldsCtrlEst_h/lds_poisson_fit_em.h>
#include <ldsCtrlEst_h/lds_sctrl.h>
#include <ldsCtrlEst_h/lds_uniform_mats.h>
#include <ldsCtrlEst_h/lds_uniform_systems.h>
#include <ldsCtrlEst_h/lds_uniform_vecs.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <carma>  // must be included before armadillo (included in ldsCtrlEst)
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

// most shared System functionality is in base System class
// but here we define shared functions that just need to
// be templated
template <typename Sys>
py::class_<Sys> define_System(py::module &m) {
  return py::class_<Sys, lds::System>(m, "System")
      .def(py::init<>())  // default constructor
      .def("copy", [](const Sys &self) { return Sys(self); })
      .def("Simulate", &Sys::Simulate)
      .def("Print", &Sys::Print)
      .def("__str__",
           [](Sys &system) {
             return bindutils::capture_output([&system]() { system.Print(); });
           })
      // see matlab/[GLDS|PLDS]/simulate.m
      .def(
          "simulate_block",
          [](Sys &sys, vector<Matrix> u) -> vector<vector<Matrix>> {
            vector<Matrix> x;
            vector<Matrix> y;
            vector<Matrix> z;
            for (size_t k = 0; k < u.size(); k++) {
              size_t n_t = arma::size(u[k])[1];
              Matrix x_k(sys.n_x(), n_t, fill::zeros);
              Matrix y_k(sys.n_y(), n_t, fill::zeros);
              Matrix z_k(sys.n_y(), n_t, fill::zeros);
              for (size_t t = 1; t < n_t; t++) {
                z_k.col(t) = sys.Simulate(u[k].col(t - 1));
                y_k.col(t) = sys.y();
                x_k.col(t) = sys.x();
              }
              x.push_back(x_k);
              y.push_back(y_k);
              z.push_back(z_k);
            }
            return {y, x, z};
          },
          "simulates for multiple trials/timesteps. u is list of n_u X n_t "
          "arrays")
      .def("nstep_pred_block", &Sys::nstep_pred_block)
      // see matlab/[GLDS|PLDS]/simulate_imp.m
      .def(
          "simulate_imp",
          [](Sys &sys, size_t n_t) {
            Matrix old_Q = sys.Q();
            // 0 noise to simulate impulse response
            sys.set_Q(Matrix(sys.n_x(), sys.n_x(), fill::zeros));
            vector<Matrix> cx_imp(sys.n_u(),
                                  Matrix(sys.n_y(), n_t, fill::zeros));
            Vector u_imp(n_t, fill::zeros);
            u_imp[0] = 1;
            for (size_t which_u = 0; which_u < sys.n_u(); which_u++) {
              Vector x(sys.n_x(), fill::zeros);
              for (size_t t = 1; t < n_t; t++) {
                x = sys.A() * x +
                    sys.B().col(which_u) * (sys.g()[which_u]) * u_imp[t - 1];
                cx_imp[which_u].col(t) = sys.C() * x;
              }
            }
            sys.set_Q(old_Q);
            return cx_imp;
          },
          "simulates impulse response. returns list of length n_u of n_y X n_t "
          "arrays. result is Cx: y for GLDS, logy for PLDS.");
}

// The FitEM classes are children of abstract EM<Fit> (either Gaussian or
// Poisson) so this function defines the base EM<Fit> functionality to avoid
// repeating it.
template <typename FitType, typename FitEMType>
py::class_<FitEMType> define_FitEM(py::module &m) {
  return py::class_<FitEMType>(m, "FitEM")
      // constructors
      .def(py::init<>())
      .def(py::init<size_t, data_t, UniformMatrixList<kMatFreeDim2>,
                    UniformMatrixList<kMatFreeDim2>>())
      .def(py::init<const FitType &, UniformMatrixList<kMatFreeDim2>,
                    UniformMatrixList<kMatFreeDim2>>())

      // functions
      .def("Run", &FitEMType::Run, "calc_dynamics"_a = true, "calc_Q"_a = true,
           "calc_init"_a = true, "calc_output"_a = true,
           "calc_measurement"_a = true, "max_iter"_a = 100, "tol"_a = 1e-2)
      .def("ReturnData", &FitEMType::ReturnData,
           "Returns the input/output data to caller")

      // getters
      .def_property_readonly("x", &FitEMType::x, "estimated state (over time)")
      .def_property_readonly("y", &FitEMType::y, "estimated output (over time)")
      .def_property_readonly("sum_E_x_t_x_t", &FitEMType::sum_E_x_t_x_t,
                             "state-input covariance")
      .def_property_readonly("sum_E_xu_tm1_xu_tm1",
                             &FitEMType::sum_E_xu_tm1_xu_tm1,
                             "state-input covariance (t-minus-1")
      .def_property_readonly("sum_E_xu_t_xu_tm1", &FitEMType::sum_E_xu_t_xu_tm1,
                             "single-lag state-input covariance")
      .def_property_readonly("n_t_tot", &FitEMType::n_t_tot,
                             "total number of time samples")
      .def_property_readonly("theta", &FitEMType::theta,
                             "parameters updated in M step");
}

template <typename FitType, typename FitSSIDType>
py::class_<FitSSIDType> define_FitSSID(py::module &m) {
  return py::class_<FitSSIDType>(m, "FitSSID")
      // constructors
      .def(py::init<>())
      .def(py::init<size_t, size_t, data_t, UniformMatrixList<kMatFreeDim2>,
                    UniformMatrixList<kMatFreeDim2>, const Vector &>(),
           "n_x"_a, "n_h"_a, "dt"_a, "u_train"_a, "z_train"_a,
           "d"_a = Vector(1).fill(-kInf))

      // functions
      .def("Run", &FitSSIDType::Run, "ssid_wt"_a)
      .def("ReturnData", &FitSSIDType::ReturnData,
           "Returns the input/output data to caller");
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
      .def("__len__", py::overload_cast<>(&UML::size))
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

template <typename System>
py::class_<Controller<System>> define_BaseController(py::module &m) {
  using Ctrl = Controller<System>;
  return py::class_<Ctrl>(m, "BaseController")
      // functions
      .def("Control", &Ctrl::Control, "z"_a, "do_control"_a = true,
           "do_lock_control"_a = false, "sigma_soft_start"_a = 0,
           "sigma_u_noise"_a = 0, "do_reset_at_control_onset"_a = true,
           "updates control signal (single-step)")
      .def("ControlOutputReference", &Ctrl::ControlOutputReference, "z"_a,
           "do_control"_a = true, "do_estimation"_a = true,
           "do_lock_control"_a = false, "sigma_soft_start"_a = 0,
           "sigma_u_noise"_a = 0, "do_reset_at_control_onset"_a = true,
           "updates control signal, given previously set y_ref (single-step)")
      .def("Reset", &Ctrl::Reset, "reset system and control variables")
      .def("Print", &Ctrl::Print, "prints variables to stdout")
      .def("__str__",
           [](Ctrl &ctrl) {
             return capture_output([&ctrl]() { ctrl.Print(); });
           })

      // getters/setters
      .def_property("sys", &Ctrl::sys, &Ctrl::set_sys)
      .def_property("Kc", &Ctrl::Kc, &Ctrl::set_Kc)
      .def_property("Kc_inty", &Ctrl::Kc_inty, &Ctrl::set_Kc_inty)
      .def_property("Kc_u", &Ctrl::Kc_u, &Ctrl::set_Kc_u)
      .def_property("g_design", &Ctrl::g_design, &Ctrl::set_g_design)
      .def_property("u_ref", &Ctrl::u_ref, &Ctrl::set_u_ref)
      .def_property("x_ref", &Ctrl::x_ref, &Ctrl::set_x_ref)
      .def_property("control_type", &Ctrl::control_type,
                    &Ctrl::set_control_type)
      .def_property("tau_awu", &Ctrl::tau_awu, &Ctrl::set_tau_awu)
      .def_property("u_lb", &Ctrl::u_lb, &Ctrl::set_u_lb)
      .def_property("u_ub", &Ctrl::u_ub, &Ctrl::set_u_ub);
}

template <typename Ctrl, typename System>
py::class_<Ctrl, Controller<System>> define_Controller(py::module &m) {
  return py::class_<Ctrl, Controller<System>>(m, "Controller")
      // constructors
      .def(py::init<>())
      // only binding copy constructor, not move constructor
      .def(py::init<const System &, data_t, data_t, size_t>(), "sys"_a,
           "u_lb"_a, "u_ub"_a, "control_type"_a = 0)

      .def_property("y_ref", &Ctrl::y_ref, &Ctrl::set_y_ref);
}

template <typename SCtrl, typename System>
py::class_<SCtrl, Controller<System>> define_SwitchedController(py::module &m) {
  return py::class_<SCtrl, Controller<System>>(m, "SwitchedController")
      // constructors
      .def(py::init<>())
      // only binding copy constructor, not move constructor
      .def(py::init<const std::vector<System> &, data_t, data_t, size_t>(),
           "systems"_a, "u_lb"_a, "u_ub"_a, "control_type"_a = 0)

      // functions
      .def("Switch", &SCtrl::Switch, "idx"_a, "do_force_switch"_a = false,
           "switch to a different sub-system/controller")

      // setters
      // using copy, not moves, specified with overload_cast
      .def_property(
          "Kc", nullptr,
          py::overload_cast<const UniformMatrixList<> &>(&SCtrl::set_Kc))
      .def_property(
          "Kc_inty", nullptr,
          py::overload_cast<const UniformMatrixList<> &>(&SCtrl::set_Kc_inty))
      .def_property(
          "Kc_u", nullptr,
          py::overload_cast<const UniformMatrixList<> &>(&SCtrl::set_Kc_u))
      .def_property(
          "g_design", nullptr,
          py::overload_cast<const UniformVectorList &>(&SCtrl::set_g_design))

      // getter/setter
      .def_property("y_ref", &SCtrl::y_ref, &SCtrl::set_y_ref);
}

}  // namespace bindutils
