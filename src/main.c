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
#include <zephyr/mgmt/mcumgr/transport/smp_bt.h>

#include <app_version.h>

#include "bsp.h"
#include "bsp_msg_parser.h"

#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)

extern BSP_ST g_Bsp;

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, SMP_BT_SVC_UUID_VAL),
};

static struct k_work adv_work;

static void start_adv_handler(struct k_work *work)
{
	int err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err)
	{
		LOG_ERR("Failed to start advertising: %d\n", err);
	}
	else
	{
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

	g_Bsp.nus_notif_enabled = enabled ? 1 : 0;
	LOG_INF("%s() - %s\n", __func__, (enabled ? "Enabled" : "Disabled"));
}

struct nus_rx_msg
{
	uint8_t data[MAX_PAYLOAD];
	uint16_t len;
};

#define MSG_QUEUE_SIZE 8
K_MSGQ_DEFINE(nus_rx_msgq, sizeof(struct nus_rx_msg), MSG_QUEUE_SIZE, 4);

#define NUS_RX_THREAD_STACK_SIZE 2048
#define NUS_RX_THREAD_PRIORITY 8

K_THREAD_STACK_DEFINE(nus_rx_thread_stack, NUS_RX_THREAD_STACK_SIZE);
static struct k_thread nus_rx_thread_data;

static void nus_rx_thread_entry(void *unused1, void *unused2, void *unused3)
{
	struct nus_rx_msg rx_msg;
	while (1)
	{
		if (k_msgq_get(&nus_rx_msgq, &rx_msg, K_FOREVER) == 0)
		{
			bsp_msg_parser((const char *)rx_msg.data, rx_msg.len);
		}
	}
}

static void received(struct bt_conn *conn, const void *data, uint16_t len, void *ctx)
{
	struct nus_rx_msg rx_msg;

	ARG_UNUSED(conn);
	ARG_UNUSED(ctx);

	LOG_INF("%s() - Len: %d, Message: %.*s\n", __func__, len, len, (char *)data);

	if (len > MAX_PAYLOAD)
	{
		LOG_ERR("Received data exceeds MAX_PAYLOAD");
		return;
	}

	memcpy(rx_msg.data, data, len);
	rx_msg.len = len;

	int err = k_msgq_put(&nus_rx_msgq, &rx_msg, K_NO_WAIT);
	if (err)
	{
		LOG_ERR("Failed to queue NUS RX message: %d", err);
	}
}

struct bt_nus_cb nus_listener = {
	.notif_enabled = notif_enabled,
	.received = received,
};

static void connected(struct bt_conn *conn, uint8_t err)
{
	if (err)
	{
		LOG_ERR("BLE Connection failed: %d\n", err);
	}
	else
	{
		LOG_INF("BLE Connected\n");
	}
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("BLE Disconnected: %d\n", reason);

	g_Bsp.nus_notif_enabled = 0;
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
	LOG_INF("Firmware Version: %s\n", APP_VERSION_STRING);

	bsp_init();

	k_work_init(&adv_work, start_adv_handler);

	err = bt_nus_cb_register(&nus_listener, NULL);
	if (err)
	{
		LOG_ERR("Failed to register NUS callback: %d\n", err);
		return err;
	}

	bt_gatt_cb_register(&gatt_callbacks);

	err = bt_enable(NULL);
	if (err)
	{
		LOG_ERR("Failed to enable bluetooth: %d\n", err);
		return err;
	}

	err = bt_le_adv_start(BT_LE_ADV_CONN_FAST_1, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err)
	{
		LOG_ERR("Failed to start advertising: %d\n", err);
		return err;
	}

	LOG_INF("Initialization complete\n");

	k_thread_create(&nus_rx_thread_data, nus_rx_thread_stack,
					K_THREAD_STACK_SIZEOF(nus_rx_thread_stack),
					nus_rx_thread_entry, NULL, NULL, NULL,
					NUS_RX_THREAD_PRIORITY, 0, K_NO_WAIT);
#if 0
	while (true)
	{
		const char *hello_world = "Hello World!\n";

		k_sleep(K_SECONDS(g_Bsp.nus_duration));

		if(g_Bsp.nus_notif_enabled)
		{
			err = bsp_nus_send(hello_world, strlen(hello_world));

			if (err < 0 && (err != -EAGAIN) && (err != -ENOTCONN))
			{
				LOG_ERR("Failed to send data over BLE NUS: %d\n", err);
				// return err;
			}
		}
	}
#endif
	return 0;
}
