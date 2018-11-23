#pragma once
// MESSAGE ATMEGA PACKING

#define MAVLINK_MSG_ID_ATMEGA 157

MAVPACKED(
typedef struct __mavlink_atmega_t {
 float time; /*< current time*/
 float pressure; /*< pressure*/
 float height; /*< height*/
 float temp; /*< temp*/
 uint8_t baro_state; /*< state of baro*/
 uint8_t motor_state; /*< state of motor*/
 uint8_t para_state; /*< state of parashute*/
 uint8_t globalStage; /*< stage*/
}) mavlink_atmega_t;

#define MAVLINK_MSG_ID_ATMEGA_LEN 20
#define MAVLINK_MSG_ID_ATMEGA_MIN_LEN 20
#define MAVLINK_MSG_ID_157_LEN 20
#define MAVLINK_MSG_ID_157_MIN_LEN 20

#define MAVLINK_MSG_ID_ATMEGA_CRC 77
#define MAVLINK_MSG_ID_157_CRC 77



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_ATMEGA { \
    157, \
    "ATMEGA", \
    8, \
    {  { "time", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_atmega_t, time) }, \
         { "pressure", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_atmega_t, pressure) }, \
         { "height", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_atmega_t, height) }, \
         { "temp", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_atmega_t, temp) }, \
         { "baro_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_atmega_t, baro_state) }, \
         { "motor_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 17, offsetof(mavlink_atmega_t, motor_state) }, \
         { "para_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_atmega_t, para_state) }, \
         { "globalStage", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_atmega_t, globalStage) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_ATMEGA { \
    "ATMEGA", \
    8, \
    {  { "time", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_atmega_t, time) }, \
         { "pressure", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_atmega_t, pressure) }, \
         { "height", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_atmega_t, height) }, \
         { "temp", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_atmega_t, temp) }, \
         { "baro_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_atmega_t, baro_state) }, \
         { "motor_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 17, offsetof(mavlink_atmega_t, motor_state) }, \
         { "para_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 18, offsetof(mavlink_atmega_t, para_state) }, \
         { "globalStage", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_atmega_t, globalStage) }, \
         } \
}
#endif

/**
 * @brief Pack a atmega message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time current time
 * @param pressure pressure
 * @param height height
 * @param temp temp
 * @param baro_state state of baro
 * @param motor_state state of motor
 * @param para_state state of parashute
 * @param globalStage stage
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_atmega_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float time, float pressure, float height, float temp, uint8_t baro_state, uint8_t motor_state, uint8_t para_state, uint8_t globalStage)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ATMEGA_LEN];
    _mav_put_float(buf, 0, time);
    _mav_put_float(buf, 4, pressure);
    _mav_put_float(buf, 8, height);
    _mav_put_float(buf, 12, temp);
    _mav_put_uint8_t(buf, 16, baro_state);
    _mav_put_uint8_t(buf, 17, motor_state);
    _mav_put_uint8_t(buf, 18, para_state);
    _mav_put_uint8_t(buf, 19, globalStage);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ATMEGA_LEN);
#else
    mavlink_atmega_t packet;
    packet.time = time;
    packet.pressure = pressure;
    packet.height = height;
    packet.temp = temp;
    packet.baro_state = baro_state;
    packet.motor_state = motor_state;
    packet.para_state = para_state;
    packet.globalStage = globalStage;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ATMEGA_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ATMEGA;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_ATMEGA_MIN_LEN, MAVLINK_MSG_ID_ATMEGA_LEN, MAVLINK_MSG_ID_ATMEGA_CRC);
}

/**
 * @brief Pack a atmega message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time current time
 * @param pressure pressure
 * @param height height
 * @param temp temp
 * @param baro_state state of baro
 * @param motor_state state of motor
 * @param para_state state of parashute
 * @param globalStage stage
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_atmega_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float time,float pressure,float height,float temp,uint8_t baro_state,uint8_t motor_state,uint8_t para_state,uint8_t globalStage)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ATMEGA_LEN];
    _mav_put_float(buf, 0, time);
    _mav_put_float(buf, 4, pressure);
    _mav_put_float(buf, 8, height);
    _mav_put_float(buf, 12, temp);
    _mav_put_uint8_t(buf, 16, baro_state);
    _mav_put_uint8_t(buf, 17, motor_state);
    _mav_put_uint8_t(buf, 18, para_state);
    _mav_put_uint8_t(buf, 19, globalStage);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ATMEGA_LEN);
#else
    mavlink_atmega_t packet;
    packet.time = time;
    packet.pressure = pressure;
    packet.height = height;
    packet.temp = temp;
    packet.baro_state = baro_state;
    packet.motor_state = motor_state;
    packet.para_state = para_state;
    packet.globalStage = globalStage;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ATMEGA_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ATMEGA;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_ATMEGA_MIN_LEN, MAVLINK_MSG_ID_ATMEGA_LEN, MAVLINK_MSG_ID_ATMEGA_CRC);
}

/**
 * @brief Encode a atmega struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param atmega C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_atmega_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_atmega_t* atmega)
{
    return mavlink_msg_atmega_pack(system_id, component_id, msg, atmega->time, atmega->pressure, atmega->height, atmega->temp, atmega->baro_state, atmega->motor_state, atmega->para_state, atmega->globalStage);
}

/**
 * @brief Encode a atmega struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param atmega C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_atmega_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_atmega_t* atmega)
{
    return mavlink_msg_atmega_pack_chan(system_id, component_id, chan, msg, atmega->time, atmega->pressure, atmega->height, atmega->temp, atmega->baro_state, atmega->motor_state, atmega->para_state, atmega->globalStage);
}

/**
 * @brief Send a atmega message
 * @param chan MAVLink channel to send the message
 *
 * @param time current time
 * @param pressure pressure
 * @param height height
 * @param temp temp
 * @param baro_state state of baro
 * @param motor_state state of motor
 * @param para_state state of parashute
 * @param globalStage stage
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_atmega_send(mavlink_channel_t chan, float time, float pressure, float height, float temp, uint8_t baro_state, uint8_t motor_state, uint8_t para_state, uint8_t globalStage)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ATMEGA_LEN];
    _mav_put_float(buf, 0, time);
    _mav_put_float(buf, 4, pressure);
    _mav_put_float(buf, 8, height);
    _mav_put_float(buf, 12, temp);
    _mav_put_uint8_t(buf, 16, baro_state);
    _mav_put_uint8_t(buf, 17, motor_state);
    _mav_put_uint8_t(buf, 18, para_state);
    _mav_put_uint8_t(buf, 19, globalStage);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ATMEGA, buf, MAVLINK_MSG_ID_ATMEGA_MIN_LEN, MAVLINK_MSG_ID_ATMEGA_LEN, MAVLINK_MSG_ID_ATMEGA_CRC);
#else
    mavlink_atmega_t packet;
    packet.time = time;
    packet.pressure = pressure;
    packet.height = height;
    packet.temp = temp;
    packet.baro_state = baro_state;
    packet.motor_state = motor_state;
    packet.para_state = para_state;
    packet.globalStage = globalStage;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ATMEGA, (const char *)&packet, MAVLINK_MSG_ID_ATMEGA_MIN_LEN, MAVLINK_MSG_ID_ATMEGA_LEN, MAVLINK_MSG_ID_ATMEGA_CRC);
#endif
}

/**
 * @brief Send a atmega message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_atmega_send_struct(mavlink_channel_t chan, const mavlink_atmega_t* atmega)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_atmega_send(chan, atmega->time, atmega->pressure, atmega->height, atmega->temp, atmega->baro_state, atmega->motor_state, atmega->para_state, atmega->globalStage);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ATMEGA, (const char *)atmega, MAVLINK_MSG_ID_ATMEGA_MIN_LEN, MAVLINK_MSG_ID_ATMEGA_LEN, MAVLINK_MSG_ID_ATMEGA_CRC);
#endif
}

#if MAVLINK_MSG_ID_ATMEGA_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_atmega_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float time, float pressure, float height, float temp, uint8_t baro_state, uint8_t motor_state, uint8_t para_state, uint8_t globalStage)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, time);
    _mav_put_float(buf, 4, pressure);
    _mav_put_float(buf, 8, height);
    _mav_put_float(buf, 12, temp);
    _mav_put_uint8_t(buf, 16, baro_state);
    _mav_put_uint8_t(buf, 17, motor_state);
    _mav_put_uint8_t(buf, 18, para_state);
    _mav_put_uint8_t(buf, 19, globalStage);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ATMEGA, buf, MAVLINK_MSG_ID_ATMEGA_MIN_LEN, MAVLINK_MSG_ID_ATMEGA_LEN, MAVLINK_MSG_ID_ATMEGA_CRC);
#else
    mavlink_atmega_t *packet = (mavlink_atmega_t *)msgbuf;
    packet->time = time;
    packet->pressure = pressure;
    packet->height = height;
    packet->temp = temp;
    packet->baro_state = baro_state;
    packet->motor_state = motor_state;
    packet->para_state = para_state;
    packet->globalStage = globalStage;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ATMEGA, (const char *)packet, MAVLINK_MSG_ID_ATMEGA_MIN_LEN, MAVLINK_MSG_ID_ATMEGA_LEN, MAVLINK_MSG_ID_ATMEGA_CRC);
#endif
}
#endif

#endif

// MESSAGE ATMEGA UNPACKING


/**
 * @brief Get field time from atmega message
 *
 * @return current time
 */
static inline float mavlink_msg_atmega_get_time(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field pressure from atmega message
 *
 * @return pressure
 */
static inline float mavlink_msg_atmega_get_pressure(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field height from atmega message
 *
 * @return height
 */
static inline float mavlink_msg_atmega_get_height(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field temp from atmega message
 *
 * @return temp
 */
static inline float mavlink_msg_atmega_get_temp(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field baro_state from atmega message
 *
 * @return state of baro
 */
static inline uint8_t mavlink_msg_atmega_get_baro_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  16);
}

/**
 * @brief Get field motor_state from atmega message
 *
 * @return state of motor
 */
static inline uint8_t mavlink_msg_atmega_get_motor_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  17);
}

/**
 * @brief Get field para_state from atmega message
 *
 * @return state of parashute
 */
static inline uint8_t mavlink_msg_atmega_get_para_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  18);
}

/**
 * @brief Get field globalStage from atmega message
 *
 * @return stage
 */
static inline uint8_t mavlink_msg_atmega_get_globalStage(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  19);
}

/**
 * @brief Decode a atmega message into a struct
 *
 * @param msg The message to decode
 * @param atmega C-struct to decode the message contents into
 */
static inline void mavlink_msg_atmega_decode(const mavlink_message_t* msg, mavlink_atmega_t* atmega)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    atmega->time = mavlink_msg_atmega_get_time(msg);
    atmega->pressure = mavlink_msg_atmega_get_pressure(msg);
    atmega->height = mavlink_msg_atmega_get_height(msg);
    atmega->temp = mavlink_msg_atmega_get_temp(msg);
    atmega->baro_state = mavlink_msg_atmega_get_baro_state(msg);
    atmega->motor_state = mavlink_msg_atmega_get_motor_state(msg);
    atmega->para_state = mavlink_msg_atmega_get_para_state(msg);
    atmega->globalStage = mavlink_msg_atmega_get_globalStage(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_ATMEGA_LEN? msg->len : MAVLINK_MSG_ID_ATMEGA_LEN;
        memset(atmega, 0, MAVLINK_MSG_ID_ATMEGA_LEN);
    memcpy(atmega, _MAV_PAYLOAD(msg), len);
#endif
}
