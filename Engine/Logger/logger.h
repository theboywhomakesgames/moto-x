#ifndef MY_LOGGER
#define MY_LOGGER

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <stdarg.h>

using namespace std;

#define GL_LOG_FILE "gl.log"

class logger
{
    private:
        
    public:
        logger();
        ~logger();
        bool restart_gl_log();
        bool gl_log(const char* message, ...);
};

#endif