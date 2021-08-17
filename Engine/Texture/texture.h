#ifndef MOTOX_TEXTURE
#define MOTOX_TEXTURE

#include "../common.h"
#include "../Logger/logger.h"

class texture{
    private:
        logger _logger;

    public:
        unsigned int textureID;
        unsigned char *data;
        int width, height, nrChannels;

        texture(const char* address, logger engine_logger);
};

#endif