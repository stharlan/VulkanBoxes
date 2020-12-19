
#include "voxc.h"

// https://lodev.org/cgtutor/randomnoise.html

#define noiseWidth 512
#define noiseHeight 512

double noise[noiseHeight][noiseWidth]; //the noise array

void generateNoise()
{
    srand(clock());
    for (int y = 0; y < noiseHeight; y++)
        for (int x = 0; x < noiseWidth; x++)
        {
            noise[y][x] = (rand() % 32768) / 32768.0;
        }
}

double smoothNoise(double x, double y)
{
    //get fractional part of x and y
    double fractX = x - int(x);
    double fractY = y - int(y);

    //wrap around
    int x1 = (int(x) + noiseWidth) % noiseWidth;
    int y1 = (int(y) + noiseHeight) % noiseHeight;

    //neighbor values
    int x2 = (x1 + noiseWidth - 1) % noiseWidth;
    int y2 = (y1 + noiseHeight - 1) % noiseHeight;

    //smooth the noise with bilinear interpolation
    double value = 0.0;
    value += fractX * fractY * noise[y1][x1];
    value += (1 - fractX) * fractY * noise[y1][x2];
    value += fractX * (1 - fractY) * noise[y2][x1];
    value += (1 - fractX) * (1 - fractY) * noise[y2][x2];

    return value;
}

double turbulence(double x, double y, double size)
{
    double value = 0.0, initialSize = size;

    while (size >= 1)
    {
        value += smoothNoise(x / size, y / size) * size;
        size /= 2.0;
    }

    return(128.0 * value / initialSize);
}

void generate_terrain(BYTE* pixels)
{
    generateNoise();
    for (int y = 0; y < 512; y++)
        for (int x = 0; x < 512; x++)
        {
            pixels[(y * 512) + x] = (BYTE)(turbulence(x, y, 512));
        }
}
