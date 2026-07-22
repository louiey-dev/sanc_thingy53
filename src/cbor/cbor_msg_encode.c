/*
 * Generated using zcbor version 0.9.1
 * https://github.com/NordicSemiconductor/zcbor
 * Generated with a --default-max-qty of 3
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "zcbor_encode.h"
#include "cbor_msg_encode.h"
#include "zcbor_print.h"

#if DEFAULT_MAX_QTY != 3
#error "The type file was generated with a different default_max_qty than this file"
#endif

#define log_result(state, result, func) do { \
	if (!result) { \
		zcbor_trace_file(state); \
		zcbor_log("%s error: %s\r\n", func, zcbor_error_str(zcbor_peek_error(state))); \
	} else { \
		zcbor_log("%s success\r\n", func); \
	} \
} while(0)

static bool encode_Statistics(zcbor_state_t *state, const struct Statistics *input);
static bool encode_SensorData(zcbor_state_t *state, const struct SensorData *input);
static bool encode_CpuTelemetry(zcbor_state_t *state, const struct CpuTelemetry *input);


static bool encode_Statistics(
		zcbor_state_t *state, const struct Statistics *input)
{
	zcbor_log("%s\r\n", __func__);

	bool res = (((zcbor_map_start_encode(state, 5) && (((((zcbor_uint32_put(state, (0))))
	&& (zcbor_uint32_encode(state, (&(*input).Statistics_uint0uint))))
	&& (((zcbor_uint32_put(state, (1))))
	&& (zcbor_uint32_encode(state, (&(*input).Statistics_uint1uint))))
	&& (((zcbor_uint32_put(state, (2))))
	&& (zcbor_uint32_encode(state, (&(*input).Statistics_uint2uint))))
	&& (((zcbor_uint32_put(state, (3))))
	&& (zcbor_uint32_encode(state, (&(*input).Statistics_uint3uint))))
	&& (((zcbor_uint32_put(state, (4))))
	&& (zcbor_float32_encode(state, (&(*input).Statistics_uint4float))))) || (zcbor_list_map_end_force_encode(state), false)) && zcbor_map_end_encode(state, 5))));

	log_result(state, res, __func__);
	return res;
}

static bool encode_SensorData(
		zcbor_state_t *state, const struct SensorData *input)
{
	zcbor_log("%s\r\n", __func__);

	bool res = (((zcbor_map_start_encode(state, 20) && (((((zcbor_uint32_put(state, (0))))
	&& (zcbor_uint32_encode(state, (&(*input).SensorData_uint0uint))))
	&& (((zcbor_uint32_put(state, (1))))
	&& (zcbor_uint32_encode(state, (&(*input).SensorData_uint1uint))))
	&& (((zcbor_uint32_put(state, (2))))
	&& (zcbor_uint32_encode(state, (&(*input).SensorData_uint2uint))))
	&& (((zcbor_uint32_put(state, (3))))
	&& (zcbor_uint32_encode(state, (&(*input).SensorData_uint3uint))))
	&& (((zcbor_uint32_put(state, (4))))
	&& (zcbor_uint32_encode(state, (&(*input).SensorData_uint4uint))))
	&& (((zcbor_uint32_put(state, (5))))
	&& (zcbor_uint32_encode(state, (&(*input).SensorData_uint5uint))))
	&& (((zcbor_uint32_put(state, (6))))
	&& (zcbor_float32_encode(state, (&(*input).SensorData_uint6float))))
	&& (((zcbor_uint32_put(state, (7))))
	&& (zcbor_float32_encode(state, (&(*input).SensorData_uint7float))))
	&& (((zcbor_uint32_put(state, (8))))
	&& (zcbor_float32_encode(state, (&(*input).SensorData_uint8float))))
	&& (((zcbor_uint32_put(state, (9))))
	&& (zcbor_uint32_encode(state, (&(*input).SensorData_uint9uint))))
	&& (((zcbor_uint32_put(state, (10))))
	&& (zcbor_float32_encode(state, (&(*input).SensorData_uint10float))))
	&& (((zcbor_uint32_put(state, (11))))
	&& (zcbor_float32_encode(state, (&(*input).SensorData_uint11float))))
	&& (((zcbor_uint32_put(state, (12))))
	&& (zcbor_float32_encode(state, (&(*input).SensorData_uint12float))))
	&& (((zcbor_uint32_put(state, (13))))
	&& (zcbor_float32_encode(state, (&(*input).SensorData_uint13float))))
	&& (((zcbor_uint32_put(state, (14))))
	&& (zcbor_float32_encode(state, (&(*input).SensorData_uint14float))))
	&& (((zcbor_uint32_put(state, (15))))
	&& (zcbor_float32_encode(state, (&(*input).SensorData_uint15float))))
	&& (((zcbor_uint32_put(state, (16))))
	&& (zcbor_float32_encode(state, (&(*input).SensorData_uint16float))))
	&& (((zcbor_uint32_put(state, (17))))
	&& (zcbor_float32_encode(state, (&(*input).SensorData_uint17float))))
	&& (((zcbor_uint32_put(state, (18))))
	&& (zcbor_float32_encode(state, (&(*input).SensorData_uint18float))))
	&& (((zcbor_uint32_put(state, (19))))
	&& (zcbor_uint32_encode(state, (&(*input).SensorData_uint19uint))))) || (zcbor_list_map_end_force_encode(state), false)) && zcbor_map_end_encode(state, 20))));

	log_result(state, res, __func__);
	return res;
}

static bool encode_CpuTelemetry(
		zcbor_state_t *state, const struct CpuTelemetry *input)
{
	zcbor_log("%s\r\n", __func__);

	bool res = (((zcbor_map_start_encode(state, 5) && (((((zcbor_uint32_put(state, (0))))
	&& (zcbor_uint32_encode(state, (&(*input).CpuTelemetry_uint0uint))))
	&& (((zcbor_uint32_put(state, (1))))
	&& (zcbor_uint32_encode(state, (&(*input).CpuTelemetry_uint1uint))))
	&& (((zcbor_uint32_put(state, (2))))
	&& (zcbor_float32_encode(state, (&(*input).CpuTelemetry_uint2float))))
	&& (((zcbor_uint32_put(state, (3))))
	&& (zcbor_float32_encode(state, (&(*input).CpuTelemetry_uint3float))))
	&& (((zcbor_uint32_put(state, (4))))
	&& (zcbor_uint32_encode(state, (&(*input).CpuTelemetry_uint4uint))))) || (zcbor_list_map_end_force_encode(state), false)) && zcbor_map_end_encode(state, 5))));

	log_result(state, res, __func__);
	return res;
}



int cbor_encode_CpuTelemetry(
		uint8_t *payload, size_t payload_len,
		const struct CpuTelemetry *input,
		size_t *payload_len_out)
{
	zcbor_state_t states[3];

	return zcbor_entry_function(payload, payload_len, (void *)input, payload_len_out, states,
		(zcbor_decoder_t *)encode_CpuTelemetry, sizeof(states) / sizeof(zcbor_state_t), 1);
}


int cbor_encode_SensorData(
		uint8_t *payload, size_t payload_len,
		const struct SensorData *input,
		size_t *payload_len_out)
{
	zcbor_state_t states[3];

	return zcbor_entry_function(payload, payload_len, (void *)input, payload_len_out, states,
		(zcbor_decoder_t *)encode_SensorData, sizeof(states) / sizeof(zcbor_state_t), 1);
}


int cbor_encode_Statistics(
		uint8_t *payload, size_t payload_len,
		const struct Statistics *input,
		size_t *payload_len_out)
{
	zcbor_state_t states[3];

	return zcbor_entry_function(payload, payload_len, (void *)input, payload_len_out, states,
		(zcbor_decoder_t *)encode_Statistics, sizeof(states) / sizeof(zcbor_state_t), 1);
}
