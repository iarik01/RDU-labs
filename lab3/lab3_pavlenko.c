#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/moduleparam.h>
#include <linux/string.h>      
#include <linux/rcupdate.h>    
#include <linux/version.h>     

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yarpslav Pavlenko");
MODULE_DESCRIPTION("Lab3, task 4: next(next(current)) + surname string");
MODULE_VERSION("0.02");


static int pid = 1;
module_param(pid, int, S_IRUGO);
MODULE_PARM_DESC(pid, "Process ID"); 


struct task_info {
    pid_t         t_pid;          
    unsigned long timestamp;      
    char          student[32];    
};

static struct task_info *ti1;


static pid_t choose_next_next_of_current(void)
{
    pid_t res;

    rcu_read_lock();                             
    {
        struct task_struct *t1 = next_task(current);
        struct task_struct *t2 = next_task(t1);
        res = task_pid_nr(t2);                    
    }
    rcu_read_unlock();

    return res;
}


static struct task_info *task_info_alloc_pavlenko(int in_pid)
{
    struct task_info *ti = kmalloc(sizeof(*ti), GFP_KERNEL);
    if (!ti)
        return NULL;

    ti->t_pid     = in_pid;
    ti->timestamp = jiffies;

    
    strscpy(ti->student, "pavlenko", sizeof(ti->student));

    return ti;
}



static int __init lab3_init_pavlenko(void)
{
    pr_info("lab3_pavlenko: loading module...\n");

    
    pid_t target = choose_next_next_of_current();

    ti1 = task_info_alloc_pavlenko(target);
    if (!ti1) {
        pr_err("lab3_pavlenko: memory allocation failed!\n");
        return -ENOMEM;
    }

    pr_info("lab3_pavlenko: init ok, student=%s, target pid=%d, jiffies(now)=%lu\n",
            ti1->student, ti1->t_pid, jiffies);
    return 0;
}


static void __exit lab3_exit_pavlenko(void)
{
    pr_info("lab3_pavlenko: unloading module...\n");

    if (ti1) {
        pr_info("lab3_pavlenko: student=%s, PID=%d, jiffies(now)=%lu\n",
                ti1->student, ti1->t_pid, jiffies);
        kfree(ti1);
        ti1 = NULL;
    }

    pr_info("lab3_pavlenko: module unloaded.\n");
}

module_init(lab3_init_pavlenko);
module_exit(lab3_exit_pavlenko);
