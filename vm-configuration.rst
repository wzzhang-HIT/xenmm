vm-configuration
================

server
------

服务器  : 173.26.100.209 
用户名  : root3 
密码    : 111111 
vnc密码 : vncvnc

目前安装的虚拟机的分配:

+-----------+---------+------+----------------+--------------------------+
| domain    | version | type | static-ip      | disk                     |
+-----------+---------+------+----------------+--------------------------+
| ubuntu_1  | 12.10   | hvm  | 192.168.122.20 | /dev/mapper/vg-ubuntu_1  |
+-----------+---------+------+----------------+--------------------------+
| ubuntu_10 | 12.10   | hvm  | 192.168.122.21 | /dev/mapper/vg-ubuntu_10 |
+-----------+---------+------+----------------+--------------------------+
| ubuntu_2  | 12.10   | hvm  | 192.168.122.22 | /dev/mapper/vg-ubuntu_2  |
+-----------+---------+------+----------------+--------------------------+
| ubuntu_3  | 12.10   | hvm  | 192.168.122.23 | /dev/mapper/vg-ubuntu_3  |
+-----------+---------+------+----------------+--------------------------+
| ubuntu_4  | 12.10   | hvm  | 192.168.122.24 | /dev/mapper/vg-ubuntu_4  |
+-----------+---------+------+----------------+--------------------------+
| ubuntu_5  | 12.10   | hvm  | 192.168.122.25 | /dev/mapper/vg-ubuntu_5  |
+-----------+---------+------+----------------+--------------------------+
| ubuntu_6  | 12.10   | hvm  | 192.168.122.26 | /dev/mapper/vg-ubuntu_6  |
+-----------+---------+------+----------------+--------------------------+
| ubuntu_7  | 12.10   | hvm  | 192.168.122.27 | /dev/mapper/vg-ubuntu_7  |
+-----------+---------+------+----------------+--------------------------+
| ubuntu_8  | 12.10   | hvm  | 192.168.122.28 | /dev/mapper/vg-ubuntu_8  |
+-----------+---------+------+----------------+--------------------------+
| ubuntu_9  | 12.10   | hvm  | 192.168.122.29 | /dev/mapper/vg-ubuntu_9  |
+-----------+---------+------+----------------+--------------------------+

对于所有ubuntu虚拟机

+ **username** : xen
+ **passwd** : xen

对于所有centos/fedora虚拟机

+ **username** : root
+ **passwd** : xenxen

测试pvm安装比较困难。
其中fedora 16/17 安装失败。

lvm-configure
---------------

配置lvm组管理能够方便的创建动态卷。具有高速，易扩展的优势。作为虚拟机的磁盘镜像
非常合适::

   # pvcreate /dev/sdb[No.]
   # vgcreate vg /dev/sdb[No.]
   # lvcreate --name name -L size vg

quick make vm
----------------

虚拟机的克隆使用 ``virt-clone`` 命令, 能快速的扩展虚拟机的数量::

    #virt-clone -o origin -n target -f file
    
disable grub wait time
------------------------

有可能grub配置不正确, 导致启动的时候无限阻塞在grub启动界面上, 等待用户输入. 这
明显是很麻烦的. 

为了取消grub的等待时间, 首先用 virt-manager 连接虚拟机, 进入系统后修改
``/etc/default/grub``. 有 TIMEOUT 关键字的选项这样修改::

   GRUB_HIDDEN_TIMEOUT=0
   #GRUB_HIDDEN_TIMEOUT_QUIET=true
   #GRUB_TIMEOUT=2
   
这样配置能让grub不出现等待图形菜单, 直接进入第一个系统. 修改完后不要忘记输入
``sudo grub-mkconfig -o /boot/grub/grub.cfg`` 来更新grub.

static ip addr
----------------

有一些实验需要静态IP的支持. 修改虚拟机的 ``/etc/network/interfaces`` 中的 eth0
项::
 
    auto eth0
    iface eth0 inet static
        address   192.168.122.21   # ip addr
        broadcast 192.168.122.255
        netmask   255.255.255.0
        gateway   192.268.122.1

运行 `sudo service networking restart` 重启网络服务(对于Ubuntu系统)

另外如果不成功, 也可以直接手动设置ip地址. (会在下次重启时失效). 例如::

   2: eth0: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc pfifo_fast state UP qlen 1000    
      link/ether 00:16:3e:76:db:ed brd ff:ff:ff:ff:ff:ff    
      inet6 fe80::216:3eff:fe76:dbed/64 scope link    
       valid_lft forever preferred_lft forever

这个地址就没有ipv4地址, 输入::

   sudo ip addr add 192.168.122.25/24 brd + dev eth0

来强制指定ipv4地址. 其中 'brd +' 是一个简写.

ssh configure
--------------

有一些实验需要使用ssh来给虚拟机发送指令. 一般虚拟机安装服务器版本的操作系统上会自动打开sshd. 另外加上上一步配置的静态IP, 就能够实现ssh.

1.  可以给ssh加上别名, 方便连接. 修改dom0的 ``~/.ssh/configure`` ::

       Host ubuntu_1
           HostName 192.168.122.20
           User xen

    以后就能够简单的使用 ``ssh ubuntu_1`` 来登录.

2.  在 dom0 上使用 ``$ssh-keygen`` 生成密钥, 为下一部作准备.

3.  在dom0上使用 ``ssh-copy-id user@vm-ip`` 来把自己加入虚拟机的信任列表. 对每
    个虚拟机都操作一下, 以后可以避免ssh登录时候输入密码.

automatic configure
---------------------

我们提供了大量的脚本来辅助搭建实验环境. 在完成了上述步骤之后, 修改
``script/ip.sh`` . 提供正确的ip地址. 如果只需要对部分虚拟机做实验, 只需要在关闭
了虚拟机之后, 注释掉相应的行即可.

以下命令都默认在 build 目录下执行.

1. ``../script/set-mem.sh 1024 2048``: 设置初始化内存和最大内存, 并重启虚拟机.
   有时候无法顺利启动, 需要手工检查.
