#ifndef MY_ENGINE
#define MY_ENGINE

#include "common.h"
#include "logger.h"

class engine
{
private:
    logger _logger;
    GLFWwindow* window;
    GLuint shader_program;
    GLuint vao;

    void getShaders();
    void compileShaders(const char* vertex_char_arr, const char* frag_char_arr);
    void _update_fps_counter();
public:
    engine(/* args */);
    ~engine();
    bool initialize(logger _logger);
    void run();
};

#endif