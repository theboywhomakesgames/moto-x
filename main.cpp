#include "common.h"
#include "logger.h"
#include "engine.h"

bool isRunning = true;

int main(int argc, char* argv[])
{
    logger _logger = logger();
    _logger.restart_gl_log();
    
    engine e = engine();
    e.initialize(_logger);
    e.run();

    return 0;
}