/*
 * Copyright (C) 2012  Sercomm Corporation
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* Shared library add-on to iptables to add masquerade support. */
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <iptables.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sc/cnapt/ipt_HAIRPIN.h>

/* Function which prints out usage message. */
static void
help(void)
{
	printf("HAIRPIN options: \n");
}

static struct option opts[] = {
	{"eaddr", 1, 0, '1'},
	{ 0 }
};

/* Initialize the target. */
static void
init(struct xt_entry_target *t)
{
}


/* Function which parses command options; returns true if it
   ate an option */
static int parse(int c, char **argv, int invert, unsigned int *flags,
		     const void *entry,
		     struct xt_entry_target **target)
{
	struct ipt_hairpin_info_t *info = (struct ipt_hairpin_info_t *)(*target)->data;

	switch (c) {
	case '1':
/* We shouldn't use "inet_network" here as its return value is "Host byte order".     --- Joe */
//		info->eaddr = inet_network(optarg);
		info->eaddr = inet_addr(optarg);
		if (info->eaddr == -1)
			return 0;
		return 1;
	default:
		return 0;
	}

	return 1;
}

/* Final check; don't care. */
static void final_check(unsigned int flags)
{
}

/* Prints out the targinfo. */
static void
print(const void *ip,
		      const struct xt_entry_target *target, int numeric)
{
	struct ipt_hairpin_info_t *info = (struct ipt_hairpin_info_t*)target->data;
	struct in_addr addr;

	addr.s_addr = info->eaddr;
	printf("eaddr:%s", inet_ntoa(addr));
}

/* Saves the union ipt_targinfo in parsable form to stdout. */
static void
save(const void *ip,
		 const struct xt_entry_target *target)
{
	struct ipt_hairpin_info_t *info = (struct ipt_hairpin_info_t*)target->data;
	struct in_addr addr;

	addr.s_addr = info->eaddr;
	printf("--eaddr %s", inet_ntoa(addr));
}

static struct xtables_target ipt_HAIRPIN = { NULL,
	.name		= "HAIRPIN",
	.version	= XTABLES_VERSION,
	.size		= XT_ALIGN(sizeof(struct ipt_hairpin_info_t)),
	.userspacesize	= XT_ALIGN(sizeof(struct ipt_hairpin_info_t)),
	.help		= &help,
	.init		= &init,
	.parse		= &parse,
	.final_check	= &final_check,
	.print		= &print,
	.save		= &save,
	.extra_opts	= opts
};

void _init(void)
{
	xtables_register_target(&ipt_HAIRPIN);
}
