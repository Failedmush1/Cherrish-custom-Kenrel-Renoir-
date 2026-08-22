#ifndef __LINUX_USERMODE_DRIVER_H__
#define __LINUX_USERMODE_DRIVER_H__

#include <linux/umh.h>
#include <linux/path.h>

<<<<<<< HEAD
#ifdef CONFIG_BPFILTER
void __exit_umh(struct task_struct *tsk);

static inline void exit_umh(struct task_struct *tsk)
{
	if (unlikely(tsk->flags & PF_UMH))
		__exit_umh(tsk);
}
#else
static inline void exit_umh(struct task_struct *tsk)
{
}
#endif

=======
>>>>>>> 7103de2804e1 (Backport Android 17 BPF support)
struct umd_info {
	const char *driver_name;
	struct file *pipe_to_umh;
	struct file *pipe_from_umh;
	struct list_head list;
	void (*cleanup)(struct umd_info *info);
	struct path wd;
	struct pid *tgid;
};
<<<<<<< HEAD
=======

>>>>>>> 7103de2804e1 (Backport Android 17 BPF support)
int umd_load_blob(struct umd_info *info, const void *data, size_t len);
int umd_unload_blob(struct umd_info *info);
int fork_usermode_driver(struct umd_info *info);
void umd_cleanup_helper(struct umd_info *info);

#endif /* __LINUX_USERMODE_DRIVER_H__ */
