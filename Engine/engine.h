#ifndef MY_ENGINE
#define MY_ENGINE

#include "common.h"
#include "logger.h"

class engine
{
private:
    logger _logger;
    GLFWwindow* window;
    GLuint shader_programme;
    GLuint vao;

    void getShaders();
    void compileShaders(const char* vertex_char_arr, const char* frag_char_arr);
public:
    engine(/* args */);
    ~engine();
    bool initialize(logger _logger);
    void run();
};

#endif