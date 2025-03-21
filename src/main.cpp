#include "Raytracer.h"

#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[])
{
    srand(time(nullptr));

    Raytracer raytracer;
    raytracer.run();

    return 0;
}
