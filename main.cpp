#include <iostream>
#include <cstdlib>
#include <ctime>

struct Vec {
	double x;
	double y;
};

int main() {
	srand(time(0));

	Vec loc;
	loc.x = rand();
	loc.y = rand();
}
