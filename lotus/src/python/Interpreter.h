#pragma once

class Interpreter
{
private:
    Interpreter();
public:
    Interpreter(const Interpreter&) = delete;
    Interpreter& operator=(const Interpreter&) = delete;

    static Interpreter& get();

    static void exec_file(const std::string& filepath);
};
