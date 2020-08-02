#include "pybind11/embed.h"
#include "lotus/python.h"
#include "Interpreter.h"

int add(int i, int j)
{
    return i + j;
}

void exec_file(const std::string& filepath)
{
    Interpreter::exec_file(filepath);
}

PYBIND11_EMBEDDED_MODULE(lotus, m)
{
    m.doc() = "pybind11 example plugin"; // module docstring
    m.def("add", &add, "Function that adds");
}
