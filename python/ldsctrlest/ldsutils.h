#include <iostream>
#include <string>
#include <functional>

namespace ldsutils {

std::string capture_output(const std::function<void(void)> &f) {

    std::ostringstream capture_out;
    std::streambuf *coutbuf = std::cout.rdbuf();  // save old buf
    std::cout.rdbuf(
        capture_out.rdbuf());  // redirect std::cout to capture_out

    f();
    std::string captured_output = capture_out.str();

    std::cout.rdbuf(coutbuf);  // reset to standard output
    return captured_output;
}

}