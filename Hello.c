// SPDX-License-Identifier: GPL-2.0 OR BSD-3-Clause
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Markelov Ivan");
MODULE_DESCRIPTION("Lab 4");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int num_of_greet = 1;
module_param(num_of_greet, uint, 0444);
MODULE_PARM_DESC(num_of_greet, "Number of times 'Hello, World!' string will be printed");

struct event_list {
struct list_head list;
ktime_t event_time;
};

static LIST_HEAD(event_list_head);

static void new_event(void);

static int __init hello_init(void)
{
int i;

if (num_of_greet == 0 || (num_of_greet >= 5 && num_of_greet <= 10))
	pr_warn("Number is 0 or in range [5, 10]\n");

if (num_of_greet > 10) {
	pr_err("Number is too big\n");
	return -EINVAL;
}

for (i = 0; i < num_of_greet; i++) {
	pr_emerg("Hello, World!\n");
	new_event();
}

return 0;
}

static void __exit hello_exit(void)
{
struct event_list *md, *tmp;

list_for_each_entry_safe(md, tmp, &event_list_head, list) {
	pr_emerg("Time: %lld\n", md->event_time);
	list_del(&md->list);
	kfree(md);
}
}

static void new_event(void)
{
struct event_list *element = kmalloc(sizeof(struct event_list), GFP_KERNEL);

if (!element)
	return;

element->event_time = ktime_get();
list_add_tail(&element->list, &event_list_head);
}

module_init(hello_init);
module_exit(hello_exit);
