/**
 * Copyright (c) 2015 Benoît Taine
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * @file virtio_can.c
 * @author Benoît Taine (benoit.taine@openwide.fr)
 * @brief VirtIO based CAN controller emulator.
 */

#include <vmm_error.h>
#include <vmm_heap.h>
#include <vmm_modules.h>
#include <vmm_devemu.h>

#include <emu/virtio.h>
#include <emu/virtio_can.h>

#define MODULE_DESC			"VirtIO CAN Emulator"
#define MODULE_AUTHOR			"Benoit Taine"
#define MODULE_LICENSE			"GPL"
#define MODULE_IPRIORITY		(VIRTIO_IPRIORITY + 1)
#define MODULE_INIT			virtio_can_init
#define MODULE_EXIT			virtio_can_exit

struct virtio_can_dev {
	struct virtio_device *vdev;

	struct virtio_queue vq_ctrl;
	struct virtio_queue *vqs_rx;
	struct virtio_queue *vqs_tx;
	u32 feature;

	char name[VIRTIO_DEVICE_MAX_NAME_LEN];
};

struct virtio_device_id virtio_can_emu_id[] = {
	{.type = VIRTIO_ID_CAN},
	{ },
};

struct virtio_emulator virtio_can = {
	.name = "virtio_can",
	.id_table = virtio_can_emu_id,
};

static int __init virtio_can_init(void)
{
	return virtio_register_emulator(&virtio_can);
}

static void __exit virtio_can_exit(void)
{
	virtio_unregister_emulator(&virtio_can);
}

VMM_DECLARE_MODULE(MODULE_DESC,
			MODULE_AUTHOR,
			MODULE_LICENSE,
			MODULE_IPRIORITY,
			MODULE_INIT,
			MODULE_EXIT);
