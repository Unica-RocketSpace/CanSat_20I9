#pragma once
// MESSAGE GPS PACKING

#define MAVLINK_MSG_ID_GPS 154

MAVPACKED(
typedef struct __mavlink_gps_t {
 float coordinates[3]; /*< coordinates from GPS*/
 float time; /*< current time*/
}) mavlink_gps_t;

#define MAVLINK_MSG_ID_GPS_LEN 16
#define MAVLINK_MSG_ID_GPS_MIN_LEN 16
#define MAVLINK_MSG_ID_154_LEN 16
#define MAVLINK_MSG_ID_154_MIN_LEN 16

#define MAVLINK_MSG_ID_GPS_CRC 33
#define MAVLINK_MSG_ID_154_CRC 33

#define MAVLINK_MSG_GPS_FIELD_COORDINATES_LEN 3

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_GPS { \
    154, \
    "GPS", \
    2, \
    {  { "coordinates", NULL, MAVLINK_TYPE_FLOAT, 3, 0, offsetof(mavlink_gps_t, coordinates) }, \
         { "time", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_gps_t, time) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_GPS { \
    "GPS", \
    2, \
    {  { "coordinates", NULL, MAVLINK_TYPE_FLOAT, 3, 0, offsetof(mavlink_gps_t, coordinates) }, \
         { "time", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_gps_t, time) }, \
         } \
}
#endif

/**
 * @brief Pack a gps message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param coordinates coordinates from GPS
 * @param time current time
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_gps_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               const float *coordinates, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_GPS_LEN];
    _mav_put_float(buf, 12, time);
    _mav_put_float_array(buf, 0, coordinates, 3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_GPS_LEN);
#else
    mavlink_gps_t packet;
    packet.time = time;
    mav_array_memcpy(packet.coordinates, coordinates, sizeof(float)*3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_GPS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_GPS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_GPS_MIN_LEN, MAVLINK_MSG_ID_GPS_LEN, MAVLINK_MSG_ID_GPS_CRC);
}

/**
 * @brief Pack a gps message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param coordinates coordinates from GPS
 * @param time current time
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_gps_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   const float *coordinates,float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_GPS_LEN];
    _mav_put_float(buf, 12, time);
    _mav_put_float_array(buf, 0, coordinates, 3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_GPS_LEN);
#else
    mavlink_gps_t packet;
    packet.time = time;
    mav_array_memcpy(packet.coordinates, coordinates, sizeof(float)*3);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_GPS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_GPS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_GPS_MIN_LEN, MAVLINK_MSG_ID_GPS_LEN, MAVLINK_MSG_ID_GPS_CRC);
}

/**
 * @brief Encode a gps struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param gps C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_gps_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_gps_t* gps)
{
    return mavlink_msg_gps_pack(system_id, component_id, msg, gps->coordinates, gps->time);
}

/**
 * @brief Encode a gps struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param gps C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_gps_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_gps_t* gps)
{
    return mavlink_msg_gps_pack_chan(system_id, component_id, chan, msg, gps->coordinates, gps->time);
}

/**
 * @brief Send a gps message
 * @param chan MAVLink channel to send the message
 *
 * @param coordinates coordinates from GPS
 * @param time current time
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_gps_send(mavlink_channel_t chan, const float *coordinates, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_GPS_LEN];
    _mav_put_float(buf, 12, time);
    _mav_put_float_array(buf, 0, coordinates, 3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GPS, buf, MAVLINK_MSG_ID_GPS_MIN_LEN, MAVLINK_MSG_ID_GPS_LEN, MAVLINK_MSG_ID_GPS_CRC);
#else
    mavlink_gps_t packet;
    packet.time = time;
    mav_array_memcpy(packet.coordinates, coordinates, sizeof(float)*3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GPS, (const char *)&packet, MAVLINK_MSG_ID_GPS_MIN_LEN, MAVLINK_MSG_ID_GPS_LEN, MAVLINK_MSG_ID_GPS_CRC);
#endif
}

/**
 * @brief Send a gps message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_gps_send_struct(mavlink_channel_t chan, const mavlink_gps_t* gps)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_gps_send(chan, gps->coordinates, gps->time);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GPS, (const char *)gps, MAVLINK_MSG_ID_GPS_MIN_LEN, MAVLINK_MSG_ID_GPS_LEN, MAVLINK_MSG_ID_GPS_CRC);
#endif
}

#if MAVLINK_MSG_ID_GPS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_gps_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  const float *coordinates, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 12, time);
    _mav_put_float_array(buf, 0, coordinates, 3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GPS, buf, MAVLINK_MSG_ID_GPS_MIN_LEN, MAVLINK_MSG_ID_GPS_LEN, MAVLINK_MSG_ID_GPS_CRC);
#else
    mavlink_gps_t *packet = (mavlink_gps_t *)msgbuf;
    packet->time = time;
    mav_array_memcpy(packet->coordinates, coordinates, sizeof(float)*3);
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_GPS, (const char *)packet, MAVLINK_MSG_ID_GPS_MIN_LEN, MAVLINK_MSG_ID_GPS_LEN, MAVLINK_MSG_ID_GPS_CRC);
#endif
}
#endif

#endif

// MESSAGE GPS UNPACKING


/**
 * @brief Get field coordinates from gps message
 *
 * @return coordinates from GPS
 */
static inline uint16_t mavlink_msg_gps_get_coordinates(const mavlink_message_t* msg, float *coordinates)
{
    return _MAV_RETURN_float_array(msg, coordinates, 3,  0);
}

/**
 * @brief Get field time from gps message
 *
 * @return current time
 */
static inline float mavlink_msg_gps_get_time(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Decode a gps message into a struct
 *
 * @param msg The message to decode
 * @param gps C-struct to decode the message contents into
 */
static inline void mavlink_msg_gps_decode(const mavlink_message_t* msg, mavlink_gps_t* gps)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_gps_get_coordinates(msg, gps->coordinates);
    gps->time = mavlink_msg_gps_get_time(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_GPS_LEN? msg->len : MAVLINK_MSG_ID_GPS_LEN;
        memset(gps, 0, MAVLINK_MSG_ID_GPS_LEN);
    memcpy(gps, _MAV_PAYLOAD(msg), len);
#endif
}
