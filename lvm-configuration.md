lvm-configure
=============

配置lvm组管理能够方便的创建动态卷。具有高速，易扩展的优势。

   # pvcreate /dev/sdb[NO]
   # vgcreate vg /dev/sdb[NO]
   # lvcreate --name name -L size vg

