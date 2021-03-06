#ifndef ULINK_PSO_H
#define ULINK_PSO_H

#include <stdint.h>

#define ULINK_MSG_ID_PSO_COMMAND_LEN 6
#define ULINK_MSG_ID_151_LEN 6

#define ULINK_MSG_ID_PSO_COMMAND_CRC 141
#define ULINK_MSG_ID_151_CRC 141

#define ULINK_MSG_ID_PSO_DATA 150
#define ULINK_MSG_ID_PSO_DATA_LEN 14
#define ULINK_MSG_ID_150_LEN 14

#define ULINK_MSG_ID_PSO_DATA_CRC 5
#define ULINK_MSG_ID_150_CRC 5


typedef struct __ulink_pso_data_t
{
 uint8_t throttle; /*< Throttle  (0 ~ 100)*/
 uint16_t index; /*< Frame index */
 int16_t rpm; /*< Propeller rotation (rpm)*/
 int16_t v_motor; /*< Motor voltage (V)*/
 int16_t i_motor; /*< Motor current (A)*/
 int16_t thrust; /*< Thrust generated by the propulsion system (N)*/
 int16_t accel[3]; /*< Accelerometer readings (raw)*/
} ulink_pso_data_t;

typedef struct __ulink_pso_command_t
{
 uint16_t state; /*< Aileron command, 0 to 65535*/
 uint16_t throttle; /*< Throttle command, 0 to 65535*/
 uint16_t debug; /*< Debug command, 0 to 65535*/
} ulink_pso_command_t;

#endif // ULINK_PSO_H
