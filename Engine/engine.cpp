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

    _logger.gl_log("creating program");
    this->shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);
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
    this->window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    
    _logger.gl_log("terminating");
    glfwTerminate();
}