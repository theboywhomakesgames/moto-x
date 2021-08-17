#include "common.h"

#include "engine.h"
#include "logger.h"
#include "Shader/shader.h"
#include "Texture/texture.h"

engine::engine(){

}

engine::~engine(){

}

//private
void engine::getShaders(){
    const char vertex_path[] = "./Shaders/vertex.glsl";
    const char fragment_path[] = "./Shaders/fragment.glsl";

    shader main_shader = shader(&vertex_path[0], &fragment_path[0], this->engine_logger);
    main_shader.get_shaders();

    shaders.push_back(main_shader);
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
    this->engine_logger = _logger;

    engine_logger.gl_log("intializing engine");

    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return false;
    } 

    engine_logger.gl_log("creating context window");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* video_mode = glfwGetVideoMode(primary_monitor);
    this->window = glfwCreateWindow(800, 600, "Full Screen App", NULL, NULL);

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
        // points                   // colors                   // texture coords
        0.5f,   0.5f,   0.0f,       1.0f,   0.0f,   0.0f,       1.0f, 1.0f,
        0.5f,   -0.5f,  0.0f,       0.0f,   1.0f,   0.0f,       1.0f, 0.0f,
        -0.5f,  -0.5f,  0.0f,       0.0f,   0.0f,   1.0f,       0.0f, 0.0f,
        -0.5f,  0.5f,   0.0f,       1.0f,   1.0f,   0.0f,       0.0f, 1.0f
    };

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    
    // // creating a texture
    // int width, height, nrChannels;
    // unsigned char *data = stbi_load("./Resource/Images/container.jpg", &width, &height, &nrChannels, 0);
    // unsigned int textureID;
    // glGenTextures(1, &textureID);

    texture st = texture("./Resource/Images/container.jpg", engine_logger);
    
    glActiveTexture(GL_TEXTURE0); // activate texture0 unit
    glBindTexture(GL_TEXTURE_2D, st.textureID); // bind texture to the last active unit

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, st.width, st.height, 0, GL_RGB, GL_UNSIGNED_BYTE, st.data);
    glGenerateMipmap(GL_TEXTURE_2D);

    this->vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    // points attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);

    // colors attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));

    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // ===============================================================================
    // ===============================================================================
    // The last element buffer object that gets bound while
    // a VAO is bound, is stored as the VAO's element buffer object.
    // Binding to a VAO then also automatically binds that EBO.
    // ===============================================================================
    // ===============================================================================
    // A VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
    // This also means it stores its unbind calls so make sure you
    // don't unbind the element array buffer before unbinding your VAO,
    // otherwise it doesn't have an EBO configured.
    // ===============================================================================
    // ===============================================================================

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    getShaders();

    return true;
}

void engine::run(){
    engine_logger.gl_log("rendering loop");

    while(!glfwWindowShouldClose(window)) {
        _update_fps_counter();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaders[0].use();

        glBindVertexArray(vao);

        // To draw your triangles in wireframe mode,
        // you can configure how OpenGL draws its primitives
        // via glPolygonMode(GL_FRONT_AND_BACK, GL_LINE).
        // The first argument says we want to apply it to the
        // front and back of all triangles and the second line
        // tells us to draw them as lines. Any subsequent drawing
        // calls will render the triangles in wireframe mode until
        // we set it back to its default using glPolygonMode(GL_FRONT_AND_BACK, GL_FILL).
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwPollEvents();
        glfwSwapBuffers(window);
        if(GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)){
            glfwSetWindowShouldClose(window, 1);
        }
    }
    
    engine_logger.gl_log("terminating");
    glfwTerminate();
}