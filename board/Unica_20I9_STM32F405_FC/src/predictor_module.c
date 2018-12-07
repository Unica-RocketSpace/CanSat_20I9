#include <stdio.h>
#include "config.h"
#include <math.h>
#include "state.h"

stateGPS_t stateGPS;
stateIMU_isc_t stateIMU;
//WTF

bool check_tube_target() {
	return 0;
}
void height_predictor() {
	float x = stateGPS.coordinates[0];
	float y = stateGPS.coordinates[1];
	float z = stateGPS.coordinates[2];
	float target_dist = sqrt(x * x + y * y);
	z -= delta_h;
	float distmin, distmax;
	distmin = z / tan(descent_angle_max);
	distmax = z / tan(descent_angle_min);
	if ((target_dist > distmin) && (target_dist < distmax)) {
		straight_flight(atan2(z, target_dist));
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

void direction_predictor() {
	if (check_tube_target()) {
		height_predictor();
	}
	float x = stateGPS.coordinates[0];
	float y = stateGPS.coordinates[1];
	//float z = stateGPS.coordinates[2];
	float vx = stateIMU.velocities[0];
	float vy = stateIMU.velocities[1];
	//float vz = stateIMU.velocities[2];
	float x0 = x + vx * spare_time;
	float y0 = y + vy * spare_time;
	float a, b, c;
	a = y0 - y;
	b = x - x0;
	c = x0 * y - x * y0;
	float centerX, centerY;
	float r = radius_of_turn_circle;
	centerX = - a * r;
	centerY = b * r;
	float target_lenght;
	target_lenght = sqrt(centerX * centerX + centerY * centerY);
	float access_radius;
	access_radius = r + delta_r;
	if (target_lenght >= access_radius) {
		if (c > 0) {
			turn_flight(x0, y0, 'L');
			return;
		} else {
			turn_flight(x0, y0, 'R');
			return;
		}
	} else {
		//дописать передачу ключа поворота L R;
		calculating_distance_of_linear_further_motion(access_radius, r, vx, vy);
		return;
	}
	return;
}

void calculating_distance_of_linear_further_motion(float R, float r, float vx, float vy) {
	float dist;
	dist = sqrt(R * R - (y - r) * (y - r));
	if (x > 0) {
		//дописать х и y для перехода в turn_flight(), с помощью кватарниона
		turn_flight(x + dist + vx * spare_time, );
	}
    return;
}

















