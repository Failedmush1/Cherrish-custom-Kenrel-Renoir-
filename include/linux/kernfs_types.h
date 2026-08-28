#ifndef __LINUX_KERNFS_TYPES_H
#define __LINUX_KERNFS_TYPES_H

#include <linux/types.h>

union kernfs_node_id {
	struct {
		u32		ino;
		u32		generation;
	};
	u64			id;
};

#endif /* __LINUX_KERNFS_TYPES_H */
