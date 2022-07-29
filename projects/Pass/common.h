#pragma once

#include <glad/glad.h>

namespace zeno {
namespace opengl_details {

static const char *get_opengl_error_string(GLenum err) {
    switch (err) {
#define PER_GL_ERR(x) \
    case x: return #x;
        PER_GL_ERR(GL_NO_ERROR)
        PER_GL_ERR(GL_INVALID_ENUM)
        PER_GL_ERR(GL_INVALID_VALUE)
        PER_GL_ERR(GL_INVALID_OPERATION)
        PER_GL_ERR(GL_INVALID_FRAMEBUFFER_OPERATION)
        PER_GL_ERR(GL_OUT_OF_MEMORY)
        PER_GL_ERR(GL_STACK_UNDERFLOW)
        PER_GL_ERR(GL_STACK_OVERFLOW)
#undef PER_GL_ERR
    }
    static char tmp[233];
    sprintf(tmp, "%d\n", err);
    return tmp;
}

static void _check_opengl_error(const char *file, int line, const char *hint) {
    auto err = glGetError();
#if defined(__GUNC__) || defined(__clang__)
    if (__builtin_expect(!!(err != GL_NO_ERROR), 0)) {
#else
    if (err != GL_NO_ERROR) {
#endif
        auto msg = get_opengl_error_string(err);
        throw zeno::makeError((std::string)file + ':' + std::to_string(line) +
                              ": " + hint + ": " + msg);
    }
}

#define CHECK_GL(x)                                                          \
    do {                                                                     \
        (x);                                                                 \
        ::zeno::opengl_details::_check_opengl_error(__FILE__, __LINE__, #x); \
    } while (0)

}
}
