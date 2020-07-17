#include <spdlog/spdlog.h>
#include <glad/glad.h>

#define LOG_INFO(...)   spdlog::info(__VA_ARGS__)
#define LOG_WARN(...)   spdlog::warn(__VA_ARGS__)
#define LOG_ERROR(...)  spdlog::error(__VA_ARGS__)

#define glCheckError() glCheckError_(__FILE__, __LINE__)

GLenum glCheckError_(const char* file, int line);
void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
