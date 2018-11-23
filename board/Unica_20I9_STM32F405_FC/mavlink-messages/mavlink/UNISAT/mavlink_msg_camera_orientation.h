#pragma once
// MESSAGE Camera_Orientation PACKING

#define MAVLINK_MSG_ID_Camera_Orientation 155

MAVPACKED(
typedef struct __mavlink_camera_orientation_t {
 float servo_pos; /*< position of servo motor*/
 float step_engine_pos; /*< position of step angine*/
 float time; /*< current time*/
}) mavlink_camera_orientation_t;

#define MAVLINK_MSG_ID_Camera_Orientation_LEN 12
#define MAVLINK_MSG_ID_Camera_Orientation_MIN_LEN 12
#define MAVLINK_MSG_ID_155_LEN 12
#define MAVLINK_MSG_ID_155_MIN_LEN 12

#define MAVLINK_MSG_ID_Camera_Orientation_CRC 159
#define MAVLINK_MSG_ID_155_CRC 159



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_Camera_Orientation { \
    155, \
    "Camera_Orientation", \
    3, \
    {  { "servo_pos", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_camera_orientation_t, servo_pos) }, \
         { "step_engine_pos", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_camera_orientation_t, step_engine_pos) }, \
         { "time", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_camera_orientation_t, time) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_Camera_Orientation { \
    "Camera_Orientation", \
    3, \
    {  { "servo_pos", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_camera_orientation_t, servo_pos) }, \
         { "step_engine_pos", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_camera_orientation_t, step_engine_pos) }, \
         { "time", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_camera_orientation_t, time) }, \
         } \
}
#endif

/**
 * @brief Pack a camera_orientation message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param servo_pos position of servo motor
 * @param step_engine_pos position of step angine
 * @param time current time
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_camera_orientation_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               float servo_pos, float step_engine_pos, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_Camera_Orientation_LEN];
    _mav_put_float(buf, 0, servo_pos);
    _mav_put_float(buf, 4, step_engine_pos);
    _mav_put_float(buf, 8, time);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_Camera_Orientation_LEN);
#else
    mavlink_camera_orientation_t packet;
    packet.servo_pos = servo_pos;
    packet.step_engine_pos = step_engine_pos;
    packet.time = time;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_Camera_Orientation_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_Camera_Orientation;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_Camera_Orientation_MIN_LEN, MAVLINK_MSG_ID_Camera_Orientation_LEN, MAVLINK_MSG_ID_Camera_Orientation_CRC);
}

/**
 * @brief Pack a camera_orientation message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param servo_pos position of servo motor
 * @param step_engine_pos position of step angine
 * @param time current time
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_camera_orientation_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   float servo_pos,float step_engine_pos,float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_Camera_Orientation_LEN];
    _mav_put_float(buf, 0, servo_pos);
    _mav_put_float(buf, 4, step_engine_pos);
    _mav_put_float(buf, 8, time);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_Camera_Orientation_LEN);
#else
    mavlink_camera_orientation_t packet;
    packet.servo_pos = servo_pos;
    packet.step_engine_pos = step_engine_pos;
    packet.time = time;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_Camera_Orientation_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_Camera_Orientation;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_Camera_Orientation_MIN_LEN, MAVLINK_MSG_ID_Camera_Orientation_LEN, MAVLINK_MSG_ID_Camera_Orientation_CRC);
}

/**
 * @brief Encode a camera_orientation struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param camera_orientation C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_camera_orientation_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_camera_orientation_t* camera_orientation)
{
    return mavlink_msg_camera_orientation_pack(system_id, component_id, msg, camera_orientation->servo_pos, camera_orientation->step_engine_pos, camera_orientation->time);
}

/**
 * @brief Encode a camera_orientation struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param camera_orientation C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_camera_orientation_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_camera_orientation_t* camera_orientation)
{
    return mavlink_msg_camera_orientation_pack_chan(system_id, component_id, chan, msg, camera_orientation->servo_pos, camera_orientation->step_engine_pos, camera_orientation->time);
}

/**
 * @brief Send a camera_orientation message
 * @param chan MAVLink channel to send the message
 *
 * @param servo_pos position of servo motor
 * @param step_engine_pos position of step angine
 * @param time current time
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_camera_orientation_send(mavlink_channel_t chan, float servo_pos, float step_engine_pos, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_Camera_Orientation_LEN];
    _mav_put_float(buf, 0, servo_pos);
    _mav_put_float(buf, 4, step_engine_pos);
    _mav_put_float(buf, 8, time);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_Camera_Orientation, buf, MAVLINK_MSG_ID_Camera_Orientation_MIN_LEN, MAVLINK_MSG_ID_Camera_Orientation_LEN, MAVLINK_MSG_ID_Camera_Orientation_CRC);
#else
    mavlink_camera_orientation_t packet;
    packet.servo_pos = servo_pos;
    packet.step_engine_pos = step_engine_pos;
    packet.time = time;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_Camera_Orientation, (const char *)&packet, MAVLINK_MSG_ID_Camera_Orientation_MIN_LEN, MAVLINK_MSG_ID_Camera_Orientation_LEN, MAVLINK_MSG_ID_Camera_Orientation_CRC);
#endif
}

/**
 * @brief Send a camera_orientation message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_camera_orientation_send_struct(mavlink_channel_t chan, const mavlink_camera_orientation_t* camera_orientation)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_camera_orientation_send(chan, camera_orientation->servo_pos, camera_orientation->step_engine_pos, camera_orientation->time);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_Camera_Orientation, (const char *)camera_orientation, MAVLINK_MSG_ID_Camera_Orientation_MIN_LEN, MAVLINK_MSG_ID_Camera_Orientation_LEN, MAVLINK_MSG_ID_Camera_Orientation_CRC);
#endif
}

#if MAVLINK_MSG_ID_Camera_Orientation_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_camera_orientation_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  float servo_pos, float step_engine_pos, float time)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, servo_pos);
    _mav_put_float(buf, 4, step_engine_pos);
    _mav_put_float(buf, 8, time);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_Camera_Orientation, buf, MAVLINK_MSG_ID_Camera_Orientation_MIN_LEN, MAVLINK_MSG_ID_Camera_Orientation_LEN, MAVLINK_MSG_ID_Camera_Orientation_CRC);
#else
    mavlink_camera_orientation_t *packet = (mavlink_camera_orientation_t *)msgbuf;
    packet->servo_pos = servo_pos;
    packet->step_engine_pos = step_engine_pos;
    packet->time = time;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_Camera_Orientation, (const char *)packet, MAVLINK_MSG_ID_Camera_Orientation_MIN_LEN, MAVLINK_MSG_ID_Camera_Orientation_LEN, MAVLINK_MSG_ID_Camera_Orientation_CRC);
#endif
}
#endif

#endif

// MESSAGE Camera_Orientation UNPACKING


/**
 * @brief Get field servo_pos from camera_orientation message
 *
 * @return position of servo motor
 */
static inline float mavlink_msg_camera_orientation_get_servo_pos(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field step_engine_pos from camera_orientation message
 *
 * @return position of step angine
 */
static inline float mavlink_msg_camera_orientation_get_step_engine_pos(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field time from camera_orientation message
 *
 * @return current time
 */
static inline float mavlink_msg_camera_orientation_get_time(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Decode a camera_orientation message into a struct
 *
 * @param msg The message to decode
 * @param camera_orientation C-struct to decode the message contents into
 */
static inline void mavlink_msg_camera_orientation_decode(const mavlink_message_t* msg, mavlink_camera_orientation_t* camera_orientation)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    camera_orientation->servo_pos = mavlink_msg_camera_orientation_get_servo_pos(msg);
    camera_orientation->step_engine_pos = mavlink_msg_camera_orientation_get_step_engine_pos(msg);
    camera_orientation->time = mavlink_msg_camera_orientation_get_time(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_Camera_Orientation_LEN? msg->len : MAVLINK_MSG_ID_Camera_Orientation_LEN;
        memset(camera_orientation, 0, MAVLINK_MSG_ID_Camera_Orientation_LEN);
    memcpy(camera_orientation, _MAV_PAYLOAD(msg), len);
#endif
}
