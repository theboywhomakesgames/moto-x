#include "common.h"

bool isRunning = true;

int main(int argc, char* argv[])
{
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    } 

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    /* OTHER STUFF GOES HERE NEXT */

    // this is an array of points with 9 floats in it indicating 3 vertices of a triangle
    float points[] = {
        0.0f,  0.5f,  0.0f,
        0.0f, 0.0f,  0.0f,
        -0.5f, -0.5f,  0.0f
    };

    GLuint vbo = 0; // vertex buffer object - I think it's an address
    glGenBuffers(1, &vbo); // generate a buffer and put the address in vbo uint | this buffer is inside graphics card

    cout << vbo << endl; // print the address to see where it is rn

    glBindBuffer(GL_ARRAY_BUFFER, vbo); // set the buffer as the main buffer in gl state (GL_ARRAY_BUFFER)
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW); // specify the targer, buffer size, data, usage | creates and initializes a buffer object's data store

    // Now an unusual step.
    // Most meshes will use a collection of one or more vertex buffer objects to hold vertex points,
    // texture-coordinates, vertex normals, etc.
    // In older GL implementations we would have to bind each one,
    // and define their memory layout, every time that we draw the mesh.
    // To simplify that, we have new thing called the vertex array object (VAO),
    // which remembers all of the vertex buffers that you want to use,
    // and the memory layout of each one. We set up the vertex array object once per mesh.
    // When we want to draw, all we do then is bind the VAO and draw.

    GLuint vao = 0; // an address for vao
    glGenVertexArrays(1, &vao); // generate vertex array on gpu and save the address
    glBindVertexArray(vao); // bind it to bring it to focus in satate machine
    glEnableVertexAttribArray(0); // enable the generic vertex attribute array specified by index = 0 | enabling the first attrib array bcz we're only using one array
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // bind the buffer again to use this
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); // define the layout of the vertex buffer on index 0
    // index, size, type, normalized, stride (byte offset between vertex attributes (if 0 it's tightly packed)), pointer

    // vertex shader as string 
    const char* vertex_shader =
    "#version 400\n"
    "in vec3 vp;"
    "void main() {"
    "  gl_Position = vec4(vp, 1.0);" // gl_Position is a reserved output
    "}";

    // fragment shader as string
    const char* fragment_shader =
    "#version 400\n"
    "out vec4 frag_colour;" 
    "void main() {"
    "  frag_colour = vec4(0.5, 0.0, 0.5, 1.0);"
    "}";

    // load and compile the shaders
    GLuint vs = glCreateShader(GL_VERTEX_SHADER); // create vertex shader and save address
    glShaderSource(vs, 1, &vertex_shader, NULL); // load the string into the GL shader
    glCompileShader(vs); // compile the shader
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER); // create another GL shader
    glShaderSource(fs, 1, &fragment_shader, NULL); // load the frag shader
    glCompileShader(fs); // compile the shader

    // combine the shaders as a GPU shader programme
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    // create a window
    while(!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window
        glUseProgram(shader_programme); // use the shader
        glBindVertexArray(vao); // use VAO not VBO as the input variable for drawing
        glDrawArrays(GL_TRIANGLES, 0, 3); // draw points 0-3 from the currently bound VAO with current in-use shader
        glfwPollEvents(); // update other events like input handling 
        glfwSwapBuffers(window); // put the stuff we've been drawing onto the display
    }
    
    glfwTerminate(); // close GL context and any other GLFW resources
    return 0; 
}