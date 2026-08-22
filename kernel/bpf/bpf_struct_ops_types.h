/* SPDX-License-Identifier: GPL-2.0 */
/* internal file - do not include directly */

<<<<<<< HEAD
#ifdef CONFIG_BPF_JIT
#ifdef CONFIG_INET
#include <net/tcp.h>
BPF_STRUCT_OPS_TYPE(tcp_congestion_ops)
#endif
#endif
=======
/* No struct_ops backend is registered on this 5.4 vendor networking stack. */
>>>>>>> 7103de2804e1 (Backport Android 17 BPF support)
