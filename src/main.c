/*
 * Copyright (c) 2024 Croxel, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/services/nus.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/logging/log.h>

#include "bsp.h"

const char *FW_VER_STRING = "v0.1.0";

#define DEVICE_NAME		CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN		(sizeof(DEVICE_NAME) - 1)

extern BSP_ST g_Bsp;

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_NUS_SRV_VAL),
};


static struct k_work adv_work;
    
static void start_adv_handler(struct k_work *work)
{
	int err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err) {
		LOG_ERR("Failed to start advertising: %d\n", err);
	} else {
		LOG_INF("Advertising started\n");
	}
}

// MTU updated callback
static void mtu_updated(struct bt_conn *conn, uint16_t tx, uint16_t rx)
{
	LOG_INF("ATT MTU exchange complete. New MTU: TX %d RX %d (Max payload: %d)", 
			tx, rx, tx - 3);
}

static struct bt_gatt_cb gatt_callbacks = {
	.att_mtu_updated = mtu_updated,
};
///////////////////////////////////////////////////////////////////////////////

static void notif_enabled(bool enabled, void *ctx)
{
	ARG_UNUSED(ctx);

	LOG_INF("%s() - %s\n", __func__, (enabled ? "Enabled" : "Disabled"));
}

static void received(struct bt_conn *conn, const void *data, uint16_t len, void *ctx)
{
	ARG_UNUSED(conn);
	ARG_UNUSED(ctx);

	LOG_INF("%s() - Len: %d, Message: %.*s\n", __func__, len, len, (char *)data);

	bsp_msg_parser((const char *)data, len);
}

struct bt_nus_cb nus_listener = {
	.notif_enabled = notif_enabled,
	.received = received,
};

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err) {
		LOG_ERR("BLE Connection failed: %d\n", err);
	} else {
		LOG_INF("BLE Connected\n");
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("BLE Disconnected: %d\n", reason);

	k_work_submit(&adv_work);
}

BT_CONN_CB_DEFINE(conn_callbacks) = {
	.connected = connected,
	.disconnected = disconnected,
};

int main(void)
{
	int err;

	LOG_INF("sanc_thingy53 NUS Application\n");
	LOG_INF("Firmware Version: %s\n", FW_VER_STRING);

	bsp_init();

	k_work_init(&adv_work, start_adv_handler);

	err = bt_nus_cb_register(&nus_listener, NULL);
	if (err) {
		LOG_ERR("Failed to register NUS callback: %d\n", err);
		return err;
	}

	bt_gatt_cb_register(&gatt_callbacks);

	err = bt_enable(NULL);
	if (err) {
		LOG_ERR("Failed to enable bluetooth: %d\n", err);
		return err;
	}

	err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err) {
		LOG_ERR("Failed to start advertising: %d\n", err);
		return err;
	}

	LOG_INF("Initialization complete\n");

	while (true) {
		const char *hello_world = "Hello World!\n";

		k_sleep(K_SECONDS(g_Bsp.nus_duration));

		err = bt_nus_send(NULL, hello_world, strlen(hello_world));
		LOG_INF("Data send - Result: %d\n", err);

		if (err < 0 && (err != -EAGAIN) && (err != -ENOTCONN)) {
			LOG_ERR("Failed to send data over BLE NUS: %d\n", err);
			return err;
		}
	}

	return 0;
}
