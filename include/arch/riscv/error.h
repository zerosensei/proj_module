/*
 * Copyright (c) 2020 BayLibre, SAS
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief RISCV public error handling
 *
 * RISCV-specific kernel error handling interface. Included by riscv/arch.h.
 */

#ifndef ZEPHYR_INCLUDE_ARCH_RISCV_ERROR_H_
#define ZEPHYR_INCLUDE_ARCH_RISCV_ERROR_H_

#include <arch/riscv/syscall.h>
#include <arch/riscv/exp.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_USERSPACE

/*
 * Kernel features like canary (software stack guard) are built
 * with an argument to bypass the test before syscall (test if CPU
 * is running in user or kernel) and directly execute the function.
 * Then if this kind of code wishes to trigger a CPU exception,
 * the implemented syscall is useless because  the function is directly
 * called even if the CPU is running in user (which happens during
 * sanity check). To fix that, I bypass the generated test code by writing
 * the test myself to remove the bypass ability.
 */

#define ARCH_EXCEPT(reason_p)	do {			\
		if (k_is_user_context()) {		\
			arch_syscall_invoke1(reason_p,	\
				K_SYSCALL_USER_FAULT);	\
		} else {				\
			compiler_barrier();		\
			z_impl_user_fault(reason_p);	\
		}					\
		CODE_UNREACHABLE; /* LCOV_EXCL_LINE */	\
	} while (false)
#else
/*
 * Raise an illegal instruction exception so that mepc will hold expected value in
 * exception handler, and generated coredump can reconstruct the failing stack.
 * Store reason_p in register t6, marker in t5
 */
#define ARCH_EXCEPT_MARKER 0x00DEAD00
#define ARCH_EXCEPT(reason_p)	do {			\
		__asm__ volatile("addi t5, %[marker], 0"	\
			: : [marker] "r" (ARCH_EXCEPT_MARKER));	\
		__asm__ volatile("addi t6, %[reason], 0"	\
			: : [reason] "r" (reason_p));	\
		__asm__ volatile("unimp");		\
	} while (false)
#endif

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_ARCH_RISCV_ERROR_H_ */
