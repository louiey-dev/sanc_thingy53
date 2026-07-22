/*
 * Generated using zcbor version 0.9.1
 * https://github.com/NordicSemiconductor/zcbor
 * Generated with a --default-max-qty of 3
 */

#ifndef CBOR_MSG_ENCODE_H__
#define CBOR_MSG_ENCODE_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "cbor_msg_encode_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#if DEFAULT_MAX_QTY != 3
#error "The type file was generated with a different default_max_qty than this file"
#endif


int cbor_encode_CpuTelemetry(
		uint8_t *payload, size_t payload_len,
		const struct CpuTelemetry *input,
		size_t *payload_len_out);


int cbor_encode_SensorData(
		uint8_t *payload, size_t payload_len,
		const struct SensorData *input,
		size_t *payload_len_out);


int cbor_encode_Statistics(
		uint8_t *payload, size_t payload_len,
		const struct Statistics *input,
		size_t *payload_len_out);


#ifdef __cplusplus
}
#endif

#endif /* CBOR_MSG_ENCODE_H__ */
