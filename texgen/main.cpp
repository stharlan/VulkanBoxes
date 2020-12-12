
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stdio.h>
#include <string>
#include <random>
#include "stb_image.h"
#include "stb_image_write.h"

#define noiseWidth 64
#define noiseHeight 64

double noise[noiseHeight][noiseWidth]; //the noise array

void generateNoise()
{
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

void make_wood_rings()
{
    generateNoise();

    double xyPeriod = 8.0; //number of rings
    double turbPower = 0.2; //makes twists
    double turbSize = 32.0; //initial size of the turbulence

    uint8_t* pixels = (uint8_t*)malloc(64 * 64 * 4);
    memset(pixels, 0, 64 * 64 * 4);

    for (int y = 0; y < 64; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            double xValue = (x - noiseWidth / 2) / double(noiseWidth);
            double yValue = (y - noiseHeight / 2) / double(noiseHeight);
            double distValue = sqrt(xValue * xValue + yValue * yValue) + turbPower * turbulence(x, y, turbSize) / 256.0;
            double sineValue = 128.0 * fabs(sin(2 * xyPeriod * distValue * 3.14159));
            // rgba
            pixels[(y * 64 * 4) + (x * 4)] = 80 + sineValue;
            pixels[(y * 64 * 4) + (x * 4) + 1] = 30 + sineValue;
            pixels[(y * 64 * 4) + (x * 4) + 2] = 30;
            pixels[(y * 64 * 4) + (x * 4) + 3] = 0xff;
        }
    }

    stbi_write_png("output.png", 64, 64, 4, pixels, 64 * 4);
    free(pixels);

}

void make_wood_bark()
{
    generateNoise();

    //xPeriod and yPeriod together define the angle of the lines
    //xPeriod and yPeriod both 0 ==> it becomes a normal clouds or turbulence pattern
    double xPeriod = 10.0; //defines repetition of marble lines in x direction
    double yPeriod = 10.0; //defines repetition of marble lines in y direction
    //turbPower = 0 ==> it becomes a normal sine pattern
    double turbPower = 2.5; //makes twists
    double turbSize = 32.0; //initial size of the turbulence

    uint8_t* pixels = (uint8_t*)malloc(64 * 64 * 4);
    memset(pixels, 0, 64 * 64 * 4);

    for (int y = 0; y < 64; y++)
    {
        for (int x = 0; x < 64; x++)
        {
            //double xyValue = x * xPeriod / noiseWidth + y * yPeriod / noiseHeight + turbPower * turbulence(x, y, turbSize) / 256.0;
            double xyValue = x * xPeriod / noiseWidth + turbPower * turbulence(x, y, turbSize) / 256.0;
            double sineValue = 128 * fabs(sin(xyValue * 3.14159));
            //color.r = color.g = color.b = Uint8(sineValue);
            //pset(x, y, color);
            // rgba
            pixels[(y * 64 * 4) + (x * 4)] = 30 + sineValue;
            pixels[(y * 64 * 4) + (x * 4) + 1] = sineValue;
            pixels[(y * 64 * 4) + (x * 4) + 2] = 0;
            pixels[(y * 64 * 4) + (x * 4) + 3] = 0xff;
        }

    }

    stbi_write_png("output.png", 64, 64, 4, pixels, 64 * 4);
    free(pixels);

}

int main(int argc, char* argv[])
{
    make_wood_rings();
    //make_wood_bark();
    return 0;
}