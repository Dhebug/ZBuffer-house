#include <iostream.h>
#include <conio.h>


void main()
{
	long x=0x12345678;
	int* y;
	y=(int*)&x;

	cout << x << " " << int(x>>16) << "\n";

	cout <<*(y+1) << "\n";


	getch();
}