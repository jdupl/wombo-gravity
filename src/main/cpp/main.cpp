#include <stdio.h>
#include <math.h>
#include <iostream>

double dist(double r1[], double r2[]) {
	return sqrt(
		  pow((r1[0] - r2[0]), 2)
		+ pow((r1[1] - r2[1]), 2)
		+ pow((r1[2] - r2[2]), 2)
	);
}

void accel(double m, double r1[], double r2[], double a[]) {
	double distance = pow(dist(r1, r2), 3);
	a[0] = (m/distance) * (r1[0] - r2[0]);
	a[1] = (m/distance) * (r1[1] - r2[1]);
	a[2] = (m/distance) * (r1[2] - r2[2]);
}

int main(int argc, char **argv) {
	double interval = 1;
	double end = 3600 * 24 * 365;
	double G = -6.6738480 * pow(10, -11);
	
	double m[2] = { 1.9891 * pow(10, 30), 5.9736 * pow(10, 24)};
	double v[2][3] = {
			{0, 0, 0},
			{0, 30287, 0}
		};
	double r[2][3] = {
			{0, 0, 0},
			{149597887500, 0, 0}
		};

	long nb = sizeof(m) / sizeof(*m);
	std::cout << nb << '\n';
	
	getchar();
	return 0;
}
