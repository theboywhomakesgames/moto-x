#ifndef MOTOX_SHADERS
#define MOTOX_SHADERS

#include "../common.h"
#include "../Logger/logger.h"

class shader{
    private:
        const char* vertex_char_arr;
        const char* frag_char_arr;

        const char* vertex_path;
        const char* fragment_path;

        logger _logger;

    public:
        GLuint ID;

        shader(const char* vertex_path, const char* fragment_path, logger _logger);

        void use();

        void setBool(const string &name, bool value) const;
        void setInt(const string &name, int value) const;
        void setFloat(const string &name, float value) const;

        void get_shaders();
        void compile_shaders();

        GLuint get_shader_program();
};

#endif