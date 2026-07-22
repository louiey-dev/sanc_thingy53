zcbor code -c cbor_msg.cddl \
    -t CpuTelemetry SensorData Statistics \
    --output-c cbor_msg_encode.c \
    --output-h cbor_msg_encode.h \
    -e