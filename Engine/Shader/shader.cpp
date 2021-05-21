#include "../common.h"
#include "../Logger/logger.h"
#include "shader.h"

shader::shader(const char* vertex_path, const char* fragment_path, logger engine_logger){
    this->_logger = engine_logger;
    this->vertex_path = vertex_path;
    this->fragment_path = fragment_path;
}

void shader::get_shaders(){
    _logger.gl_log("importing shader files");
    std::ifstream ifs(this->vertex_path);
    std::string content_vtx((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()));
    this->vertex_char_arr = &content_vtx[0];
    ifs.close();

    ifs.open(this->fragment_path);
    std::string content_frag((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()));
    this->frag_char_arr = &content_frag[0];
    ifs.close();

    _logger.gl_log("vertex shader:");
    _logger.gl_log("=================");
    _logger.gl_log(vertex_char_arr);
    _logger.gl_log("=================");
    _logger.gl_log("");
    _logger.gl_log("");

    _logger.gl_log("fragment shader");
    _logger.gl_log("=================");
    _logger.gl_log(frag_char_arr);
    _logger.gl_log("=================");
    _logger.gl_log("");
    _logger.gl_log("");

    compile_shaders();
}

void shader::compile_shaders(){
    _logger.gl_log("creating shaders");
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_char_arr, NULL);
    glCompileShader(vs);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &frag_char_arr, NULL);
    glCompileShader(fs);

    int success_vtx, success_frg;
    char infoLog_vtx[512], infoLog_frg[512];

    glGetShaderiv(vs, GL_COMPILE_STATUS, &success_vtx);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success_frg);
    
    if(!success_vtx){
        glGetShaderInfoLog(vs, 512, NULL, infoLog_vtx);
        _logger.gl_log("error compiling vertex shader:");
        _logger.gl_log(infoLog_vtx);
    }

    if(!success_frg){
        glGetShaderInfoLog(fs, 512, NULL, infoLog_frg);
        _logger.gl_log("error compiling fragment shader:");
        _logger.gl_log(infoLog_frg);
    }

    _logger.gl_log("creating program");
    this->ID = glCreateProgram();
    glAttachShader(ID, fs);
    glAttachShader(ID, vs);
    glLinkProgram(ID);

    glDeleteShader(vs);
    glDeleteShader(fs);

    int success_program;
    char infolog_program[512];

    glGetProgramiv(ID, GL_LINK_STATUS, &success_program);
    
    if(!success_program){
        glGetProgramPipelineInfoLog(ID, 512, NULL, infolog_program);
        _logger.gl_log("error linking shaders");
        _logger.gl_log(infolog_program);
    }
}

GLuint shader::get_shader_program(){
    return this->ID;
}

void shader::use(){
    glUseProgram(ID);
}