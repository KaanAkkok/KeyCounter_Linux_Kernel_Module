#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/keyboard.h>
#include <linux/input.h>

#define PROC_NAME "KeyCounter"
#define MAX_KEYS 256

static unsigned long key_counts[MAX_KEYS];
static struct proc_dir_entry *proc_entry;

static char get_key_char(int keycode)
{
    switch (keycode) {
        case KEY_A ... KEY_Z: 
            return 'A' + (keycode - KEY_A);
        case KEY_1 ... KEY_9: 
            return '1' + (keycode - KEY_1);
        case KEY_0: 
            return '0';
        case KEY_SPACE: 
            return ' ';
        case KEY_ENTER: 
            return '\n';
        case KEY_TAB: 
            return '\t';
        default: 
            return '\0';  // Desteklenmeyen tuþlar için
    }
}

// Klavye olaylarýný yakalayan fonksiyon
static int keyboard_callback(struct notifier_block *nb, unsigned long action, void *data)
{
    struct keyboard_notifier_param *param = data;
    
    if (action == KBD_KEYCODE) {
        if (param->down) {  
            if (param->value < MAX_KEYS) {
                key_counts[param->value]++;
            }
        }
    }
    return NOTIFY_OK;
}

// Keyboard notifier
static struct notifier_block keyboard_nb = {
    .notifier_call = keyboard_callback,
};

// /proc dosyasýnýn içine key counterlarý yazan fonksiyon
static int keycount_show(struct seq_file *m, void *v)
{
    seq_printf(m, "=== Key Counter ===\n");
    
    for (int i = 0; i < MAX_KEYS; i++) {
        if (key_counts[i] > 0) {
            char key_char = get_key_char(i);
            if (key_char != '\0') {
                if (key_char == ' ') {
                    seq_printf(m, "SPACE: %lu\n", key_counts[i]);
                } else if (key_char == '\n') {
                    seq_printf(m, "ENTER: %lu\n", key_counts[i]);
                } else if (key_char == '\t') {
                    seq_printf(m, "TAB: %lu\n", key_counts[i]);
                } else {
                    seq_printf(m, "%c: %lu\n", key_char, key_counts[i]);
                }
            }
        }
    }
    
    seq_printf(m, "\nTotal key pressed: ");
    unsigned long total = 0;
    for (int i = 0; i < MAX_KEYS; i++) {
        if (get_key_char(i) != '\0') {
            total += key_counts[i];
        }
    }
    seq_printf(m, "%lu\n", total);
    
    return 0;
}

// /proc dosyasýný açan fonksiyon
static int keycount_open(struct inode *inode, struct file *file)
{
    return single_open(file, keycount_show, NULL);
}

// /proc dosya operasyonlarý
static const struct proc_ops keycount_proc_ops = {
    .proc_open = keycount_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

// Modül yükleme fonksiyonu
static int __init KeyCounter_init(void)
{
    int ret;
    
    // Tuþ sayaçlarýný sýfýrla
    memset(key_counts, 0, sizeof(key_counts));
    
    // /proc dosyasý oluþtur
    proc_entry = proc_create(PROC_NAME, 0644, NULL, &keycount_proc_ops);
    if (!proc_entry) {
        printk(KERN_ERR "KeyCounter /proc file created\n");
        return -ENOMEM;
    }
    
    // Keyboard notifierý kaydet
    ret = register_keyboard_notifier(&keyboard_nb);
    if (ret) {
        proc_remove(proc_entry);
        printk(KERN_ERR "KeyCounter /proc file not created\n");
        return ret;
    }
    
    printk(KERN_INFO "KeyCounter Module loaded.\n");
    printk(KERN_INFO "KeyCounter You can see the result with: /proc/%s \n", PROC_NAME);
    
    return 0;
}

// Modül kaldýrma fonksiyonu
static void __exit KeyCounter_exit(void)
{
    unregister_keyboard_notifier(&keyboard_nb);
    proc_remove(proc_entry);
    printk(KERN_INFO "KeyCounter Module unloaded.\n");
}

// Modül giriþ ve çýkýþ noktalarýný tanýmla
module_init(KeyCounter_init);
module_exit(KeyCounter_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kaan Akkok")
