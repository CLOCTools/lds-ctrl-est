#include <carma>
#include <armadillo>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;


class MyClass {
  private:
    arma::Mat<double> A_;
  public:
    MyClass() { A_ = arma::Mat<double>(2, 2); };
    const arma::Mat<double>& A() const { return A_; };
    arma::Mat<double>& A_non_const() { return A_; };
};

PYBIND11_MODULE(example, m) {

    py::class_<MyClass>(m, "MyClass")
        .def(py::init<>())

        // 1. this works:
        .def("A_non_const", &MyClass::A_non_const)

        // 2. doesn't work: 
        .def("A", &MyClass::A);

        // 3. this works
        // .def("A", [](const MyClass &self) { return self.A(); });
}
