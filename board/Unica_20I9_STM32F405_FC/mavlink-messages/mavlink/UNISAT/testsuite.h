/** @file
 *    @brief MAVLink comm protocol testsuite generated from UNISAT.xml
 *    @see http://qgroundcontrol.org/mavlink/
 */
#pragma once
#ifndef UNISAT_TESTSUITE_H
#define UNISAT_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL
static void mavlink_test_common(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_UNISAT(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_common(system_id, component_id, last_msg);
    mavlink_test_UNISAT(system_id, component_id, last_msg);
}
#endif

#include "../common/testsuite.h"


static void mavlink_test_state(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_STATE >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_state_t packet_in = {
        17.0,17,84,151,218,29,96,163
    };
    mavlink_state_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time = packet_in.time;
        packet1.MPU_state = packet_in.MPU_state;
        packet1.BMP_state = packet_in.BMP_state;
        packet1.SD_state = packet_in.SD_state;
        packet1.NRF_state = packet_in.NRF_state;
        packet1.MOTOR_state = packet_in.MOTOR_state;
        packet1.GPS_state = packet_in.GPS_state;
        packet1.globalStage = packet_in.globalStage;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_STATE_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_STATE_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_state_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_state_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_state_pack(system_id, component_id, &msg , packet1.time , packet1.MPU_state , packet1.BMP_state , packet1.SD_state , packet1.NRF_state , packet1.MOTOR_state , packet1.GPS_state , packet1.globalStage );
    mavlink_msg_state_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_state_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time , packet1.MPU_state , packet1.BMP_state , packet1.SD_state , packet1.NRF_state , packet1.MOTOR_state , packet1.GPS_state , packet1.globalStage );
    mavlink_msg_state_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_state_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_state_send(MAVLINK_COMM_1 , packet1.time , packet1.MPU_state , packet1.BMP_state , packet1.SD_state , packet1.NRF_state , packet1.MOTOR_state , packet1.GPS_state , packet1.globalStage );
    mavlink_msg_state_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_imu_rsc(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_IMU_rsc >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_imu_rsc_t packet_in = {
        { 17.0, 18.0, 19.0 },{ 101.0, 102.0, 103.0 },{ 185.0, 186.0, 187.0 },269.0
    };
    mavlink_imu_rsc_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time = packet_in.time;
        
        mav_array_memcpy(packet1.accel, packet_in.accel, sizeof(float)*3);
        mav_array_memcpy(packet1.gyro, packet_in.gyro, sizeof(float)*3);
        mav_array_memcpy(packet1.compass, packet_in.compass, sizeof(float)*3);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_IMU_rsc_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_IMU_rsc_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_imu_rsc_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_imu_rsc_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_imu_rsc_pack(system_id, component_id, &msg , packet1.accel , packet1.gyro , packet1.compass , packet1.time );
    mavlink_msg_imu_rsc_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_imu_rsc_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.accel , packet1.gyro , packet1.compass , packet1.time );
    mavlink_msg_imu_rsc_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_imu_rsc_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_imu_rsc_send(MAVLINK_COMM_1 , packet1.accel , packet1.gyro , packet1.compass , packet1.time );
    mavlink_msg_imu_rsc_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_imu_isc(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_IMU_isc >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_imu_isc_t packet_in = {
        { 17.0, 18.0, 19.0 },{ 101.0, 102.0, 103.0 },{ 185.0, 186.0, 187.0 },{ 269.0, 270.0, 271.0 },{ 353.0, 354.0, 355.0, 356.0 },465.0
    };
    mavlink_imu_isc_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time = packet_in.time;
        
        mav_array_memcpy(packet1.accel, packet_in.accel, sizeof(float)*3);
        mav_array_memcpy(packet1.compass, packet_in.compass, sizeof(float)*3);
        mav_array_memcpy(packet1.velocities, packet_in.velocities, sizeof(float)*3);
        mav_array_memcpy(packet1.coordinates, packet_in.coordinates, sizeof(float)*3);
        mav_array_memcpy(packet1.quaternion, packet_in.quaternion, sizeof(float)*4);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_IMU_isc_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_IMU_isc_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_imu_isc_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_imu_isc_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_imu_isc_pack(system_id, component_id, &msg , packet1.accel , packet1.compass , packet1.velocities , packet1.coordinates , packet1.quaternion , packet1.time );
    mavlink_msg_imu_isc_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_imu_isc_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.accel , packet1.compass , packet1.velocities , packet1.coordinates , packet1.quaternion , packet1.time );
    mavlink_msg_imu_isc_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_imu_isc_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_imu_isc_send(MAVLINK_COMM_1 , packet1.accel , packet1.compass , packet1.velocities , packet1.coordinates , packet1.quaternion , packet1.time );
    mavlink_msg_imu_isc_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_sensors(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_Sensors >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_sensors_t packet_in = {
        17.0,45.0,73.0,101.0
    };
    mavlink_sensors_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.temp = packet_in.temp;
        packet1.pressure = packet_in.pressure;
        packet1.height = packet_in.height;
        packet1.time = packet_in.time;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_Sensors_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_Sensors_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_sensors_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_sensors_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_sensors_pack(system_id, component_id, &msg , packet1.temp , packet1.pressure , packet1.height , packet1.time );
    mavlink_msg_sensors_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_sensors_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.temp , packet1.pressure , packet1.height , packet1.time );
    mavlink_msg_sensors_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_sensors_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_sensors_send(MAVLINK_COMM_1 , packet1.temp , packet1.pressure , packet1.height , packet1.time );
    mavlink_msg_sensors_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_gps(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_GPS >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_gps_t packet_in = {
        { 17.0, 18.0, 19.0 },101.0
    };
    mavlink_gps_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time = packet_in.time;
        
        mav_array_memcpy(packet1.coordinates, packet_in.coordinates, sizeof(float)*3);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_GPS_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_GPS_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_gps_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_gps_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_gps_pack(system_id, component_id, &msg , packet1.coordinates , packet1.time );
    mavlink_msg_gps_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_gps_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.coordinates , packet1.time );
    mavlink_msg_gps_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_gps_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_gps_send(MAVLINK_COMM_1 , packet1.coordinates , packet1.time );
    mavlink_msg_gps_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_camera_orientation(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_Camera_Orientation >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_camera_orientation_t packet_in = {
        17.0,45.0,73.0
    };
    mavlink_camera_orientation_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.servo_pos = packet_in.servo_pos;
        packet1.step_engine_pos = packet_in.step_engine_pos;
        packet1.time = packet_in.time;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_Camera_Orientation_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_Camera_Orientation_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_camera_orientation_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_camera_orientation_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_camera_orientation_pack(system_id, component_id, &msg , packet1.servo_pos , packet1.step_engine_pos , packet1.time );
    mavlink_msg_camera_orientation_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_camera_orientation_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.servo_pos , packet1.step_engine_pos , packet1.time );
    mavlink_msg_camera_orientation_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_camera_orientation_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_camera_orientation_send(MAVLINK_COMM_1 , packet1.servo_pos , packet1.step_engine_pos , packet1.time );
    mavlink_msg_camera_orientation_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_state_zero(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_STATE_ZERO >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_state_zero_t packet_in = {
        17.0,{ 45.0, 46.0, 47.0, 48.0 },{ 157.0, 158.0, 159.0 },{ 241.0, 242.0, 243.0 },{ 325.0, 326.0, 327.0 },409.0
    };
    mavlink_state_zero_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.zero_pressure = packet_in.zero_pressure;
        packet1.time = packet_in.time;
        
        mav_array_memcpy(packet1.zero_quaternion, packet_in.zero_quaternion, sizeof(float)*4);
        mav_array_memcpy(packet1.zero_GPS, packet_in.zero_GPS, sizeof(float)*3);
        mav_array_memcpy(packet1.gyro_staticShift, packet_in.gyro_staticShift, sizeof(float)*3);
        mav_array_memcpy(packet1.accel_staticShift, packet_in.accel_staticShift, sizeof(float)*3);
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_STATE_ZERO_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_STATE_ZERO_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_state_zero_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_state_zero_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_state_zero_pack(system_id, component_id, &msg , packet1.zero_pressure , packet1.zero_quaternion , packet1.zero_GPS , packet1.gyro_staticShift , packet1.accel_staticShift , packet1.time );
    mavlink_msg_state_zero_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_state_zero_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.zero_pressure , packet1.zero_quaternion , packet1.zero_GPS , packet1.gyro_staticShift , packet1.accel_staticShift , packet1.time );
    mavlink_msg_state_zero_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_state_zero_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_state_zero_send(MAVLINK_COMM_1 , packet1.zero_pressure , packet1.zero_quaternion , packet1.zero_GPS , packet1.gyro_staticShift , packet1.accel_staticShift , packet1.time );
    mavlink_msg_state_zero_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_atmega(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_ATMEGA >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_atmega_t packet_in = {
        17.0,45.0,73.0,101.0,53,120,187,254
    };
    mavlink_atmega_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time = packet_in.time;
        packet1.pressure = packet_in.pressure;
        packet1.height = packet_in.height;
        packet1.temp = packet_in.temp;
        packet1.baro_state = packet_in.baro_state;
        packet1.motor_state = packet_in.motor_state;
        packet1.para_state = packet_in.para_state;
        packet1.globalStage = packet_in.globalStage;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_ATMEGA_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_ATMEGA_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_atmega_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_atmega_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_atmega_pack(system_id, component_id, &msg , packet1.time , packet1.pressure , packet1.height , packet1.temp , packet1.baro_state , packet1.motor_state , packet1.para_state , packet1.globalStage );
    mavlink_msg_atmega_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_atmega_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time , packet1.pressure , packet1.height , packet1.temp , packet1.baro_state , packet1.motor_state , packet1.para_state , packet1.globalStage );
    mavlink_msg_atmega_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_atmega_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_atmega_send(MAVLINK_COMM_1 , packet1.time , packet1.pressure , packet1.height , packet1.temp , packet1.baro_state , packet1.motor_state , packet1.para_state , packet1.globalStage );
    mavlink_msg_atmega_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_cmd(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_CMD >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_cmd_t packet_in = {
        5
    };
    mavlink_cmd_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.cmd = packet_in.cmd;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_CMD_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_CMD_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_cmd_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_cmd_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_cmd_pack(system_id, component_id, &msg , packet1.cmd );
    mavlink_msg_cmd_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_cmd_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.cmd );
    mavlink_msg_cmd_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_cmd_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_cmd_send(MAVLINK_COMM_1 , packet1.cmd );
    mavlink_msg_cmd_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_UNISAT(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_state(system_id, component_id, last_msg);
    mavlink_test_imu_rsc(system_id, component_id, last_msg);
    mavlink_test_imu_isc(system_id, component_id, last_msg);
    mavlink_test_sensors(system_id, component_id, last_msg);
    mavlink_test_gps(system_id, component_id, last_msg);
    mavlink_test_camera_orientation(system_id, component_id, last_msg);
    mavlink_test_state_zero(system_id, component_id, last_msg);
    mavlink_test_atmega(system_id, component_id, last_msg);
    mavlink_test_cmd(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // UNISAT_TESTSUITE_H
