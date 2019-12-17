#include <math.h>

const int Radius = 300;
int X, Y;
double Arg;

void setup()
{
    Arg = 0;
}

void loop()
{
    int newX = Radius + Radius * cos(Arg);
    int newY = Radius - Radius * sin(Arg);

    Mouse.move(newX - X, newY - Y);

    X = newX;
    Y = newY;
    Arg += 0.01;

    delay(10);
}
