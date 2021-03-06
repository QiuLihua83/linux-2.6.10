/*
 * linux/arch/arm/mach-iop3xx/iop321-setup.c
 *
 * Author: Nicolas Pitre <nico@cam.org>
 * Copyright (C) 2001 MontaVista Software, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/config.h>
#include <linux/init.h>
#include <linux/major.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/serial.h>
#include <linux/tty.h>
#include <linux/serial_core.h>

#include <asm/io.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <asm/mach/map.h>
#include <asm/setup.h>
#include <asm/system.h>
#include <asm/memory.h>
#include <asm/hardware.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>

#define IOP321_UART_XTAL 1843200

/*
 * Standard IO mapping for all IOP321 based systems
 */
static struct map_desc iop321_std_desc[] __initdata = {
 /* virtual     physical      length      type */

 /* mem mapped registers */
 { IOP321_VIRT_MEM_BASE,  IOP321_PHY_MEM_BASE,   0x00002000,  MT_DEVICE },

 /* PCI IO space */
 { 0xfe000000,  0x90000000,   0x00020000,  MT_DEVICE }
};

#ifdef CONFIG_ARCH_IQ80321
#define UARTBASE IQ80321_UART
#define IRQ_UART IRQ_IQ80321_UART
#endif

#ifdef CONFIG_ARCH_IQ31244
#define UARTBASE IQ31244_UART
#define IRQ_UART IRQ_IQ31244_UART
#endif

static struct uart_port iop321_serial_ports[] = {
	{
		.membase	= (char*)(UARTBASE),
		.mapbase	= (UARTBASE),
		.irq		= IRQ_UART,
		.flags		= UPF_SKIP_TEST,
		.iotype		= UPIO_MEM,
		.regshift	= 0,
		.uartclk	= IOP321_UART_XTAL,
		.line		= 0,
		.type		= PORT_16550A,
		.fifosize	= 16
	}
};

void __init iop321_map_io(void)
{
	iotable_init(iop321_std_desc, ARRAY_SIZE(iop321_std_desc));
	early_serial_setup(&iop321_serial_ports[0]);
}

#ifdef CONFIG_ARCH_IQ80321
extern void iq80321_map_io(void);
extern struct sys_timer iop321_timer;
extern void iop321_init_time(void);
#endif

#ifdef CONFIG_ARCH_IQ31244
extern void iq31244_map_io(void);
extern struct sys_timer iop321_timer;
extern void iop321_init_time(void);
#endif

#if defined(CONFIG_ARCH_IQ80321)
MACHINE_START(IQ80321, "Intel IQ80321")
	MAINTAINER("Intel Corporation")
	BOOT_MEM(PHYS_OFFSET, IQ80321_UART, IQ80321_UART)
	MAPIO(iq80321_map_io)
	INITIRQ(iop321_init_irq)
	.timer		= &iop321_timer,
    BOOT_PARAMS(0xa0000100)
MACHINE_END
#elif defined(CONFIG_ARCH_IQ31244)
MACHINE_START(IQ31244, "Intel IQ31244")
    MAINTAINER("Intel Corp.")
    BOOT_MEM(PHYS_OFFSET, IQ31244_UART, IQ31244_UART)
    MAPIO(iq31244_map_io)
    INITIRQ(iop321_init_irq)
	.timer		= &iop321_timer,
    BOOT_PARAMS(0xa0000100)
MACHINE_END
#else
#error No machine descriptor defined for this IOP3XX implementation
#endif
