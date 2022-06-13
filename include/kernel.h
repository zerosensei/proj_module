/*
 * Copyright (c) 2016, Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 *
 * @brief Public kernel APIs.
 */

#ifndef ZEPHYR_INCLUDE_KERNEL_H_
#define ZEPHYR_INCLUDE_KERNEL_H_

#if !defined(_ASMLANGUAGE)
#include <kernel_includes.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <toolchain.h>

#ifdef __cplusplus
extern "C" {
#endif



/**
 * @addtogroup clock_apis
 * @{
 */

/**
 * @brief Generate null timeout delay.
 *
 * This macro generates a timeout delay that instructs a kernel API
 * not to wait if the requested operation cannot be performed immediately.
 *
 * @return Timeout delay value.
 */
#define K_NO_WAIT Z_TIMEOUT_NO_WAIT

/**
 * @brief Generate timeout delay from nanoseconds.
 *
 * This macro generates a timeout delay that instructs a kernel API to
 * wait up to @a t nanoseconds to perform the requested operation.
 * Note that timer precision is limited to the tick rate, not the
 * requested value.
 *
 * @param t Duration in nanoseconds.
 *
 * @return Timeout delay value.
 */
#define K_NSEC(t)     Z_TIMEOUT_NS(t)

/**
 * @brief Generate timeout delay from microseconds.
 *
 * This macro generates a timeout delay that instructs a kernel API
 * to wait up to @a t microseconds to perform the requested operation.
 * Note that timer precision is limited to the tick rate, not the
 * requested value.
 *
 * @param t Duration in microseconds.
 *
 * @return Timeout delay value.
 */
#define K_USEC(t)     Z_TIMEOUT_US(t)

/**
 * @brief Generate timeout delay from cycles.
 *
 * This macro generates a timeout delay that instructs a kernel API
 * to wait up to @a t cycles to perform the requested operation.
 *
 * @param t Duration in cycles.
 *
 * @return Timeout delay value.
 */
#define K_CYC(t)     Z_TIMEOUT_CYC(t)

/**
 * @brief Generate timeout delay from system ticks.
 *
 * This macro generates a timeout delay that instructs a kernel API
 * to wait up to @a t ticks to perform the requested operation.
 *
 * @param t Duration in system ticks.
 *
 * @return Timeout delay value.
 */
#define K_TICKS(t)     Z_TIMEOUT_TICKS(t)

/**
 * @brief Generate timeout delay from milliseconds.
 *
 * This macro generates a timeout delay that instructs a kernel API
 * to wait up to @a ms milliseconds to perform the requested operation.
 *
 * @param ms Duration in milliseconds.
 *
 * @return Timeout delay value.
 */
#define K_MSEC(ms)     Z_TIMEOUT_MS(ms)

/**
 * @brief Generate timeout delay from seconds.
 *
 * This macro generates a timeout delay that instructs a kernel API
 * to wait up to @a s seconds to perform the requested operation.
 *
 * @param s Duration in seconds.
 *
 * @return Timeout delay value.
 */
#define K_SECONDS(s)   K_MSEC((s) * MSEC_PER_SEC)

/**
 * @brief Generate timeout delay from minutes.

 * This macro generates a timeout delay that instructs a kernel API
 * to wait up to @a m minutes to perform the requested operation.
 *
 * @param m Duration in minutes.
 *
 * @return Timeout delay value.
 */
#define K_MINUTES(m)   K_SECONDS((m) * 60)

/**
 * @brief Generate timeout delay from hours.
 *
 * This macro generates a timeout delay that instructs a kernel API
 * to wait up to @a h hours to perform the requested operation.
 *
 * @param h Duration in hours.
 *
 * @return Timeout delay value.
 */
#define K_HOURS(h)     K_MINUTES((h) * 60)

/**
 * @brief Generate infinite timeout delay.
 *
 * This macro generates a timeout delay that instructs a kernel API
 * to wait as long as necessary to perform the requested operation.
 *
 * @return Timeout delay value.
 */
#define K_FOREVER Z_FOREVER

#ifdef CONFIG_TIMEOUT_64BIT

/**
 * @brief Generates an absolute/uptime timeout value from system ticks
 *
 * This macro generates a timeout delay that represents an expiration
 * at the absolute uptime value specified, in system ticks.  That is, the
 * timeout will expire immediately after the system uptime reaches the
 * specified tick count.
 *
 * @param t Tick uptime value
 * @return Timeout delay value
 */
#define K_TIMEOUT_ABS_TICKS(t) \
	Z_TIMEOUT_TICKS(Z_TICK_ABS((k_ticks_t)MAX(t, 0)))

/**
 * @brief Generates an absolute/uptime timeout value from milliseconds
 *
 * This macro generates a timeout delay that represents an expiration
 * at the absolute uptime value specified, in milliseconds.  That is,
 * the timeout will expire immediately after the system uptime reaches
 * the specified tick count.
 *
 * @param t Millisecond uptime value
 * @return Timeout delay value
 */
#define K_TIMEOUT_ABS_MS(t) K_TIMEOUT_ABS_TICKS(k_ms_to_ticks_ceil64(t))

/**
 * @brief Generates an absolute/uptime timeout value from microseconds
 *
 * This macro generates a timeout delay that represents an expiration
 * at the absolute uptime value specified, in microseconds.  That is,
 * the timeout will expire immediately after the system uptime reaches
 * the specified time.  Note that timer precision is limited by the
 * system tick rate and not the requested timeout value.
 *
 * @param t Microsecond uptime value
 * @return Timeout delay value
 */
#define K_TIMEOUT_ABS_US(t) K_TIMEOUT_ABS_TICKS(k_us_to_ticks_ceil64(t))

/**
 * @brief Generates an absolute/uptime timeout value from nanoseconds
 *
 * This macro generates a timeout delay that represents an expiration
 * at the absolute uptime value specified, in nanoseconds.  That is,
 * the timeout will expire immediately after the system uptime reaches
 * the specified time.  Note that timer precision is limited by the
 * system tick rate and not the requested timeout value.
 *
 * @param t Nanosecond uptime value
 * @return Timeout delay value
 */
#define K_TIMEOUT_ABS_NS(t) K_TIMEOUT_ABS_TICKS(k_ns_to_ticks_ceil64(t))

/**
 * @brief Generates an absolute/uptime timeout value from system cycles
 *
 * This macro generates a timeout delay that represents an expiration
 * at the absolute uptime value specified, in cycles.  That is, the
 * timeout will expire immediately after the system uptime reaches the
 * specified time.  Note that timer precision is limited by the system
 * tick rate and not the requested timeout value.
 *
 * @param t Cycle uptime value
 * @return Timeout delay value
 */
#define K_TIMEOUT_ABS_CYC(t) K_TIMEOUT_ABS_TICKS(k_cyc_to_ticks_ceil64(t))

#endif

/** @} */

/**
 * @addtogroup clock_apis
 * @ingroup kernel_apis
 * @{
 */

/**
 * @brief Get system uptime, in system ticks.
 *
 * This routine returns the elapsed time since the system booted, in
 * ticks (c.f. @kconfig{CONFIG_SYS_CLOCK_TICKS_PER_SEC}), which is the
 * fundamental unit of resolution of kernel timekeeping.
 *
 * @return Current uptime in ticks.
 */
int64_t k_uptime_ticks(void);

/**
 * @brief Get system uptime.
 *
 * This routine returns the elapsed time since the system booted,
 * in milliseconds.
 *
 * @note
 *    While this function returns time in milliseconds, it does
 *    not mean it has millisecond resolution. The actual resolution depends on
 *    @kconfig{CONFIG_SYS_CLOCK_TICKS_PER_SEC} config option.
 *
 * @return Current uptime in milliseconds.
 */
static inline int64_t k_uptime_get(void)
{
	return k_ticks_to_ms_floor64(k_uptime_ticks());
}

/**
 * @brief Get system uptime (32-bit version).
 *
 * This routine returns the lower 32 bits of the system uptime in
 * milliseconds.
 *
 * Because correct conversion requires full precision of the system
 * clock there is no benefit to using this over k_uptime_get() unless
 * you know the application will never run long enough for the system
 * clock to approach 2^32 ticks.  Calls to this function may involve
 * interrupt blocking and 64-bit math.
 *
 * @note
 *    While this function returns time in milliseconds, it does
 *    not mean it has millisecond resolution. The actual resolution depends on
 *    @kconfig{CONFIG_SYS_CLOCK_TICKS_PER_SEC} config option
 *
 * @return The low 32 bits of the current uptime, in milliseconds.
 */
static inline uint32_t k_uptime_get_32(void)
{
	return (uint32_t)k_uptime_get();
}

/**
 * @brief Get elapsed time.
 *
 * This routine computes the elapsed time between the current system uptime
 * and an earlier reference time, in milliseconds.
 *
 * @param reftime Pointer to a reference time, which is updated to the current
 *                uptime upon return.
 *
 * @return Elapsed time.
 */
static inline int64_t k_uptime_delta(int64_t *reftime)
{
	int64_t uptime, delta;

	uptime = k_uptime_get();
	delta = uptime - *reftime;
	*reftime = uptime;

	return delta;
}

/**
 * @brief Read the hardware clock.
 *
 * This routine returns the current time, as measured by the system's hardware
 * clock.
 *
 * @return Current hardware clock up-counter (in cycles).
 */
static inline uint32_t k_cycle_get_32(void)
{
	return arch_k_cycle_get_32();
}

/**
 * @brief Read the 64-bit hardware clock.
 *
 * This routine returns the current time in 64-bits, as measured by the
 * system's hardware clock, if available.
 *
 * @see CONFIG_TIMER_HAS_64BIT_CYCLE_COUNTER
 *
 * @return Current hardware clock up-counter (in cycles).
 */
static inline uint64_t k_cycle_get_64(void)
{
	if (!IS_ENABLED(CONFIG_TIMER_HAS_64BIT_CYCLE_COUNTER)) {
		__ASSERT(0, "64-bit cycle counter not enabled on this platform. "
			    "See CONFIG_TIMER_HAS_64BIT_CYCLE_COUNTER");
		return 0;
	}

	return arch_k_cycle_get_64();
}


#ifdef __cplusplus
}
#endif

#endif /* !_ASMLANGUAGE */

#endif /* ZEPHYR_INCLUDE_KERNEL_H_ */
