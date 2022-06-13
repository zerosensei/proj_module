/*
 * Copyright (c) 2022 zerosensei
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc.h"


static char *cause_str(ulong_t cause)
{
	switch (cause) {
	case 0:
		return "Instruction address misaligned";
	case 1:
		return "Instruction Access fault";
	case 2:
		return "Illegal instruction";
	case 3:
		return "Breakpoint";
	case 4:
		return "Load address misaligned";
	case 5:
		return "Load access fault";
	case 6:
		return "Store/AMO address misaligned";
	case 7:
		return "Store/AMO access fault";
	case 8:
		return "Environment call from U-mode";
	case 9:
		return "Environment call from S-mode";
	case 11:
		return "Environment call from M-mode";
	case 12:
		return "Instruction page fault";
	case 13:
		return "Load page fault";
	case 15:
		return "Store/AMO page fault";
	default:
		return "unknown";
	}
}

__HIGH_CODE
// __attribute__((weak))
void HardFault_Handler(void)
{
    PRINT("hard fault:\n");

	ulong_t mcause;

	__asm__ volatile("csrr %0, mcause" : "=r" (mcause));

	ulong_t mtval;
	__asm__ volatile("csrr %0, mtval" : "=r" (mtval));

	ulong_t mepc;
	__asm__ volatile("csrr %0, mepc" : "=r" (mepc));

	mcause &= 0x1f;
	PRINT("\n");
	PRINT("mcause: %ld, %s\n", mcause, cause_str(mcause));
	PRINT("mtval: %#lx\n", mtval);
	PRINT("mepc: %#lx\n", mepc);
    WAIT_FOR_DBG;
    
    while(1);
}

