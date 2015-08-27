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
 * @file cmd_can.c
 * @author Benoît Taine (benoit.taine@openwide.fr)
 * @brief Test utility for CAN driver developpement 
 */

#include <vmm_error.h>
#include <vmm_stdio.h>
#include <vmm_version.h>
#include <vmm_modules.h>
#include <vmm_cmdmgr.h>
#include <net/vmm_protocol.h>
#include <libs/netstack.h>
#include <libs/stringlib.h>
#include <libs/mathlib.h>

#define MODULE_DESC			"CAN dev tests"
#define MODULE_AUTHOR			"Benoit Taine"
#define MODULE_LICENSE			"GPL"
#define MODULE_IPRIORITY		0
#define	MODULE_INIT			cmd_can_init
#define	MODULE_EXIT			cmd_can_exit

static void cmd_can_usage(struct vmm_chardev *cdev)
{
	vmm_cprintf(cdev, "Usage: can COMMAND\n");
	vmm_cprintf(cdev, "where  COMMAND := { dump | send }\n");
}

static int cmd_can_exec(struct vmm_chardev *cdev, int argc, char **argv)
{
	vmm_cprintf(cdev, "Called can command\n");

	return VMM_OK;
}

static struct vmm_cmd cmd_can = {
	.name = "can",
	.desc = "Test utility for CAN driver developpement",
	.usage = cmd_can_usage,
	.exec = cmd_can_exec,
};

static int __init cmd_can_init(void)
{
	return vmm_cmdmgr_register_cmd(&cmd_can);
}

static void __exit cmd_can_exit(void)
{
	vmm_cmdmgr_unregister_cmd(&cmd_can);
}

VMM_DECLARE_MODULE(MODULE_DESC, 
			MODULE_AUTHOR, 
			MODULE_LICENSE, 
			MODULE_IPRIORITY, 
			MODULE_INIT, 
			MODULE_EXIT);
