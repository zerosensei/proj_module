/*
 * Copyright (c) 2022 zerosensei
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <sys_clock.h>
#include <spinlock.h>
#include <drivers/timer/system_timer.h>
#include <soc.h>

#define CYC_PER_TICK (uint32_t)((uint64_t) (sys_clock_hw_cycles_per_sec()			 \
				 / (uint64_t)CONFIG_SYS_CLOCK_TICKS_PER_SEC))
#define MAX_CYC __INT_MAX__
#define MAX_TICKS ((MAX_CYC - CYC_PER_TICK) / CYC_PER_TICK)
#define MIN_DELAY 1000

#define TICKLESS IS_ENABLED(CONFIG_TICKLESS_KERNEL)

static struct k_spinlock lock;
static uint64_t last_count = 0;

static void set_systimer_alarm(uint64_t time)
{
    if((time - 1) > SysTick_LOAD_RELOAD_Msk)
        return ; /* Reload value impossible */

    SysTick->CMP = time - 1; /* set reload register */
}

static uint64_t systimer_alarm(void)
{
    return SysTick->CNT;
}

__HIGH_CODE
void SysTick_Handler(void)
{
	k_spinlock_key_t key = k_spin_lock(&lock);
	uint64_t now = systimer_alarm();
	uint32_t dticks = (uint32_t)((now - last_count) / CYC_PER_TICK);

	last_count += dticks * CYC_PER_TICK;

	if (!TICKLESS) {
		uint64_t next = last_count + CYC_PER_TICK;

		if ((uint64_t)(next - now) < MIN_DELAY) {
			next += CYC_PER_TICK;
		}
		set_systimer_alarm(next);
	}

	k_spin_unlock(&lock, key);
	sys_clock_announce(IS_ENABLED(CONFIG_TICKLESS_KERNEL) ? dticks : 1);

	SysTick->SR &= ~(1 << 0);
}

void sys_clock_set_timeout(int32_t ticks, bool idle)
{
    ARG_UNUSED(idle);
#if defined(CONFIG_TICKLESS_KERNEL)
	ticks = ticks == K_TICKS_FOREVER ? MAX_TICKS : ticks;
	ticks = CLAMP(ticks - 1, 0, (int32_t)MAX_TICKS);

	k_spinlock_key_t key = k_spin_lock(&lock);
	uint64_t now = systimer_alarm();
	uint32_t adj, cyc = ticks * CYC_PER_TICK;

	/* Round up to next tick boundary. */
	adj = (uint32_t)(now - last_count) + (CYC_PER_TICK - 1);
	if (cyc <= MAX_CYC - adj) {
		cyc += adj;
	} else {
		cyc = MAX_CYC;
	}
	cyc = (cyc / CYC_PER_TICK) * CYC_PER_TICK;

	if ((int32_t)(cyc + last_count - now) < MIN_DELAY) {
		cyc += CYC_PER_TICK;
	}

	set_systimer_alarm(cyc + last_count);
	k_spin_unlock(&lock, key);
#endif
}

uint32_t sys_clock_elapsed(void)
{
	if (!IS_ENABLED(CONFIG_TICKLESS_KERNEL)) {
		return 0;
	}

	k_spinlock_key_t key = k_spin_lock(&lock);
	uint32_t ret = ((uint32_t)systimer_alarm() - (uint32_t)last_count) / CYC_PER_TICK;

	k_spin_unlock(&lock, key);
	return ret;
}

uint32_t sys_clock_cycle_get_32(void)
{
	return (uint32_t)systimer_alarm();
}

uint64_t sys_clock_cycle_get_64(void)
{
	return systimer_alarm();
}

int sys_clock_driver_init(void)
{
    SysTick->CTLR = SysTick_CTLR_INIT |
                    SysTick_CTLR_STRE |
                    SysTick_CTLR_STCLK |
                    SysTick_CTLR_STIE |
                    SysTick_CTLR_STE; 

    PFIC_EnableIRQ(SysTick_IRQn);

	return 0;
}