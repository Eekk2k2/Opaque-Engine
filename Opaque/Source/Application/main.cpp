#include "Application.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

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