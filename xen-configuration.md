xen-configuration
===================

**author**:xiehuc<xiehuc@gmail.com>

installnation
-------------

以ubuntu为例子：

	$ sudo apt-get install xen-hypervisory-amd-4.1 virt-manager

等包

configuration
-------------

为了使得本地的virt-manager能够连接远程服务器的xen.需要配置一下

1. 修改grub默认启动顺序

	将/etc/grub.d/20_linux_xen改为06_linux_xen 保证在10_linux之前就可以了。
	再用`sudo update-grub`重新生成一下。

2. 将当前用户添加到libvirtd组。
	`sudo usermod -aG libvirtd root3`

3. 修改/etc/xen/xend-config.sxp
	
	(xen-http-server yes)
	(xen-port 8000)

4. 重启xend服务
	`sudo service xend restart`

此时就可以使用virt-manager连接远程xen了。

virt-manager
------------

安装时需要指定内存不得小与1024MB.因为不然无法开启图形安装程序。
进而在指定储存分配的时候，使用控制台的很困难。

guest-configure
---------------

xenstore-*无论是hvm还是pvm均可以使用。
在客户机中使用xenstore-*失败的时候。如果是提示No Such File Or Diretory.
1. 查看/proc/xen/xenbus 或者 /dev/xen/xenbus
2. 修改/etc/environment 添加 XENSTORED_PATH=<上面的值>
3. 重启电脑
