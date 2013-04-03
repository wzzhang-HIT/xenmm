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

console-configure
------------------

编辑`/etc/default/grub`文件:

    GRUB_CMDLINE_LINUX_DEFAULT="console=ttyS0,115200"
    GRUB_TERMINAL=serial
    GRUB_SERIAL_COMMAND="serial --speed=115200 --unit=0 --word=8 --pairty=no --stop=1"

使用`update-grub`命令生成`grub.cfg`

创建`/etc/init/ttyS0.conf`内容如下:

    start on stopped rc or RUNLEVEL=[2345]
    stop on runlevel [!2345]
     
    respawn

    exec /sbin/getty -L 115200 ttyS0 vt102

最后.在物理机上使用`xm console $vm_name` 连接虚拟机即可

需要退出的时候,使用`Ctrl+[`即可


phoronix-test-suite configure
------------------------------

系统源带的pts可能比较老了.推荐在官方网站上下载deb包.用sudo dpkg -i package.

对于一些有选项的测试使用TOTAL_LOOP_COUNT环境变量可能得不到预期的结果.可以设置
`.phoronix-test-suite/test-profiles/pts/[Test]/test-definition.xml`
TestSettings/Default/AllowCacheShare设置为FALSE
