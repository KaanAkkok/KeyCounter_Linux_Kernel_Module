I. Introduction

This project implements a Linux kernel module called KeyCounter that tracks keyboard key presses in real time. It uses the keyboard notifier interface to detect key events and counts how many times each key is pressed. The results are shown through the /proc/KeyCounter file. The goal is to explore low-level input handling and kernel module programming in C.

II. System Requirements and Dependencies

Before you begin, verify that you are working in a Linux-based virtual machine environment and that your system packages are up to date.
Required Dependencies:
•	Linux kernel development headers:
   For Fedora: 
sudo dnf install kernel-devel kernel-headers
sudo dnf groupinstall c-development
   For Ubuntu:
sudo apt install build-essential linux-headers-$(uname -r)
•	GCC and Make tools

III. Compilation and Installation

First, create a new directory in your home folder and add the KeyCounter.c source file along with a Makefile. Then, compile the module by running the make command. To insert the module into the kernel, use:
sudo insmod KeyCounter.ko
To remove the module, run:
sudo rmmod KeyCounter.ko
Finally, to clean up the build files, use:
make clean

IV. Usage Instructions

After loading the module, use this command to view key press counts:
cat /proc/KeyCounter
Example output:
![resim](https://github.com/user-attachments/assets/47f51ca8-fa46-451c-ae76-ebf9f006e15d)
