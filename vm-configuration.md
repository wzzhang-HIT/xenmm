vm-configuration
================

**服务器** : 173.26.100.209
**用户名** : root3
**密码**   : 111111

目前安装的虚拟几的分配
***********************************

domain     version    type       disk
ubuntu_1   12.10      hvm        /dev/dm-0
ubuntu_2   12.10      hvm        /dev/dm-1
ubuntu_3   12.10      hvm        /dev/dm-2
ubuntu_4   12.10      hvm        /dev/dm-3
ubuntu_5   12.10      hvm        /dev/dm-4
ubuntu_6   12.10      hvm        /dev/dm-5
ubuntu_7   12.10      hvm        /dev/dm-6
ubuntu_8   12.10      hvm        /dev/dm-7
centos1    6.3        pvm        /dev/mapper/vg-centos_1(/dev/dm-8)
fedora1    15         pvm        /dev/mapper/vg-fedora_1(/dev/dm-9)

**********************************

对于所有ubuntu虚拟机

**username** : xen
**passwd** : xen

对于所有centos/fedora虚拟机

**username** : root
**passwd** : xenxen

测试pvm安装比较困难。
其中fedora 16/17 安装失败。
