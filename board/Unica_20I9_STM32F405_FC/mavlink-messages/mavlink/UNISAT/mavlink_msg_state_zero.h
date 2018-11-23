#pragma once
// MESSAGE STATE_ZERO PACKING

#define MAVLINK_MSG_ID_STATE_ZERO 156

MAVPACKED(
typedef struct __mavlink_state_zero_t {
 float zero_pressure; /*< pressure at zero level (on the earth)*/
 float zero_quaternion[4]; /*< quaternion at zero level (on the earth)*/
 float zero_GPS[3]; /*< GPS coordinates at zero level (on the earth)*/
 float gyro_staticShift[3]; /*< Static shift of gyro*/
 float accel_staticShift[3]; /*< Static shift of accel*/
 float time; /*< current time*/
}) mavlink_state_zero_t;

#define MAVLINK_MSG_ID_STATE_ZERO_LEN 60
#define MAVLINK_MSG_ID_STATE_ZERO_MIN_LEN 60
#define MAVLINK_MSG_ID_156_LEN 60
#define MAVLINK_MSG_ID_156_MIN_LEN 60

#define MAVLINK_MSG_ID_STATE_ZERO_CRC 188
#define MAVLINK_MSG_ID_156_CRC 188

#define MAVLINK_MSG_STATE_ZERO_FIELD_ZERO_QUATERNION_LEN 4
#define MAVLINK_MSG_STATE_ZERO_FIELD_ZERO_GPS_LEN 3
#define MAVLINK_MSG_STATE_ZERO_FIELD_GYRO_STATICSHIFT_LEN 3
#define MAVLINK_MSG_STATE_ZERO_FIELD_ACCEL_STATICSHIFT_LEN 3

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_STATE_ZERO { \
    156, \
    "STATE_ZERO", \
    6, \
    {  { "zero_pressure", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_state_zero_t, zero_pressure) }, \
         { "zero_quaternion", NULL, MAVLINK_TYPE_FLOAT, 4, 4, offsetof(mavlink_state_zero_t, zero_quaternion) }, \
         { "zero_GPS", NULL, MAVLINK_TYPE_FLOAT, 3, 20, offsetof(mavlink_state_zero_t, zero_GPS) }, \
         { "gyro_staticShift", NULL, MAVLINK_TYPE_FLOAT, 3, 32, offsetof(mavlink_state_zero_t, gyro_staticShift) }, \
         { "accel_staticShift", NULL, MAVLINK_TYPE_FLOAT, 3, 44, offsetof(mavlink_state_zero_t, accel_staticShift) }, \
         { "time", NULL, MAVLINK_TYPE_FLOAT, 0, 56, offsetof(mavlink_state_zero_t, time) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_STATE_ZERO { \
    "STATE_ZERO", \
    6, \
    {  { "zero_pressure", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_state_zero_t, zero_pressure) }, \
         { "zero_quaternion", NULL, MAVLINK_TYPE_FLOAT, 4, 4, offsetof(mavlink_state_zero_t, zero_quaternion) }, \
         { "zero_GPS", NULL, MAVLINK_TYPE_FLOAT, 3, 20, offsetof(mavlink_state_zero_t, zero_GPS) }, \
         { "gyro_staticShift", NULL, MAVLINK_TYPE_FLOAT, 3, 32, offsetof(mavlink_state_zero_t, gyro_staticShift) }, \
         { "accel_staticShift", NULL, MAVLINK_TYPE_FLOAT, 3, 44, offsetof(mavlink_state_zero_t, accel_staticShift) }, \
         { "time", NULL, MAVLINK_TYPE_FLOAT, 0, 56, offsetof(mavlink_state_zero_t, time) }, \
         } \
}
#endif

/**
 * @brief Pack a state_zero message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param zero_pressure pressure at zero level (on the earth)
 * @param zero_quaternion quaternion at zero level (on the earth)
 * @param zero_GPS GPS coordinates at zero level (on the earth)
 * @param gyro_staticShift Static shift of gyro
 * @param accel_staticShift Static shift of accel
 * @param time current time
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_state_zero_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float zero_pressure, const float *zero_quaternion, const float *zero_GPS, const float *gyro_staticShift, const float *accel_staticShift, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_STATE_ZERO_LEN];
    _mav_put_float(buf, 0, zero_pressure);
    _mav_put_float(buf, 56, time);
    _mav_put_float_array(buf, 4, zero_quaternion, 4);
    _mav_put_float_array(buf, 20, zero_GPS, 3);
    _mav_put_float_array(buf, 32, gyro_staticShift, 3);
    _mav_put_float_array(buf, 44, accel_staticShift, 3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_STATE_ZERO_LEN);
#else
    mavlink_state_zero_t packet;
    packet.zero_pressure = zero_pressure;
    packet.time = time;
    mav_array_memcpy(packet.zero_quaternion, zero_quaternion, sizeof(float)*4);
    mav_array_memcpy(packet.zero_GPS, zero_GPS, sizeof(float)*3);
    mav_array_memcpy(packet.gyro_staticShift, gyro_staticShift, sizeof(float)*3);
    mav_array_memcpy(packet.accel_staticShift, accel_staticShift, sizeof(float)*3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_STATE_ZERO_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_STATE_ZERO;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_STATE_ZERO_MIN_LEN, MAVLINK_MSG_ID_STATE_ZERO_LEN, MAVLINK_MSG_ID_STATE_ZERO_CRC);
}

/**
 * @brief Pack a state_zero message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param zero_pressure pressure at zero level (on the earth)
 * @param zero_quaternion quaternion at zero level (on the earth)
 * @param zero_GPS GPS coordinates at zero level (on the earth)
 * @param gyro_staticShift Static shift of gyro
 * @param accel_staticShift Static shift of accel
 * @param time current time
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_state_zero_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float zero_pressure,const float *zero_quaternion,const float *zero_GPS,const float *gyro_staticShift,const float *accel_staticShift,float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_STATE_ZERO_LEN];
    _mav_put_float(buf, 0, zero_pressure);
    _mav_put_float(buf, 56, time);
    _mav_put_float_array(buf, 4, zero_quaternion, 4);
    _mav_put_float_array(buf, 20, zero_GPS, 3);
    _mav_put_float_array(buf, 32, gyro_staticShift, 3);
    _mav_put_float_array(buf, 44, accel_staticShift, 3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_STATE_ZERO_LEN);
#else
    mavlink_state_zero_t packet;
    packet.zero_pressure = zero_pressure;
    packet.time = time;
    mav_array_memcpy(packet.zero_quaternion, zero_quaternion, sizeof(float)*4);
    mav_array_memcpy(packet.zero_GPS, zero_GPS, sizeof(float)*3);
    mav_array_memcpy(packet.gyro_staticShift, gyro_staticShift, sizeof(float)*3);
    mav_array_memcpy(packet.accel_staticShift, accel_staticShift, sizeof(float)*3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_STATE_ZERO_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_STATE_ZERO;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_STATE_ZERO_MIN_LEN, MAVLINK_MSG_ID_STATE_ZERO_LEN, MAVLINK_MSG_ID_STATE_ZERO_CRC);
}

/**
 * @brief Encode a state_zero struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param state_zero C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_state_zero_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_state_zero_t* state_zero)
{
    return mavlink_msg_state_zero_pack(system_id, component_id, msg, state_zero->zero_pressure, state_zero->zero_quaternion, state_zero->zero_GPS, state_zero->gyro_staticShift, state_zero->accel_staticShift, state_zero->time);
}

/**
 * @brief Encode a state_zero struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param state_zero C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_state_zero_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_state_zero_t* state_zero)
{
    return mavlink_msg_state_zero_pack_chan(system_id, component_id, chan, msg, state_zero->zero_pressure, state_zero->zero_quaternion, state_zero->zero_GPS, state_zero->gyro_staticShift, state_zero->accel_staticShift, state_zero->time);
}

/**
 * @brief Send a state_zero message
 * @param chan MAVLink channel to send the message
 *
 * @param zero_pressure pressure at zero level (on the earth)
 * @param zero_quaternion quaternion at zero level (on the earth)
 * @param zero_GPS GPS coordinates at zero level (on the earth)
 * @param gyro_staticShift Static shift of gyro
 * @param accel_staticShift Static shift of accel
 * @param time current time
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_state_zero_send(mavlink_channel_t chan, float zero_pressure, const float *zero_quaternion, const float *zero_GPS, const float *gyro_staticShift, const float *accel_staticShift, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_STATE_ZERO_LEN];
    _mav_put_float(buf, 0, zero_pressure);
    _mav_put_float(buf, 56, time);
    _mav_put_float_array(buf, 4, zero_quaternion, 4);
    _mav_put_float_array(buf, 20, zero_GPS, 3);
    _mav_put_float_array(buf, 32, gyro_staticShift, 3);
    _mav_put_float_array(buf, 44, accel_staticShift, 3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_STATE_ZERO, buf, MAVLINK_MSG_ID_STATE_ZERO_MIN_LEN, MAVLINK_MSG_ID_STATE_ZERO_LEN, MAVLINK_MSG_ID_STATE_ZERO_CRC);
#else
    mavlink_state_zero_t packet;
    packet.zero_pressure = zero_pressure;
    packet.time = time;
    mav_array_memcpy(packet.zero_quaternion, zero_quaternion, sizeof(float)*4);
    mav_array_memcpy(packet.zero_GPS, zero_GPS, sizeof(float)*3);
    mav_array_memcpy(packet.gyro_staticShift, gyro_staticShift, sizeof(float)*3);
    mav_array_memcpy(packet.accel_staticShift, accel_staticShift, sizeof(float)*3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_STATE_ZERO, (const char *)&packet, MAVLINK_MSG_ID_STATE_ZERO_MIN_LEN, MAVLINK_MSG_ID_STATE_ZERO_LEN, MAVLINK_MSG_ID_STATE_ZERO_CRC);
#endif
}

/**
 * @brief Send a state_zero message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_state_zero_send_struct(mavlink_channel_t chan, const mavlink_state_zero_t* state_zero)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_state_zero_send(chan, state_zero->zero_pressure, state_zero->zero_quaternion, state_zero->zero_GPS, state_zero->gyro_staticShift, state_zero->accel_staticShift, state_zero->time);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_STATE_ZERO, (const char *)state_zero, MAVLINK_MSG_ID_STATE_ZERO_MIN_LEN, MAVLINK_MSG_ID_STATE_ZERO_LEN, MAVLINK_MSG_ID_STATE_ZERO_CRC);
#endif
}

#if MAVLINK_MSG_ID_STATE_ZERO_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_state_zero_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float zero_pressure, const float *zero_quaternion, const float *zero_GPS, const float *gyro_staticShift, const float *accel_staticShift, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, zero_pressure);
    _mav_put_float(buf, 56, time);
    _mav_put_float_array(buf, 4, zero_quaternion, 4);
    _mav_put_float_array(buf, 20, zero_GPS, 3);
    _mav_put_float_array(buf, 32, gyro_staticShift, 3);
    _mav_put_float_array(buf, 44, accel_staticShift, 3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_STATE_ZERO, buf, MAVLINK_MSG_ID_STATE_ZERO_MIN_LEN, MAVLINK_MSG_ID_STATE_ZERO_LEN, MAVLINK_MSG_ID_STATE_ZERO_CRC);
#else
    mavlink_state_zero_t *packet = (mavlink_state_zero_t *)msgbuf;
    packet->zero_pressure = zero_pressure;
    packet->time = time;
    mav_array_memcpy(packet->zero_quaternion, zero_quaternion, sizeof(float)*4);
    mav_array_memcpy(packet->zero_GPS, zero_GPS, sizeof(float)*3);
    mav_array_memcpy(packet->gyro_staticShift, gyro_staticShift, sizeof(float)*3);
    mav_array_memcpy(packet->accel_staticShift, accel_staticShift, sizeof(float)*3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_STATE_ZERO, (const char *)packet, MAVLINK_MSG_ID_STATE_ZERO_MIN_LEN, MAVLINK_MSG_ID_STATE_ZERO_LEN, MAVLINK_MSG_ID_STATE_ZERO_CRC);
#endif
}
#endif

#endif

// MESSAGE STATE_ZERO UNPACKING


/**
 * @brief Get field zero_pressure from state_zero message
 *
 * @return pressure at zero level (on the earth)
 */
static inline float mavlink_msg_state_zero_get_zero_pressure(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field zero_quaternion from state_zero message
 *
 * @return quaternion at zero level (on the earth)
 */
static inline uint16_t mavlink_msg_state_zero_get_zero_quaternion(const mavlink_message_t* msg, float *zero_quaternion)
{
    return _MAV_RETURN_float_array(msg, zero_quaternion, 4,  4);
}

/**
 * @brief Get field zero_GPS from state_zero message
 *
 * @return GPS coordinates at zero level (on the earth)
 */
static inline uint16_t mavlink_msg_state_zero_get_zero_GPS(const mavlink_message_t* msg, float *zero_GPS)
{
    return _MAV_RETURN_float_array(msg, zero_GPS, 3,  20);
}

/**
 * @brief Get field gyro_staticShift from state_zero message
 *
 * @return Static shift of gyro
 */
static inline uint16_t mavlink_msg_state_zero_get_gyro_staticShift(const mavlink_message_t* msg, float *gyro_staticShift)
{
    return _MAV_RETURN_float_array(msg, gyro_staticShift, 3,  32);
}

/**
 * @brief Get field accel_staticShift from state_zero message
 *
 * @return Static shift of accel
 */
static inline uint16_t mavlink_msg_state_zero_get_accel_staticShift(const mavlink_message_t* msg, float *accel_staticShift)
{
    return _MAV_RETURN_float_array(msg, accel_staticShift, 3,  44);
}

/**
 * @brief Get field time from state_zero message
 *
 * @return current time
 */
static inline float mavlink_msg_state_zero_get_time(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  56);
}

/**
 * @brief Decode a state_zero message into a struct
 *
 * @param msg The message to decode
 * @param state_zero C-struct to decode the message contents into
 */
static inline void mavlink_msg_state_zero_decode(const mavlink_message_t* msg, mavlink_state_zero_t* state_zero)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    state_zero->zero_pressure = mavlink_msg_state_zero_get_zero_pressure(msg);
    mavlink_msg_state_zero_get_zero_quaternion(msg, state_zero->zero_quaternion);
    mavlink_msg_state_zero_get_zero_GPS(msg, state_zero->zero_GPS);
    mavlink_msg_state_zero_get_gyro_staticShift(msg, state_zero->gyro_staticShift);
    mavlink_msg_state_zero_get_accel_staticShift(msg, state_zero->accel_staticShift);
    state_zero->time = mavlink_msg_state_zero_get_time(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_STATE_ZERO_LEN? msg->len : MAVLINK_MSG_ID_STATE_ZERO_LEN;
        memset(state_zero, 0, MAVLINK_MSG_ID_STATE_ZERO_LEN);
    memcpy(state_zero, _MAV_PAYLOAD(msg), len);
#endif
}
