### Support cryptodev on guest virtual machine through virtio
---
#### Guest Machine
compile (`make`) and install (`./crypto\_dev\_nodes.sh` & `insmod virtio\_cryptodev.ko`)

#### Host Machine
do
	patch -p1 < /path/to/qemu_patch

and replace `virtio-cryptodev.c` in `hw/char/` in qemu source code with the one here.

---
#### **_NOTES_**
I take credit only for the implementation of the open-close-ioctl syscalls in the virtio-cryptodev.c of the host
and the crypto-chrdev.c of the guest
