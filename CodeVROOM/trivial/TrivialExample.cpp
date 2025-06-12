#include <stdio.h>

struct Coordinates
{
	int X, Y;
};

struct Size
{
	int Width, Height;
};

struct Rectangle
{
	Coordinates TopLeft;
	struct Size Size;
};

int main()
{
	Rectangle rect = {{10, 20}, {100, 200}};
	
	return 0;
}