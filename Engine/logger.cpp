#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <stdarg.h>

using namespace std;

#define GL_LOG_FILE "gl.log"
#include <logger.h>

logger::logger()
{

}

logger::~logger()
{

}

bool logger::restart_gl_log() {
    FILE* file = fopen(GL_LOG_FILE, "w");

    if(!file){
        fprintf(stderr, "EROR: could not open GL_LOG_FILE log file %s for writing\n", GL_LOG_FILE);
        return false;
    }

    time_t now = time(NULL);
    char* date = ctime(&now);
    fprintf(file, "GL_LOG_FILE log. local time %s", date);
    cout << "starting log file" << endl;
    fclose(file);
    return true;
}

bool logger::gl_log(const char* message, ...){
    va_list argptr;
    FILE* file = fopen(GL_LOG_FILE, "a");
    if(!file){
        fprintf(stderr, "EROR: could not open GL_LOG_FILE log file %s for writing\n", GL_LOG_FILE);
        return false;
    }
    
    time_t now = time(NULL);
    char* date = ctime(&now);
    fprintf(file, "%s || %s", message, date);
    cout << message << endl;
    fclose(file);
    return true;
}