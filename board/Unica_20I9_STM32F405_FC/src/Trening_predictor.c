//#include <bits/stdc++.h>
//
//using namespace std;
//
//#define double float
//#define ui8 uint8_t
//
//const float RADIUS_OF_TURN_CIRCLE = 10.0;
//const float DESCENT_ANGLE_MAX = 60 * M_PI / 180;
//const float DESCENT_ANGLE_MIN = 30 * M_PI / 180;
//const float DELTA_H = 10.0;
//const float DELTA_R = 1.0;
//
//typedef struct {
//    float x, y;
//    float r;
//} Circle_t;
//
//typedef struct {
//    float x, y;
//    float mod;
//} Vector;
//
//const float EPS = 1e-6;
//
//float dist(float x, float y) {
//    return sqrt(x * x + y * y);
//}
//
//float polar_angle(float x, float y) {
//    float p = 2 * M_PI;
//    if (y < 0) {
//        return p - acos(x / sqrt(x * x + y * y));
//    } else {
//        return acos(x / sqrt(x * x + y * y));
//    }
//}
//
//float to_point(float x1, float y1, float x2, float y2, float x, float y, char flag) {
//    float p = 2 * M_PI;
//    float angle_f, angle_s, angle;
//    angle = polar_angle(x, y);
//    angle_f = polar_angle(x1, y1);
//    angle_s = polar_angle(x2, y2);
//    float d_angle_f, d_angle_s;
//    d_angle_f = angle_f - angle;
//    d_angle_s = angle_s - angle;
//    if (abs(d_angle_f) <= EPS) {
//        d_angle_f = p;
//    }
//    if (abs(d_angle_s) <= EPS) {
//        d_angle_s = p;
//    }
//    if (flag == 'L') {
//        if (d_angle_f < 0) {
//            d_angle_f += p;
//        }
//        if (d_angle_s < 0) {
//            d_angle_s += p;
//        }
//        return min(d_angle_f, d_angle_s);
//    } else {
//        if (d_angle_f < 0) {
//            d_angle_f += p;
//        }
//        if (d_angle_s < 0) {
//            d_angle_s += p;
//        }
//        return - min(d_angle_f, d_angle_s);
//    }
//}
//
//float kasat(float x1, float y1, float r1, float xa, float ya, char flag) {
//    double x, y, x2 = 0.0, y2 = 0.0, r2, a = 0, b = 0, c = 0;
//    double d;
//    d = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
//    r2 = sqrt(d * d - r1 * r1);
//
//    if (d == r1) {
//        //1 точка касательной в точке 0, 0;
//        cout << "KEK 0 нам хана" << '\n';
//        return 0.0;
//    }
//    x = x1;
//    y = y1;
//    x1 = 0; y1 = 0;
//    x2 -= x;
//    y2 -= y;
//    a = - 2 * x2;
//    b = - 2 * y2;
//    c = x2 * x2 + y2 * y2 + r1 * r1 - r2 * r2;
//    double x0, y0;
//    x0 = - a * c / (a * a + b * b);
//    y0 = - b * c / (a * a + b * b);
//    d = r1 * r1 - c * c / (a * a + b * b);
//    double mult = sqrt(d / (a * a + b * b));
//    double ax, ay, bx, by;
//    ax = x0 + b * mult;
//    bx = x0 - b * mult;
//    ay = y0 - a * mult;
//    by = y0 + a * mult;
//    // две касательные и нам нужно выбрать
//    float p1x = ax + x;
//    float p1y = ay + y;
//    float p2x = bx + x;
//    float p2y = by + y;
//    cout << "Две точки касания: " << p1x << ", " << p1y << "; " << p2x << ", " << p2y << '\n';
//    return to_point(p1x, p1y, p2x, p2y, xa, ya, flag);
//}
//
//
//void straight_flight(float alpha) {
//    //set angle of incidence equal to alpha
//    cout << "Летим прямо" << '\n';
//    return;
//}
//
//void turn_flight(float xc, float yc,float r, float x, float y, char turn) {
//    double beta = kasat(xc, yc, r, x, y, turn);
//    cout << "Поворот на " << beta * 180 / M_PI << turn << '\n';
//}
//
//bool check_tube_target(float a, float b, float c, float x, float y) {
//    double dist = abs(c) / sqrt(a * a + b * b);
//    double target_dist = sqrt(x * x + y * y);
//    if (asin(dist / target_dist) < 0.17) {
//        return true;
//    }
//    return false;
//}
//
//void height_predictor(float x, float y, float z) {
//    float target_dist = sqrt(y * y + x * x);
//    z -= DELTA_H;
//    float distmin, distmax;
//    distmin = z / tan(DESCENT_ANGLE_MAX);
//    distmax = z / tan(DESCENT_ANGLE_MIN);
//    if ((target_dist > distmin) && (target_dist < distmax)) {
//        float good_angle = atan2(z, target_dist);
//        cout << "Угол снижения: " << good_angle * 180 / M_PI << '\n';
//        straight_flight(good_angle);
//        return;
//    }
//    if (target_dist <= distmin) {
//        cout << "Угол снижения: " << DESCENT_ANGLE_MAX * 180 / M_PI << '\n';
//        straight_flight(DESCENT_ANGLE_MAX);
//        return;
//    }
//    if (target_dist >= distmax) {
//        cout << "Угол снижения: " << DESCENT_ANGLE_MIN * 180 / M_PI << '\n';
//        straight_flight(DESCENT_ANGLE_MIN);
//        return;
//    }
//    return;
//}
//
//void direction_predictor() {
//    float x, y, z;
//    cin >> x >> y >> z;
//    float vx, vy;
//    float course;
//    cin >> vx >> vy;
//    float x_next, y_next;
//    x_next = x + vx;
//    y_next = y + vy;
//    float a, b, c;
//    a = y_next - y;
//    b = x - x_next;
//    c = x_next * y - x * y_next;
//    bool flag = 0;
//    if (dist(x_next, y_next) <= dist(x, y)) {
//        if (check_tube_target(a, b, c, x, y)) {
//            cout << " Цель в трубке." << '\n';
//            height_predictor(x, y, z);
//            return;
//        }
//    }
//    Circle_t left_turn_circle;
//    Circle_t right_turn_circle;
//    Circle_t left_access_circle;
//    Circle_t right_access_circle;
//    left_turn_circle.r = RADIUS_OF_TURN_CIRCLE;
//    right_turn_circle.r = RADIUS_OF_TURN_CIRCLE;
//    left_access_circle.r = RADIUS_OF_TURN_CIRCLE + DELTA_R;
//    right_access_circle.r = RADIUS_OF_TURN_CIRCLE + DELTA_R;
//    Vector vex;
//    vex.x = x_next - x;
//    vex.y = y_next - y;
//    vex.mod = sqrt(vex.x * vex.x + vex.y * vex.y);
//    float temp = vex.x;
//    vex.x = -vex.y;
//    vex.y = temp;
//    vex.x /= vex.mod;
//    vex.y /= vex.mod;
//    vex.x *= RADIUS_OF_TURN_CIRCLE;
//    vex.y *= RADIUS_OF_TURN_CIRCLE;
//    left_turn_circle.x = x + vex.x;
//    left_access_circle.x = x + vex.x;
//    left_turn_circle.y = y + vex.y;
//    left_access_circle.y = y + vex.y;
//    vex.x *= -1.0;
//    vex.y *= -1.0;
//    right_turn_circle.x = x + vex.x;
//    right_access_circle.x = x + vex.x;
//    right_turn_circle.y = y + vex.y;
//    right_access_circle.y = y + vex.y;
//    float left_target_dist = sqrt(left_access_circle.x * left_access_circle.x + left_access_circle.y * left_access_circle.y);
//    float right_target_dist = sqrt(right_access_circle.x * right_access_circle.x + right_access_circle.y * right_access_circle.y);
//    cout << "Левая окружность: " << left_turn_circle.x << ", " << left_turn_circle.y << '\n';
//    cout << "Правая окружность: " << right_turn_circle.x << ", " << right_turn_circle.y << '\n';
//    char turn = 'R';
//    if (left_target_dist < right_target_dist) {
//        turn = 'L';
//
//    }
//    if (left_target_dist - EPS > left_access_circle.r) {
//        if (right_target_dist - EPS > right_access_circle.r) {
//            if (turn == 'L') {
//                cout << "Поворот налево." << '\n';
//                turn_flight(left_turn_circle.x, left_turn_circle.y, left_turn_circle.r, x, y, turn);
//                return;
//            }
//            cout << "Поворот направо." << '\n';
//            turn_flight(right_turn_circle.x, right_turn_circle.y, right_turn_circle.r, x, y, turn);
//            return;
//        }
//    }
//    straight_flight(DESCENT_ANGLE_MIN);
//    cout << "Цель недоступна." << '\n';
//    return;
//}
//
//signed main() {
//    direction_predictor();
//    return 0;
//}
