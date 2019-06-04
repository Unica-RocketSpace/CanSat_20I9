#include <stdio.h>
#include "config.h"
#include <math.h>
#include "state.h"
#include "quaternion.h"
#include <stdbool.h>

//#include <vector.h>

stateGPS_t stateGPS;
stateIMU_isc_t stateIMU;
//WTF

void straight_flight(double alpha) {
	//set angle of incidence equal to alpha
	return;
}

bool check_tube_target(double a, double b, double c, double z, double x, bool flag) {
	double dist = abs(c) / sqrt(a * a + b * b);
	double target_dist = sqrt(z * z + x * x);
	if ((asin(dist / target_dist) < 0.17) && (flag)) {
		return true;
	}
	return false;
}
void height_predictor(double x, double y, double z) {
	double target_dist = sqrt(z * z + x * x);
	y -= delta_h;
	double distmin, distmax;
	distmin = y / tan(descent_angle_max);
	distmax = y / tan(descent_angle_min);
	if ((target_dist > distmin) && (target_dist < distmax)) {
		double good_angle = atan2(y, target_dist);
		straight_flight(good_angle);
		return;
	}
	if (target_dist <= distmin) {
		straight_flight(descent_angle_max);
		return;
	}
	if (target_dist >= distmax) {
		straight_flight(descent_angle_min);
		return;
	}
	return;
}

void turn_flight(double z, double x, char turn) {
	if (turn == 'L') {
		//do rotation using the left turn circle
	} else if (turn == 'R') {
		//do rotation using the right turn circle
	}
}

void direction_predictor() {
	char turn;
	double x, y, z;
	double coordinates_SC[3] = {x, y, z};
	double vz, vx;
	double z0 = z + vz * spare_time;
	double x0 = x + vx * spare_time;
	double v = sqrt(vx * vx + vz * vz);
	double a, b, c;
	a = x0 - x;
	b = z - z0;
	c = z0 * x - z * x0;
	bool flag = 1;
	if (sqrt(z0 * z0 + x0 * x0) > sqrt(z * z + x * x)) {
		flag = 0;
	}
	if (check_tube_target(a, b, c, z, x, flag)) {
		height_predictor(x, y, z);
		return;
	}
	double center_left[3], center_right[3];
	double r = radius_of_turn_circle;
	double first_vector[3] = {0, 1, 0};
	double second_vector[3] = {vx, 0, vz};
	double temporary_vector[3];
	iauPxp(first_vector, second_vector, center_left);
	iauPxp(second_vector, first_vector, center_right);
	iauSxp(r / v, center_left, temporary_vector);
	iauPpp(temporary_vector, coordinates_SC, center_left);
	iauSxp(r / v, center_right, temporary_vector);
	iauPpp(temporary_vector, coordinates_SC, center_right);
	double target_length_left, target_length_right;
	double access_radius;
	access_radius = r + delta_r;
	target_length_left = sqrt(center_left[0] * center_left[0] + center_left[2] * center_left[2]);
	target_length_right = sqrt(center_right[0] * center_right[0] + center_right[2] * center_right[2]);
	if (((c > 0) && (flag)) || ((c < 0) && (!flag))) {
		turn = 'L';
	} else if (((c < 0) && (flag)) || ((c > 0) && (!flag))){
		turn = 'R';
	}
	if ((target_length_left > access_radius) && (target_length_right > access_radius)) {
		turn_flight(z0, x0, turn);
		return;
	} else {
		calculating_distance_of_linear_further_motion(access_radius, r, vz, vx, turn);
		return;
	}
	return;
}

void calculating_distance_of_linear_further_motion(double R, double r, double vz, double vx, char turn) {
	double dist;
	double coordinates[3] = {0};
	double new_coordinates[3] = {0};
	double quat[4] = {0, 0, 0, 0};
	quat_invert(stateIMU_isc.quaternion, quat);
	vect_rotate(stateGPS.coordinates, quat, coordinates);
	double z = coordinates[0];
	double x = coordinates[1];
	double y = coordinates[2];
	dist = sqrt(R * R - (z - r) * (z - r));
	z += dist + vz * spare_time;
	vect_rotate(coordinates, stateIMU_isc.quaternion, new_coordinates);
	turn_flight(new_coordinates[0], new_coordinates[1], turn);
    return;
}