#pragma once
// MESSAGE STATE PACKING

#define MAVLINK_MSG_ID_STATE 150

MAVPACKED(
typedef struct __mavlink_state_t {
 float time; /*< current time*/
 uint8_t MPU_state; /*< state of MPU9255*/
 uint8_t BMP_state; /*< state of barometer*/
 uint8_t SD_state; /*< state of SD*/
 uint8_t NRF_state; /*< state of NRF24L01*/
 uint8_t MOTOR_state; /*< state of motor*/
 uint8_t GPS_state; /*< state of GPS-module*/
 uint8_t globalStage; /*< current global stage number*/
}) mavlink_state_t;

#define MAVLINK_MSG_ID_STATE_LEN 11
#define MAVLINK_MSG_ID_STATE_MIN_LEN 11
#define MAVLINK_MSG_ID_150_LEN 11
#define MAVLINK_MSG_ID_150_MIN_LEN 11

#define MAVLINK_MSG_ID_STATE_CRC 119
#define MAVLINK_MSG_ID_150_CRC 119



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_STATE { \
    150, \
    "STATE", \
    8, \
    {  { "time", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_state_t, time) }, \
         { "MPU_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_state_t, MPU_state) }, \
         { "BMP_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_state_t, BMP_state) }, \
         { "SD_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_state_t, SD_state) }, \
         { "NRF_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 7, offsetof(mavlink_state_t, NRF_state) }, \
         { "MOTOR_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_state_t, MOTOR_state) }, \
         { "GPS_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_state_t, GPS_state) }, \
         { "globalStage", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_state_t, globalStage) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_STATE { \
    "STATE", \
    8, \
    {  { "time", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_state_t, time) }, \
         { "MPU_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_state_t, MPU_state) }, \
         { "BMP_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_state_t, BMP_state) }, \
         { "SD_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_state_t, SD_state) }, \
         { "NRF_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 7, offsetof(mavlink_state_t, NRF_state) }, \
         { "MOTOR_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_state_t, MOTOR_state) }, \
         { "GPS_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_state_t, GPS_state) }, \
         { "globalStage", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_state_t, globalStage) }, \
         } \
}
#endif

/**
 * @brief Pack a state message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time current time
 * @param MPU_state state of MPU9255
 * @param BMP_state state of barometer
 * @param SD_state state of SD
 * @param NRF_state state of NRF24L01
 * @param MOTOR_state state of motor
 * @param GPS_state state of GPS-module
 * @param globalStage current global stage number
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_state_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float time, uint8_t MPU_state, uint8_t BMP_state, uint8_t SD_state, uint8_t NRF_state, uint8_t MOTOR_state, uint8_t GPS_state, uint8_t globalStage)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_STATE_LEN];
    _mav_put_float(buf, 0, time);
    _mav_put_uint8_t(buf, 4, MPU_state);
    _mav_put_uint8_t(buf, 5, BMP_state);
    _mav_put_uint8_t(buf, 6, SD_state);
    _mav_put_uint8_t(buf, 7, NRF_state);
    _mav_put_uint8_t(buf, 8, MOTOR_state);
    _mav_put_uint8_t(buf, 9, GPS_state);
    _mav_put_uint8_t(buf, 10, globalStage);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_STATE_LEN);
#else
    mavlink_state_t packet;
    packet.time = time;
    packet.MPU_state = MPU_state;
    packet.BMP_state = BMP_state;
    packet.SD_state = SD_state;
    packet.NRF_state = NRF_state;
    packet.MOTOR_state = MOTOR_state;
    packet.GPS_state = GPS_state;
    packet.globalStage = globalStage;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_STATE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_STATE_MIN_LEN, MAVLINK_MSG_ID_STATE_LEN, MAVLINK_MSG_ID_STATE_CRC);
}

/**
 * @brief Pack a state message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time current time
 * @param MPU_state state of MPU9255
 * @param BMP_state state of barometer
 * @param SD_state state of SD
 * @param NRF_state state of NRF24L01
 * @param MOTOR_state state of motor
 * @param GPS_state state of GPS-module
 * @param globalStage current global stage number
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_state_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float time,uint8_t MPU_state,uint8_t BMP_state,uint8_t SD_state,uint8_t NRF_state,uint8_t MOTOR_state,uint8_t GPS_state,uint8_t globalStage)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_STATE_LEN];
    _mav_put_float(buf, 0, time);
    _mav_put_uint8_t(buf, 4, MPU_state);
    _mav_put_uint8_t(buf, 5, BMP_state);
    _mav_put_uint8_t(buf, 6, SD_state);
    _mav_put_uint8_t(buf, 7, NRF_state);
    _mav_put_uint8_t(buf, 8, MOTOR_state);
    _mav_put_uint8_t(buf, 9, GPS_state);
    _mav_put_uint8_t(buf, 10, globalStage);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_STATE_LEN);
#else
    mavlink_state_t packet;
    packet.time = time;
    packet.MPU_state = MPU_state;
    packet.BMP_state = BMP_state;
    packet.SD_state = SD_state;
    packet.NRF_state = NRF_state;
    packet.MOTOR_state = MOTOR_state;
    packet.GPS_state = GPS_state;
    packet.globalStage = globalStage;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_STATE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_STATE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_STATE_MIN_LEN, MAVLINK_MSG_ID_STATE_LEN, MAVLINK_MSG_ID_STATE_CRC);
}

/**
 * @brief Encode a state struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_state_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_state_t* state)
{
    return mavlink_msg_state_pack(system_id, component_id, msg, state->time, state->MPU_state, state->BMP_state, state->SD_state, state->NRF_state, state->MOTOR_state, state->GPS_state, state->globalStage);
}

/**
 * @brief Encode a state struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param state C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_state_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_state_t* state)
{
    return mavlink_msg_state_pack_chan(system_id, component_id, chan, msg, state->time, state->MPU_state, state->BMP_state, state->SD_state, state->NRF_state, state->MOTOR_state, state->GPS_state, state->globalStage);
}

/**
 * @brief Send a state message
 * @param chan MAVLink channel to send the message
 *
 * @param time current time
 * @param MPU_state state of MPU9255
 * @param BMP_state state of barometer
 * @param SD_state state of SD
 * @param NRF_state state of NRF24L01
 * @param MOTOR_state state of motor
 * @param GPS_state state of GPS-module
 * @param globalStage current global stage number
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_state_send(mavlink_channel_t chan, float time, uint8_t MPU_state, uint8_t BMP_state, uint8_t SD_state, uint8_t NRF_state, uint8_t MOTOR_state, uint8_t GPS_state, uint8_t globalStage)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_STATE_LEN];
    _mav_put_float(buf, 0, time);
    _mav_put_uint8_t(buf, 4, MPU_state);
    _mav_put_uint8_t(buf, 5, BMP_state);
    _mav_put_uint8_t(buf, 6, SD_state);
    _mav_put_uint8_t(buf, 7, NRF_state);
    _mav_put_uint8_t(buf, 8, MOTOR_state);
    _mav_put_uint8_t(buf, 9, GPS_state);
    _mav_put_uint8_t(buf, 10, globalStage);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_STATE, buf, MAVLINK_MSG_ID_STATE_MIN_LEN, MAVLINK_MSG_ID_STATE_LEN, MAVLINK_MSG_ID_STATE_CRC);
#else
    mavlink_state_t packet;
    packet.time = time;
    packet.MPU_state = MPU_state;
    packet.BMP_state = BMP_state;
    packet.SD_state = SD_state;
    packet.NRF_state = NRF_state;
    packet.MOTOR_state = MOTOR_state;
    packet.GPS_state = GPS_state;
    packet.globalStage = globalStage;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_STATE, (const char *)&packet, MAVLINK_MSG_ID_STATE_MIN_LEN, MAVLINK_MSG_ID_STATE_LEN, MAVLINK_MSG_ID_STATE_CRC);
#endif
}

/**
 * @brief Send a state message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_state_send_struct(mavlink_channel_t chan, const mavlink_state_t* state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_state_send(chan, state->time, state->MPU_state, state->BMP_state, state->SD_state, state->NRF_state, state->MOTOR_state, state->GPS_state, state->globalStage);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_STATE, (const char *)state, MAVLINK_MSG_ID_STATE_MIN_LEN, MAVLINK_MSG_ID_STATE_LEN, MAVLINK_MSG_ID_STATE_CRC);
#endif
}

#if MAVLINK_MSG_ID_STATE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_state_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float time, uint8_t MPU_state, uint8_t BMP_state, uint8_t SD_state, uint8_t NRF_state, uint8_t MOTOR_state, uint8_t GPS_state, uint8_t globalStage)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, time);
    _mav_put_uint8_t(buf, 4, MPU_state);
    _mav_put_uint8_t(buf, 5, BMP_state);
    _mav_put_uint8_t(buf, 6, SD_state);
    _mav_put_uint8_t(buf, 7, NRF_state);
    _mav_put_uint8_t(buf, 8, MOTOR_state);
    _mav_put_uint8_t(buf, 9, GPS_state);
    _mav_put_uint8_t(buf, 10, globalStage);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_STATE, buf, MAVLINK_MSG_ID_STATE_MIN_LEN, MAVLINK_MSG_ID_STATE_LEN, MAVLINK_MSG_ID_STATE_CRC);
#else
    mavlink_state_t *packet = (mavlink_state_t *)msgbuf;
    packet->time = time;
    packet->MPU_state = MPU_state;
    packet->BMP_state = BMP_state;
    packet->SD_state = SD_state;
    packet->NRF_state = NRF_state;
    packet->MOTOR_state = MOTOR_state;
    packet->GPS_state = GPS_state;
    packet->globalStage = globalStage;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_STATE, (const char *)packet, MAVLINK_MSG_ID_STATE_MIN_LEN, MAVLINK_MSG_ID_STATE_LEN, MAVLINK_MSG_ID_STATE_CRC);
#endif
}
#endif

#endif

// MESSAGE STATE UNPACKING


/**
 * @brief Get field time from state message
 *
 * @return current time
 */
static inline float mavlink_msg_state_get_time(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field MPU_state from state message
 *
 * @return state of MPU9255
 */
static inline uint8_t mavlink_msg_state_get_MPU_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field BMP_state from state message
 *
 * @return state of barometer
 */
static inline uint8_t mavlink_msg_state_get_BMP_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  5);
}

/**
 * @brief Get field SD_state from state message
 *
 * @return state of SD
 */
static inline uint8_t mavlink_msg_state_get_SD_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  6);
}

/**
 * @brief Get field NRF_state from state message
 *
 * @return state of NRF24L01
 */
static inline uint8_t mavlink_msg_state_get_NRF_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  7);
}

/**
 * @brief Get field MOTOR_state from state message
 *
 * @return state of motor
 */
static inline uint8_t mavlink_msg_state_get_MOTOR_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Get field GPS_state from state message
 *
 * @return state of GPS-module
 */
static inline uint8_t mavlink_msg_state_get_GPS_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  9);
}

/**
 * @brief Get field globalStage from state message
 *
 * @return current global stage number
 */
static inline uint8_t mavlink_msg_state_get_globalStage(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  10);
}

/**
 * @brief Decode a state message into a struct
 *
 * @param msg The message to decode
 * @param state C-struct to decode the message contents into
 */
static inline void mavlink_msg_state_decode(const mavlink_message_t* msg, mavlink_state_t* state)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    state->time = mavlink_msg_state_get_time(msg);
    state->MPU_state = mavlink_msg_state_get_MPU_state(msg);
    state->BMP_state = mavlink_msg_state_get_BMP_state(msg);
    state->SD_state = mavlink_msg_state_get_SD_state(msg);
    state->NRF_state = mavlink_msg_state_get_NRF_state(msg);
    state->MOTOR_state = mavlink_msg_state_get_MOTOR_state(msg);
    state->GPS_state = mavlink_msg_state_get_GPS_state(msg);
    state->globalStage = mavlink_msg_state_get_globalStage(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_STATE_LEN? msg->len : MAVLINK_MSG_ID_STATE_LEN;
        memset(state, 0, MAVLINK_MSG_ID_STATE_LEN);
    memcpy(state, _MAV_PAYLOAD(msg), len);
#endif
}
