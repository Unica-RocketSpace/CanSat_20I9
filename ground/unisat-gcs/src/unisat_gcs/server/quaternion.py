import math

def quat_mult(quat, k):
    res_quat = [0, 0, 0, 0]
    for i in range(4):
        res_quat[i] = k * quat[i]

    return res_quat


def quat_abs(quat):
    tmp = 0
    for i in range(4):
        tmp += quat[i] ** 2
    return math.sqrt(tmp)


def quat_normalise(quat):
    return quat_mult(quat, 1 / quat_abs(quat))


def quat_invert(quat):
    res_quat = [quat[0], 0, 0, 0]
    for i in range(3):
        res_quat[i+1] = - quat[i+1]
    return quat_normalise(res_quat)


def quat_mult_by_quat(a, b):
    res_quat = [0, 0, 0, 0];

    res_quat[0] = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3]
    res_quat[1] = a[0] * b[1] + a[1] * b[0] + a[2] * b[3] - a[3] * b[2]
    res_quat[2] = a[0] * b[2] - a[1] * b[3] + a[2] * b[0] + a[3] * b[1]
    res_quat[3] = a[0] * b[3] + a[1] * b[2] - a[2] * b[1] + a[3] * b[0]

    return quat_normalise(res_quat)


def quat_mult_by_vect(a, b):
    res_quat = [0, 0, 0, 0]

    vect_quat = [0, b[0], b[1], b[2]]
    return quat_mult_by_quat(a, vect_quat)


def vect_rotate(vect, quat):
    tmp = quat_mult_by_vect(quat, vect)
    quat = quat_invert(quat)

    vect_quat = quat_mult_by_quat(tmp, quat)
    return [vect_quat[1], vect_quat[2], vect_quat[3]]

