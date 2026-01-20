In this program I implemented IOCTL into my kernel program. This program could read and write to the program and in the kernel, we would see logs of what was going on. If the read or write pointers overflow we reset them using IOCTL. We also
have two C files. One for the user space and one for the kernel module
