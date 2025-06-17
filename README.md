# KeyCounter Linux Kernel Module

**KeyCounter** is a Linux kernel module that tracks keyboard key presses in real time using the keyboard notifier interface. It counts how many times each key is pressed and exposes the results via the `/proc/KeyCounter` file.

---

## 🛠️ System Requirements & Dependencies

Make sure you are working in a **Linux-based virtual machine or environment** and that your system packages are up to date.

### Required Packages

#### On Fedora:
```bash
sudo dnf install kernel-devel kernel-headers
sudo dnf groupinstall c-development
```

#### On Ubuntu/Debian:
```bash
sudo apt install build-essential linux-headers-$(uname -r)
```

Also, ensure that `gcc` and `make` are installed.

---

## ⚙️ Compilation & Installation

1. **Create a working directory and navigate into it:**
   ```bash
   mkdir ~/PROJECT && cd ~/PROJECT
   ```

2. **Add the following files:**
   - `KeyCounter.c` (the kernel module source code)
   - `Makefile` (build configuration)

3. **To compile the module:**
   ```bash
   make
   ```

4. **To insert the module into the kernel:**
   ```bash
   sudo insmod KeyCounter.ko
   ```
   
   if you want to remove the module;
   
6. **To remove the module:**
   ```bash
   sudo rmmod KeyCounter.ko
   ```

7. **To clean up the build files:**
   ```bash
   make clean
   ```

---

## 📊 Usage Instructions

After loading the module, you can view the key press counts by reading the `/proc/KeyCounter` file:

```bash
cat /proc/KeyCounter
```

### Example Output:

![KeyCounter Output](https://github.com/user-attachments/assets/47f51ca8-fa46-451c-ae76-ebf9f006e15d)

Each line shows the key code and the number of times it has been pressed.

### Resources:

https://sysprog21.github.io/lkmpg/
