#include "../common.h"
#include "../Logger/logger.h"
#include "texture.h"
#include "stb_image.h"

texture::texture(const char* address, logger engine_logger){
    this->_logger = engine_logger;

    // creating a texture
    _logger.gl_log("");
    _logger.gl_log("importing texture...");
    _logger.gl_log("");
    
    data = stbi_load(address, &width, &height, &nrChannels, 0);
    glGenTextures(1, &textureID);

}