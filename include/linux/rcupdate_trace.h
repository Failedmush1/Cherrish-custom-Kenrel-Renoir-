/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Read-Copy Update mechanism for mutual exclusion, adapted for tracing.
 *
 * Copyright (C) 2020 Paul E. McKenney.
 */

#ifndef __LINUX_RCUPDATE_TRACE_H
#define __LINUX_RCUPDATE_TRACE_H

#include <linux/sched.h>
#include <linux/rcupdate.h>

#ifdef CONFIG_TASKS_TRACE_RCU

extern struct lockdep_map rcu_trace_lock_map;

#ifdef CONFIG_DEBUG_LOCK_ALLOC
static inline int rcu_read_lock_trace_held(void)
{
	return lock_is_held(&rcu_trace_lock_map);
}
#else
static inline int rcu_read_lock_trace_held(void)
{
	return 1;
}
#endif

void rcu_read_unlock_trace_special(struct task_struct *t);

static inline void rcu_read_lock_trace(void)
{
	struct task_struct *t = current;

	WRITE_ONCE(t->trc_reader_nesting, READ_ONCE(t->trc_reader_nesting) + 1);
	barrier();
	if (IS_ENABLED(CONFIG_TASKS_TRACE_RCU_READ_MB) &&
	    t->trc_reader_special.b.need_mb)
		smp_mb();
	rcu_lock_acquire(&rcu_trace_lock_map);
}

static inline void rcu_read_unlock_trace(void)
{
	int nesting;
	struct task_struct *t = current;

	rcu_lock_release(&rcu_trace_lock_map);
	nesting = READ_ONCE(t->trc_reader_nesting) - 1;
	barrier();
	WRITE_ONCE(t->trc_reader_nesting, INT_MIN + nesting);
	if (likely(!READ_ONCE(t->trc_reader_special.s)) || nesting) {
		WRITE_ONCE(t->trc_reader_nesting, nesting);
		return;
	}
	WARN_ON_ONCE(nesting != 0);
	rcu_read_unlock_trace_special(t);
}

void call_rcu_tasks_trace(struct rcu_head *rhp, rcu_callback_t func);
void synchronize_rcu_tasks_trace(void);
void rcu_barrier_tasks_trace(void);

#else /* !CONFIG_TASKS_TRACE_RCU */

static inline int rcu_read_lock_trace_held(void)
{
	return 1;
}

static inline void rcu_read_lock_trace(void) { }
static inline void rcu_read_unlock_trace(void) { }
static inline void call_rcu_tasks_trace(struct rcu_head *rhp, rcu_callback_t func)
{
	call_rcu(rhp, func);
}
static inline void synchronize_rcu_tasks_trace(void)
{
	synchronize_rcu();
}
static inline void rcu_barrier_tasks_trace(void)
{
	rcu_barrier();
}

#endif /* CONFIG_TASKS_TRACE_RCU */

#endif /* __LINUX_RCUPDATE_TRACE_H */
