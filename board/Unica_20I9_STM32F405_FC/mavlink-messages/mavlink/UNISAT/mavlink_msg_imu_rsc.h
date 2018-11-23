#pragma once
// MESSAGE IMU_rsc PACKING

#define MAVLINK_MSG_ID_IMU_rsc 151

MAVPACKED(
typedef struct __mavlink_imu_rsc_t {
 float accel[3]; /*< accelerations*/
 float gyro[3]; /*< angle velocities*/
 float compass[3]; /*< compass (magnetometer) vector*/
 float time; /*< current time*/
}) mavlink_imu_rsc_t;

#define MAVLINK_MSG_ID_IMU_rsc_LEN 40
#define MAVLINK_MSG_ID_IMU_rsc_MIN_LEN 40
#define MAVLINK_MSG_ID_151_LEN 40
#define MAVLINK_MSG_ID_151_MIN_LEN 40

#define MAVLINK_MSG_ID_IMU_rsc_CRC 195
#define MAVLINK_MSG_ID_151_CRC 195

#define MAVLINK_MSG_IMU_rsc_FIELD_ACCEL_LEN 3
#define MAVLINK_MSG_IMU_rsc_FIELD_GYRO_LEN 3
#define MAVLINK_MSG_IMU_rsc_FIELD_COMPASS_LEN 3

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_IMU_rsc { \
    151, \
    "IMU_rsc", \
    4, \
    {  { "accel", NULL, MAVLINK_TYPE_FLOAT, 3, 0, offsetof(mavlink_imu_rsc_t, accel) }, \
         { "gyro", NULL, MAVLINK_TYPE_FLOAT, 3, 12, offsetof(mavlink_imu_rsc_t, gyro) }, \
         { "compass", NULL, MAVLINK_TYPE_FLOAT, 3, 24, offsetof(mavlink_imu_rsc_t, compass) }, \
         { "time", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_imu_rsc_t, time) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_IMU_rsc { \
    "IMU_rsc", \
    4, \
    {  { "accel", NULL, MAVLINK_TYPE_FLOAT, 3, 0, offsetof(mavlink_imu_rsc_t, accel) }, \
         { "gyro", NULL, MAVLINK_TYPE_FLOAT, 3, 12, offsetof(mavlink_imu_rsc_t, gyro) }, \
         { "compass", NULL, MAVLINK_TYPE_FLOAT, 3, 24, offsetof(mavlink_imu_rsc_t, compass) }, \
         { "time", NULL, MAVLINK_TYPE_FLOAT, 0, 36, offsetof(mavlink_imu_rsc_t, time) }, \
         } \
}
#endif

/**
 * @brief Pack a imu_rsc message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param accel accelerations
 * @param gyro angle velocities
 * @param compass compass (magnetometer) vector
 * @param time current time
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_imu_rsc_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               const float *accel, const float *gyro, const float *compass, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IMU_rsc_LEN];
    _mav_put_float(buf, 36, time);
    _mav_put_float_array(buf, 0, accel, 3);
    _mav_put_float_array(buf, 12, gyro, 3);
    _mav_put_float_array(buf, 24, compass, 3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IMU_rsc_LEN);
#else
    mavlink_imu_rsc_t packet;
    packet.time = time;
    mav_array_memcpy(packet.accel, accel, sizeof(float)*3);
    mav_array_memcpy(packet.gyro, gyro, sizeof(float)*3);
    mav_array_memcpy(packet.compass, compass, sizeof(float)*3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IMU_rsc_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IMU_rsc;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_IMU_rsc_MIN_LEN, MAVLINK_MSG_ID_IMU_rsc_LEN, MAVLINK_MSG_ID_IMU_rsc_CRC);
}

/**
 * @brief Pack a imu_rsc message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param accel accelerations
 * @param gyro angle velocities
 * @param compass compass (magnetometer) vector
 * @param time current time
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_imu_rsc_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   const float *accel,const float *gyro,const float *compass,float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IMU_rsc_LEN];
    _mav_put_float(buf, 36, time);
    _mav_put_float_array(buf, 0, accel, 3);
    _mav_put_float_array(buf, 12, gyro, 3);
    _mav_put_float_array(buf, 24, compass, 3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IMU_rsc_LEN);
#else
    mavlink_imu_rsc_t packet;
    packet.time = time;
    mav_array_memcpy(packet.accel, accel, sizeof(float)*3);
    mav_array_memcpy(packet.gyro, gyro, sizeof(float)*3);
    mav_array_memcpy(packet.compass, compass, sizeof(float)*3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IMU_rsc_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IMU_rsc;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_IMU_rsc_MIN_LEN, MAVLINK_MSG_ID_IMU_rsc_LEN, MAVLINK_MSG_ID_IMU_rsc_CRC);
}

/**
 * @brief Encode a imu_rsc struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param imu_rsc C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_imu_rsc_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_imu_rsc_t* imu_rsc)
{
    return mavlink_msg_imu_rsc_pack(system_id, component_id, msg, imu_rsc->accel, imu_rsc->gyro, imu_rsc->compass, imu_rsc->time);
}

/**
 * @brief Encode a imu_rsc struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param imu_rsc C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_imu_rsc_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_imu_rsc_t* imu_rsc)
{
    return mavlink_msg_imu_rsc_pack_chan(system_id, component_id, chan, msg, imu_rsc->accel, imu_rsc->gyro, imu_rsc->compass, imu_rsc->time);
}

/**
 * @brief Send a imu_rsc message
 * @param chan MAVLink channel to send the message
 *
 * @param accel accelerations
 * @param gyro angle velocities
 * @param compass compass (magnetometer) vector
 * @param time current time
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_imu_rsc_send(mavlink_channel_t chan, const float *accel, const float *gyro, const float *compass, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IMU_rsc_LEN];
    _mav_put_float(buf, 36, time);
    _mav_put_float_array(buf, 0, accel, 3);
    _mav_put_float_array(buf, 12, gyro, 3);
    _mav_put_float_array(buf, 24, compass, 3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_rsc, buf, MAVLINK_MSG_ID_IMU_rsc_MIN_LEN, MAVLINK_MSG_ID_IMU_rsc_LEN, MAVLINK_MSG_ID_IMU_rsc_CRC);
#else
    mavlink_imu_rsc_t packet;
    packet.time = time;
    mav_array_memcpy(packet.accel, accel, sizeof(float)*3);
    mav_array_memcpy(packet.gyro, gyro, sizeof(float)*3);
    mav_array_memcpy(packet.compass, compass, sizeof(float)*3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_rsc, (const char *)&packet, MAVLINK_MSG_ID_IMU_rsc_MIN_LEN, MAVLINK_MSG_ID_IMU_rsc_LEN, MAVLINK_MSG_ID_IMU_rsc_CRC);
#endif
}

/**
 * @brief Send a imu_rsc message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_imu_rsc_send_struct(mavlink_channel_t chan, const mavlink_imu_rsc_t* imu_rsc)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_imu_rsc_send(chan, imu_rsc->accel, imu_rsc->gyro, imu_rsc->compass, imu_rsc->time);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_rsc, (const char *)imu_rsc, MAVLINK_MSG_ID_IMU_rsc_MIN_LEN, MAVLINK_MSG_ID_IMU_rsc_LEN, MAVLINK_MSG_ID_IMU_rsc_CRC);
#endif
}

#if MAVLINK_MSG_ID_IMU_rsc_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_imu_rsc_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const float *accel, const float *gyro, const float *compass, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 36, time);
    _mav_put_float_array(buf, 0, accel, 3);
    _mav_put_float_array(buf, 12, gyro, 3);
    _mav_put_float_array(buf, 24, compass, 3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_rsc, buf, MAVLINK_MSG_ID_IMU_rsc_MIN_LEN, MAVLINK_MSG_ID_IMU_rsc_LEN, MAVLINK_MSG_ID_IMU_rsc_CRC);
#else
    mavlink_imu_rsc_t *packet = (mavlink_imu_rsc_t *)msgbuf;
    packet->time = time;
    mav_array_memcpy(packet->accel, accel, sizeof(float)*3);
    mav_array_memcpy(packet->gyro, gyro, sizeof(float)*3);
    mav_array_memcpy(packet->compass, compass, sizeof(float)*3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_rsc, (const char *)packet, MAVLINK_MSG_ID_IMU_rsc_MIN_LEN, MAVLINK_MSG_ID_IMU_rsc_LEN, MAVLINK_MSG_ID_IMU_rsc_CRC);
#endif
}
#endif

#endif

// MESSAGE IMU_rsc UNPACKING


/**
 * @brief Get field accel from imu_rsc message
 *
 * @return accelerations
 */
static inline uint16_t mavlink_msg_imu_rsc_get_accel(const mavlink_message_t* msg, float *accel)
{
    return _MAV_RETURN_float_array(msg, accel, 3,  0);
}

/**
 * @brief Get field gyro from imu_rsc message
 *
 * @return angle velocities
 */
static inline uint16_t mavlink_msg_imu_rsc_get_gyro(const mavlink_message_t* msg, float *gyro)
{
    return _MAV_RETURN_float_array(msg, gyro, 3,  12);
}

/**
 * @brief Get field compass from imu_rsc message
 *
 * @return compass (magnetometer) vector
 */
static inline uint16_t mavlink_msg_imu_rsc_get_compass(const mavlink_message_t* msg, float *compass)
{
    return _MAV_RETURN_float_array(msg, compass, 3,  24);
}

/**
 * @brief Get field time from imu_rsc message
 *
 * @return current time
 */
static inline float mavlink_msg_imu_rsc_get_time(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  36);
}

/**
 * @brief Decode a imu_rsc message into a struct
 *
 * @param msg The message to decode
 * @param imu_rsc C-struct to decode the message contents into
 */
static inline void mavlink_msg_imu_rsc_decode(const mavlink_message_t* msg, mavlink_imu_rsc_t* imu_rsc)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_imu_rsc_get_accel(msg, imu_rsc->accel);
    mavlink_msg_imu_rsc_get_gyro(msg, imu_rsc->gyro);
    mavlink_msg_imu_rsc_get_compass(msg, imu_rsc->compass);
    imu_rsc->time = mavlink_msg_imu_rsc_get_time(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_IMU_rsc_LEN? msg->len : MAVLINK_MSG_ID_IMU_rsc_LEN;
        memset(imu_rsc, 0, MAVLINK_MSG_ID_IMU_rsc_LEN);
    memcpy(imu_rsc, _MAV_PAYLOAD(msg), len);
#endif
}
