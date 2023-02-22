#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cstring>

#include "Application.h"


int main()
{
    // Creates the application to run different things on it.
    Application applcation = Application();

    try
    {
        applcation.Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}