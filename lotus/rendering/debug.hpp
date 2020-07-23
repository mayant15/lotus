#include "lotus/debug.h"

// TODO: Wrapper over platform

void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
    std::string sourceString;
    std::string typeString;
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             sourceString = "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceString = "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceString = "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     sourceString = "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     sourceString = "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           sourceString = "Source: Other"; break;
    }
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               typeString = "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeString = "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeString = "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         typeString = "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         typeString = "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              typeString = "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          typeString = "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           typeString = "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               typeString = "Type: Other"; break;
    }
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         LOG_ERROR ("({}): {}. {}. {}", id, message, sourceString, typeString); break;
        case GL_DEBUG_SEVERITY_MEDIUM:       LOG_WARN  ("({}): {}. {}. {}", id, message, sourceString, typeString); break;
        case GL_DEBUG_SEVERITY_LOW:          LOG_WARN  ("({}): {}. {}. {}", id, message, sourceString, typeString); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_INFO  ("({}): {}. {}. {}", id, message, sourceString, typeString); break;
    }
}

/**
 * Print out an error message with file name and line number
 */
GLenum glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }
        LOG_ERROR("{} | {} ({})", error, file, line);
    }
    return errorCode;
}
