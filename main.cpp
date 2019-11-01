#include <iostream>
#include "vec3.h"
using namespace std;

int main() {
	int nx = 200;
	int ny = 100;

	freopen("myImage.ppm", "w", stdout);
	
	cout << "P3" << endl;  // The P3 means colors are in ASCII.
	cout << nx << " " << ny << endl;  // numbers of columns and rows
	cout << "255" << endl;  // max color

	for (int j = ny - 1; j >= 0; j--)
		for (int i = 0; i < nx; i++) {
			// float r = float(i) / float(nx);
			// float g = float(j) / float(ny);
			// float b = 0.2;
			vec3 col(float(i) / float(nx), float(j) / float(ny), 0.2);
			int ir = int(255.99 * col[0]);
			int ig = int(255.99 * col[1]);
			int ib = int(255.99 * col[2]);
			cout << ir << " " << ig << " " << ib << endl;
		}

	fclose(stdout);

	return 0;
}