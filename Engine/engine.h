#ifndef MOTOX_ENGINE
#define MOTOX_ENGINE

#include "common.h"
#include "logger.h"
#include "Shader/shader.h"

class engine
{
    private:
        GLFWwindow* window;
        GLuint vao, ebo;
        
        vector<shader> shaders;

        void getShaders();
        void _update_fps_counter();
    public:
        logger engine_logger;
        engine(/* args */);
        ~engine();
        bool initialize(logger engine_logger);
        void run();
};

#endif