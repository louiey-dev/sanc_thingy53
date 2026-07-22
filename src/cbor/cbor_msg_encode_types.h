/*
 * Generated using zcbor version 0.9.1
 * https://github.com/NordicSemiconductor/zcbor
 * Generated with a --default-max-qty of 3
 */

#ifndef CBOR_MSG_ENCODE_TYPES_H__
#define CBOR_MSG_ENCODE_TYPES_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

/** Which value for --default-max-qty this file was created with.
 *
 *  The define is used in the other generated file to do a build-time
 *  compatibility check.
 *
 *  See `zcbor --help` for more information about --default-max-qty
 */
#define DEFAULT_MAX_QTY 3

struct CpuTelemetry {
	uint32_t CpuTelemetry_uint0uint;
	uint32_t CpuTelemetry_uint1uint;
	float CpuTelemetry_uint2float;
	float CpuTelemetry_uint3float;
	uint32_t CpuTelemetry_uint4uint;
};

struct SensorData {
	uint32_t SensorData_uint0uint;
	uint32_t SensorData_uint1uint;
	uint32_t SensorData_uint2uint;
	uint32_t SensorData_uint3uint;
	uint32_t SensorData_uint4uint;
	uint32_t SensorData_uint5uint;
	float SensorData_uint6float;
	float SensorData_uint7float;
	float SensorData_uint8float;
	uint32_t SensorData_uint9uint;
	float SensorData_uint10float;
	float SensorData_uint11float;
	float SensorData_uint12float;
	float SensorData_uint13float;
	float SensorData_uint14float;
	float SensorData_uint15float;
	float SensorData_uint16float;
	float SensorData_uint17float;
	float SensorData_uint18float;
	uint32_t SensorData_uint19uint;
};

struct Statistics {
	uint32_t Statistics_uint0uint;
	uint32_t Statistics_uint1uint;
	uint32_t Statistics_uint2uint;
	uint32_t Statistics_uint3uint;
	float Statistics_uint4float;
};

#ifdef __cplusplus
}
#endif

#endif /* CBOR_MSG_ENCODE_TYPES_H__ */
