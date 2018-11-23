#pragma once
// MESSAGE Sensors PACKING

#define MAVLINK_MSG_ID_Sensors 153

MAVPACKED(
typedef struct __mavlink_sensors_t {
 float temp; /*< temperature*/
 float pressure; /*< pressure*/
 float height; /*< height*/
 float time; /*< current time*/
}) mavlink_sensors_t;

#define MAVLINK_MSG_ID_Sensors_LEN 16
#define MAVLINK_MSG_ID_Sensors_MIN_LEN 16
#define MAVLINK_MSG_ID_153_LEN 16
#define MAVLINK_MSG_ID_153_MIN_LEN 16

#define MAVLINK_MSG_ID_Sensors_CRC 53
#define MAVLINK_MSG_ID_153_CRC 53



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_Sensors { \
    153, \
    "Sensors", \
    4, \
    {  { "temp", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_sensors_t, temp) }, \
         { "pressure", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_sensors_t, pressure) }, \
         { "height", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_sensors_t, height) }, \
         { "time", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_sensors_t, time) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_Sensors { \
    "Sensors", \
    4, \
    {  { "temp", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_sensors_t, temp) }, \
         { "pressure", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_sensors_t, pressure) }, \
         { "height", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_sensors_t, height) }, \
         { "time", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_sensors_t, time) }, \
         } \
}
#endif

/**
 * @brief Pack a sensors message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param temp temperature
 * @param pressure pressure
 * @param height height
 * @param time current time
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_sensors_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float temp, float pressure, float height, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_Sensors_LEN];
    _mav_put_float(buf, 0, temp);
    _mav_put_float(buf, 4, pressure);
    _mav_put_float(buf, 8, height);
    _mav_put_float(buf, 12, time);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_Sensors_LEN);
#else
    mavlink_sensors_t packet;
    packet.temp = temp;
    packet.pressure = pressure;
    packet.height = height;
    packet.time = time;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_Sensors_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_Sensors;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_Sensors_MIN_LEN, MAVLINK_MSG_ID_Sensors_LEN, MAVLINK_MSG_ID_Sensors_CRC);
}

/**
 * @brief Pack a sensors message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param temp temperature
 * @param pressure pressure
 * @param height height
 * @param time current time
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_sensors_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float temp,float pressure,float height,float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_Sensors_LEN];
    _mav_put_float(buf, 0, temp);
    _mav_put_float(buf, 4, pressure);
    _mav_put_float(buf, 8, height);
    _mav_put_float(buf, 12, time);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_Sensors_LEN);
#else
    mavlink_sensors_t packet;
    packet.temp = temp;
    packet.pressure = pressure;
    packet.height = height;
    packet.time = time;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_Sensors_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_Sensors;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_Sensors_MIN_LEN, MAVLINK_MSG_ID_Sensors_LEN, MAVLINK_MSG_ID_Sensors_CRC);
}

/**
 * @brief Encode a sensors struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param sensors C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_sensors_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_sensors_t* sensors)
{
    return mavlink_msg_sensors_pack(system_id, component_id, msg, sensors->temp, sensors->pressure, sensors->height, sensors->time);
}

/**
 * @brief Encode a sensors struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param sensors C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_sensors_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_sensors_t* sensors)
{
    return mavlink_msg_sensors_pack_chan(system_id, component_id, chan, msg, sensors->temp, sensors->pressure, sensors->height, sensors->time);
}

/**
 * @brief Send a sensors message
 * @param chan MAVLink channel to send the message
 *
 * @param temp temperature
 * @param pressure pressure
 * @param height height
 * @param time current time
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_sensors_send(mavlink_channel_t chan, float temp, float pressure, float height, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_Sensors_LEN];
    _mav_put_float(buf, 0, temp);
    _mav_put_float(buf, 4, pressure);
    _mav_put_float(buf, 8, height);
    _mav_put_float(buf, 12, time);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_Sensors, buf, MAVLINK_MSG_ID_Sensors_MIN_LEN, MAVLINK_MSG_ID_Sensors_LEN, MAVLINK_MSG_ID_Sensors_CRC);
#else
    mavlink_sensors_t packet;
    packet.temp = temp;
    packet.pressure = pressure;
    packet.height = height;
    packet.time = time;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_Sensors, (const char *)&packet, MAVLINK_MSG_ID_Sensors_MIN_LEN, MAVLINK_MSG_ID_Sensors_LEN, MAVLINK_MSG_ID_Sensors_CRC);
#endif
}

/**
 * @brief Send a sensors message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_sensors_send_struct(mavlink_channel_t chan, const mavlink_sensors_t* sensors)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_sensors_send(chan, sensors->temp, sensors->pressure, sensors->height, sensors->time);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_Sensors, (const char *)sensors, MAVLINK_MSG_ID_Sensors_MIN_LEN, MAVLINK_MSG_ID_Sensors_LEN, MAVLINK_MSG_ID_Sensors_CRC);
#endif
}

#if MAVLINK_MSG_ID_Sensors_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_sensors_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float temp, float pressure, float height, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, temp);
    _mav_put_float(buf, 4, pressure);
    _mav_put_float(buf, 8, height);
    _mav_put_float(buf, 12, time);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_Sensors, buf, MAVLINK_MSG_ID_Sensors_MIN_LEN, MAVLINK_MSG_ID_Sensors_LEN, MAVLINK_MSG_ID_Sensors_CRC);
#else
    mavlink_sensors_t *packet = (mavlink_sensors_t *)msgbuf;
    packet->temp = temp;
    packet->pressure = pressure;
    packet->height = height;
    packet->time = time;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_Sensors, (const char *)packet, MAVLINK_MSG_ID_Sensors_MIN_LEN, MAVLINK_MSG_ID_Sensors_LEN, MAVLINK_MSG_ID_Sensors_CRC);
#endif
}
#endif

#endif

// MESSAGE Sensors UNPACKING


/**
 * @brief Get field temp from sensors message
 *
 * @return temperature
 */
static inline float mavlink_msg_sensors_get_temp(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field pressure from sensors message
 *
 * @return pressure
 */
static inline float mavlink_msg_sensors_get_pressure(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field height from sensors message
 *
 * @return height
 */
static inline float mavlink_msg_sensors_get_height(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field time from sensors message
 *
 * @return current time
 */
static inline float mavlink_msg_sensors_get_time(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Decode a sensors message into a struct
 *
 * @param msg The message to decode
 * @param sensors C-struct to decode the message contents into
 */
static inline void mavlink_msg_sensors_decode(const mavlink_message_t* msg, mavlink_sensors_t* sensors)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    sensors->temp = mavlink_msg_sensors_get_temp(msg);
    sensors->pressure = mavlink_msg_sensors_get_pressure(msg);
    sensors->height = mavlink_msg_sensors_get_height(msg);
    sensors->time = mavlink_msg_sensors_get_time(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_Sensors_LEN? msg->len : MAVLINK_MSG_ID_Sensors_LEN;
        memset(sensors, 0, MAVLINK_MSG_ID_Sensors_LEN);
    memcpy(sensors, _MAV_PAYLOAD(msg), len);
#endif
}
