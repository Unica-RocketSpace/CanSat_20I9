#pragma once
// MESSAGE IMU_isc PACKING

#define MAVLINK_MSG_ID_IMU_isc 152

MAVPACKED(
typedef struct __mavlink_imu_isc_t {
 float accel[3]; /*< accelerations*/
 float compass[3]; /*< compass (magnetometer) vector*/
 float velocities[3]; /*< velocities*/
 float coordinates[3]; /*< coordinates from IMU*/
 float quaternion[4]; /*< quaternion for translating RSC into ISC*/
 float time; /*< current time*/
}) mavlink_imu_isc_t;

#define MAVLINK_MSG_ID_IMU_isc_LEN 68
#define MAVLINK_MSG_ID_IMU_isc_MIN_LEN 68
#define MAVLINK_MSG_ID_152_LEN 68
#define MAVLINK_MSG_ID_152_MIN_LEN 68

#define MAVLINK_MSG_ID_IMU_isc_CRC 133
#define MAVLINK_MSG_ID_152_CRC 133

#define MAVLINK_MSG_IMU_isc_FIELD_ACCEL_LEN 3
#define MAVLINK_MSG_IMU_isc_FIELD_COMPASS_LEN 3
#define MAVLINK_MSG_IMU_isc_FIELD_VELOCITIES_LEN 3
#define MAVLINK_MSG_IMU_isc_FIELD_COORDINATES_LEN 3
#define MAVLINK_MSG_IMU_isc_FIELD_QUATERNION_LEN 4

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_IMU_isc { \
    152, \
    "IMU_isc", \
    6, \
    {  { "accel", NULL, MAVLINK_TYPE_FLOAT, 3, 0, offsetof(mavlink_imu_isc_t, accel) }, \
         { "compass", NULL, MAVLINK_TYPE_FLOAT, 3, 12, offsetof(mavlink_imu_isc_t, compass) }, \
         { "velocities", NULL, MAVLINK_TYPE_FLOAT, 3, 24, offsetof(mavlink_imu_isc_t, velocities) }, \
         { "coordinates", NULL, MAVLINK_TYPE_FLOAT, 3, 36, offsetof(mavlink_imu_isc_t, coordinates) }, \
         { "quaternion", NULL, MAVLINK_TYPE_FLOAT, 4, 48, offsetof(mavlink_imu_isc_t, quaternion) }, \
         { "time", NULL, MAVLINK_TYPE_FLOAT, 0, 64, offsetof(mavlink_imu_isc_t, time) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_IMU_isc { \
    "IMU_isc", \
    6, \
    {  { "accel", NULL, MAVLINK_TYPE_FLOAT, 3, 0, offsetof(mavlink_imu_isc_t, accel) }, \
         { "compass", NULL, MAVLINK_TYPE_FLOAT, 3, 12, offsetof(mavlink_imu_isc_t, compass) }, \
         { "velocities", NULL, MAVLINK_TYPE_FLOAT, 3, 24, offsetof(mavlink_imu_isc_t, velocities) }, \
         { "coordinates", NULL, MAVLINK_TYPE_FLOAT, 3, 36, offsetof(mavlink_imu_isc_t, coordinates) }, \
         { "quaternion", NULL, MAVLINK_TYPE_FLOAT, 4, 48, offsetof(mavlink_imu_isc_t, quaternion) }, \
         { "time", NULL, MAVLINK_TYPE_FLOAT, 0, 64, offsetof(mavlink_imu_isc_t, time) }, \
         } \
}
#endif

/**
 * @brief Pack a imu_isc message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param accel accelerations
 * @param compass compass (magnetometer) vector
 * @param velocities velocities
 * @param coordinates coordinates from IMU
 * @param quaternion quaternion for translating RSC into ISC
 * @param time current time
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_imu_isc_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               const float *accel, const float *compass, const float *velocities, const float *coordinates, const float *quaternion, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IMU_isc_LEN];
    _mav_put_float(buf, 64, time);
    _mav_put_float_array(buf, 0, accel, 3);
    _mav_put_float_array(buf, 12, compass, 3);
    _mav_put_float_array(buf, 24, velocities, 3);
    _mav_put_float_array(buf, 36, coordinates, 3);
    _mav_put_float_array(buf, 48, quaternion, 4);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IMU_isc_LEN);
#else
    mavlink_imu_isc_t packet;
    packet.time = time;
    mav_array_memcpy(packet.accel, accel, sizeof(float)*3);
    mav_array_memcpy(packet.compass, compass, sizeof(float)*3);
    mav_array_memcpy(packet.velocities, velocities, sizeof(float)*3);
    mav_array_memcpy(packet.coordinates, coordinates, sizeof(float)*3);
    mav_array_memcpy(packet.quaternion, quaternion, sizeof(float)*4);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IMU_isc_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IMU_isc;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_IMU_isc_MIN_LEN, MAVLINK_MSG_ID_IMU_isc_LEN, MAVLINK_MSG_ID_IMU_isc_CRC);
}

/**
 * @brief Pack a imu_isc message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param accel accelerations
 * @param compass compass (magnetometer) vector
 * @param velocities velocities
 * @param coordinates coordinates from IMU
 * @param quaternion quaternion for translating RSC into ISC
 * @param time current time
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_imu_isc_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   const float *accel,const float *compass,const float *velocities,const float *coordinates,const float *quaternion,float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IMU_isc_LEN];
    _mav_put_float(buf, 64, time);
    _mav_put_float_array(buf, 0, accel, 3);
    _mav_put_float_array(buf, 12, compass, 3);
    _mav_put_float_array(buf, 24, velocities, 3);
    _mav_put_float_array(buf, 36, coordinates, 3);
    _mav_put_float_array(buf, 48, quaternion, 4);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_IMU_isc_LEN);
#else
    mavlink_imu_isc_t packet;
    packet.time = time;
    mav_array_memcpy(packet.accel, accel, sizeof(float)*3);
    mav_array_memcpy(packet.compass, compass, sizeof(float)*3);
    mav_array_memcpy(packet.velocities, velocities, sizeof(float)*3);
    mav_array_memcpy(packet.coordinates, coordinates, sizeof(float)*3);
    mav_array_memcpy(packet.quaternion, quaternion, sizeof(float)*4);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_IMU_isc_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_IMU_isc;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_IMU_isc_MIN_LEN, MAVLINK_MSG_ID_IMU_isc_LEN, MAVLINK_MSG_ID_IMU_isc_CRC);
}

/**
 * @brief Encode a imu_isc struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param imu_isc C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_imu_isc_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_imu_isc_t* imu_isc)
{
    return mavlink_msg_imu_isc_pack(system_id, component_id, msg, imu_isc->accel, imu_isc->compass, imu_isc->velocities, imu_isc->coordinates, imu_isc->quaternion, imu_isc->time);
}

/**
 * @brief Encode a imu_isc struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param imu_isc C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_imu_isc_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_imu_isc_t* imu_isc)
{
    return mavlink_msg_imu_isc_pack_chan(system_id, component_id, chan, msg, imu_isc->accel, imu_isc->compass, imu_isc->velocities, imu_isc->coordinates, imu_isc->quaternion, imu_isc->time);
}

/**
 * @brief Send a imu_isc message
 * @param chan MAVLink channel to send the message
 *
 * @param accel accelerations
 * @param compass compass (magnetometer) vector
 * @param velocities velocities
 * @param coordinates coordinates from IMU
 * @param quaternion quaternion for translating RSC into ISC
 * @param time current time
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_imu_isc_send(mavlink_channel_t chan, const float *accel, const float *compass, const float *velocities, const float *coordinates, const float *quaternion, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_IMU_isc_LEN];
    _mav_put_float(buf, 64, time);
    _mav_put_float_array(buf, 0, accel, 3);
    _mav_put_float_array(buf, 12, compass, 3);
    _mav_put_float_array(buf, 24, velocities, 3);
    _mav_put_float_array(buf, 36, coordinates, 3);
    _mav_put_float_array(buf, 48, quaternion, 4);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_isc, buf, MAVLINK_MSG_ID_IMU_isc_MIN_LEN, MAVLINK_MSG_ID_IMU_isc_LEN, MAVLINK_MSG_ID_IMU_isc_CRC);
#else
    mavlink_imu_isc_t packet;
    packet.time = time;
    mav_array_memcpy(packet.accel, accel, sizeof(float)*3);
    mav_array_memcpy(packet.compass, compass, sizeof(float)*3);
    mav_array_memcpy(packet.velocities, velocities, sizeof(float)*3);
    mav_array_memcpy(packet.coordinates, coordinates, sizeof(float)*3);
    mav_array_memcpy(packet.quaternion, quaternion, sizeof(float)*4);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_isc, (const char *)&packet, MAVLINK_MSG_ID_IMU_isc_MIN_LEN, MAVLINK_MSG_ID_IMU_isc_LEN, MAVLINK_MSG_ID_IMU_isc_CRC);
#endif
}

/**
 * @brief Send a imu_isc message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_imu_isc_send_struct(mavlink_channel_t chan, const mavlink_imu_isc_t* imu_isc)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_imu_isc_send(chan, imu_isc->accel, imu_isc->compass, imu_isc->velocities, imu_isc->coordinates, imu_isc->quaternion, imu_isc->time);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_isc, (const char *)imu_isc, MAVLINK_MSG_ID_IMU_isc_MIN_LEN, MAVLINK_MSG_ID_IMU_isc_LEN, MAVLINK_MSG_ID_IMU_isc_CRC);
#endif
}

#if MAVLINK_MSG_ID_IMU_isc_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_imu_isc_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const float *accel, const float *compass, const float *velocities, const float *coordinates, const float *quaternion, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 64, time);
    _mav_put_float_array(buf, 0, accel, 3);
    _mav_put_float_array(buf, 12, compass, 3);
    _mav_put_float_array(buf, 24, velocities, 3);
    _mav_put_float_array(buf, 36, coordinates, 3);
    _mav_put_float_array(buf, 48, quaternion, 4);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_isc, buf, MAVLINK_MSG_ID_IMU_isc_MIN_LEN, MAVLINK_MSG_ID_IMU_isc_LEN, MAVLINK_MSG_ID_IMU_isc_CRC);
#else
    mavlink_imu_isc_t *packet = (mavlink_imu_isc_t *)msgbuf;
    packet->time = time;
    mav_array_memcpy(packet->accel, accel, sizeof(float)*3);
    mav_array_memcpy(packet->compass, compass, sizeof(float)*3);
    mav_array_memcpy(packet->velocities, velocities, sizeof(float)*3);
    mav_array_memcpy(packet->coordinates, coordinates, sizeof(float)*3);
    mav_array_memcpy(packet->quaternion, quaternion, sizeof(float)*4);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_IMU_isc, (const char *)packet, MAVLINK_MSG_ID_IMU_isc_MIN_LEN, MAVLINK_MSG_ID_IMU_isc_LEN, MAVLINK_MSG_ID_IMU_isc_CRC);
#endif
}
#endif

#endif

// MESSAGE IMU_isc UNPACKING


/**
 * @brief Get field accel from imu_isc message
 *
 * @return accelerations
 */
static inline uint16_t mavlink_msg_imu_isc_get_accel(const mavlink_message_t* msg, float *accel)
{
    return _MAV_RETURN_float_array(msg, accel, 3,  0);
}

/**
 * @brief Get field compass from imu_isc message
 *
 * @return compass (magnetometer) vector
 */
static inline uint16_t mavlink_msg_imu_isc_get_compass(const mavlink_message_t* msg, float *compass)
{
    return _MAV_RETURN_float_array(msg, compass, 3,  12);
}

/**
 * @brief Get field velocities from imu_isc message
 *
 * @return velocities
 */
static inline uint16_t mavlink_msg_imu_isc_get_velocities(const mavlink_message_t* msg, float *velocities)
{
    return _MAV_RETURN_float_array(msg, velocities, 3,  24);
}

/**
 * @brief Get field coordinates from imu_isc message
 *
 * @return coordinates from IMU
 */
static inline uint16_t mavlink_msg_imu_isc_get_coordinates(const mavlink_message_t* msg, float *coordinates)
{
    return _MAV_RETURN_float_array(msg, coordinates, 3,  36);
}

/**
 * @brief Get field quaternion from imu_isc message
 *
 * @return quaternion for translating RSC into ISC
 */
static inline uint16_t mavlink_msg_imu_isc_get_quaternion(const mavlink_message_t* msg, float *quaternion)
{
    return _MAV_RETURN_float_array(msg, quaternion, 4,  48);
}

/**
 * @brief Get field time from imu_isc message
 *
 * @return current time
 */
static inline float mavlink_msg_imu_isc_get_time(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  64);
}

/**
 * @brief Decode a imu_isc message into a struct
 *
 * @param msg The message to decode
 * @param imu_isc C-struct to decode the message contents into
 */
static inline void mavlink_msg_imu_isc_decode(const mavlink_message_t* msg, mavlink_imu_isc_t* imu_isc)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_imu_isc_get_accel(msg, imu_isc->accel);
    mavlink_msg_imu_isc_get_compass(msg, imu_isc->compass);
    mavlink_msg_imu_isc_get_velocities(msg, imu_isc->velocities);
    mavlink_msg_imu_isc_get_coordinates(msg, imu_isc->coordinates);
    mavlink_msg_imu_isc_get_quaternion(msg, imu_isc->quaternion);
    imu_isc->time = mavlink_msg_imu_isc_get_time(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_IMU_isc_LEN? msg->len : MAVLINK_MSG_ID_IMU_isc_LEN;
        memset(imu_isc, 0, MAVLINK_MSG_ID_IMU_isc_LEN);
    memcpy(imu_isc, _MAV_PAYLOAD(msg), len);
#endif
}
