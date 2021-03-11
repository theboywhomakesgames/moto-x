#include "common.h"
#include "engine.h"
#include "logger.h"

engine::engine(){

}

engine::~engine(){

}

//private
void engine::getShaders(){
    _logger.gl_log("importing shader files");
    std::ifstream ifs("./Shaders/vertex.glsl");
    std::string content_vtx((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()));
    const char* vertex_char_arr = &content_vtx[0];
    _logger.gl_log("vertex shader");
    _logger.gl_log(vertex_char_arr);
    ifs.close();

    ifs.open("./Shaders/fragment.glsl");
    std::string content_frag((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()));
    const char* frag_char_arr = &content_frag[0];
    _logger.gl_log("fragment shader");
    _logger.gl_log(frag_char_arr);
    ifs.close();

    compileShaders(vertex_char_arr, frag_char_arr);
}

void engine::compileShaders(const char* vertex_char_arr, const char* frag_char_arr){
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
    this->shader_program = glCreateProgram();
    glAttachShader(shader_program, fs);
    glAttachShader(shader_program, vs);
    glLinkProgram(shader_program);

    int success_program;
    char infolog_program[512];

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success_program);
    
    if(!success_program){
        glGetProgramPipelineInfoLog(shader_program, 512, NULL, infolog_program);
        _logger.gl_log("error linking shaders");
        _logger.gl_log(infolog_program);
    }
}

void engine::_update_fps_counter(){
    static double previous_seconds = glfwGetTime();
    static int frame_count = 0;

    double current_seconds = glfwGetTime();
    double elapsed_seconds = current_seconds - previous_seconds;

    if (elapsed_seconds > 1) {
        previous_seconds = current_seconds;
        double fps = (double)frame_count / elapsed_seconds;

        char fps_txt[128];
        sprintf(fps_txt, "%.2f", fps);
        glfwSetWindowTitle(window, fps_txt);

        frame_count = 0;
    }

    frame_count++;
}

// public
bool engine::initialize(logger _logger){
    this->_logger = _logger;

    _logger.gl_log("intializing engine");

    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return false;
    } 

    _logger.gl_log("creating context window");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* video_mode = glfwGetVideoMode(primary_monitor);
    this->window = glfwCreateWindow(300, 200, "Full Screen App", NULL, NULL);

    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    float points[] = {
        0.0f,  0.5f,  0.0f,
        0.0f, 0.0f,  0.0f,
        -0.5f, -0.5f,  0.0f
    };

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

    this->vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    getShaders();

    return true;
}

void engine::run(){
    _logger.gl_log("rendering loop");
    while(!glfwWindowShouldClose(window)) {
        _update_fps_counter();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)){
            glfwSetWindowShouldClose(window, 1);
        }
    }
    
    _logger.gl_log("terminating");
    glfwTerminate();
}