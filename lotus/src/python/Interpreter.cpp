#include "pybind11/embed.h"
#include "Interpreter.h"

Interpreter::Interpreter() {
//    pybind11::scoped_interpreter guard{};
}

Interpreter& Interpreter::get()
{
    static Interpreter instance;
    return instance;
}

void Interpreter::exec_file(const std::string &filepath) {
//    Interpreter::get();
    pybind11::scoped_interpreter guard{};
    pybind11::eval_file(filepath);
}
