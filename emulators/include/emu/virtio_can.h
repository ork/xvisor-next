/**
 * Copyright (c) 2015 Benoît Taine.
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
 * @file virtio_can.h
 * @author Benoît Taine (benoit.taine@openwide.fr)
 * @brief VirtIO CAN Framework Interface.
 */

#ifndef __VIRTIO_CAN_H_
#define __VIRTIO_CAN_H_

#include <vmm_types.h>

/* The feature bitmap for virtio can */
#define VIRTIO_CAN_F_CTRL_VQ		0	/* Control channel available */
#define VIRTIO_CAN_F_CTRL_LINK_UP	1	/* Link is up */
#define VIRTIO_CAN_F_GUEST_CANFD	10	/* Guest can handle CAN-FD frames */
#define VIRTIO_CAN_F_HOST_CANFD		20	/* Host can handle CAN-FD frames */

/*
 * Control virtqueue data structures
 *
 * The control virtqueue expects a header in the first sg entry
 * and an ack/status response in the last entry.  Data for the
 * command goes in between.
 */
struct virtio_can_ctrl_hdr {
	__u8 class;
	__u8 cmd;
} __attribute__((packed));

typedef __u8 virtio_can_ctrl_ack;

#define VIRTIO_CAN_OK	0
#define VIRTIO_CAN_ERR	1

/*
 * Control CAN chip status
 *
 * Execute standard CAN controller management operations on the host system.
 */
#define VIRTIO_CAN_CTRL_CHIP		0
 #define VIRTIO_CAN_CTRL_CHIP_ENABLE	0
 #define VIRTIO_CAN_CTRL_CHIP_DISABLE	1
 #define VIRTIO_CAN_CTRL_CHIP_FREEZE	2
 #define VIRTIO_CAN_CTRL_CHIP_UNFREEZE	3
 #define VIRTIO_CAN_CTRL_CHIP_SOFTRESET	4

#endif /* __VIRTIO_CAN_H */
