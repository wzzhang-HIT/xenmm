xen-configuration
===================

**author**: xiehuc<xiehuc@gmail.com>

installnation
-------------

以ubuntu为例子, 首先安装Xen ::

	# apt-get install xen-hypervisory-amd-4.1 libxen-dev

然后使用命令来安装半虚拟化的ubuntu::

   # virt-install -n ubuntu_0 --connect xen:/// -r 1024  -f /dev/svg/ubuntu_0 -w bridge=virbr0 \
     -l http://run.hit.edu.cn/ubuntu/dists/trusty/main/installer-amd64/ \
     --console pty,target_type=virtio

如果出现 ``error: internal error: cannot find character device <null>`` . 则再
次运行 ``sudo xl console ubuntu_0`` 继续安装.

在 **Choose a mirror of the Ubuntu archive** 一定要选择第一项, 手工输入学校的服
务器地址. run.hit.edu.cn

Xen 早的时候使用 xm 的工具链. 现在使用 xl 的工具链. 幸好用法都差不多. 但是推荐
使用 libvirt 的工具链. 能很好的统一两者, 还能兼容KVM等等的虚拟机. 首先需要将
``LIBVIRTD_DEFAULT_URI="xen:///"`` 加入到环境变量中. 当前用户和root用户都要加,
因为有些时候需要跳到sudo权限去做. 或者可以在使用命令的时候加入
--connect=xen:/// 来强制指定连接Xen的服务.

使用 ``virsh list --all`` 来查看所有虚拟机(包括关闭状态).

当安装完一台虚拟机之后, 可以使用克隆命令快速复制::

    #virt-clone -o origin -n target -f file

configuration
-------------

为了使得本地的virt-manager能够连接远程服务器的xen.需要配置一下

1. 修改grub默认启动顺序(可选)::

    将/etc/grub.d/20_linux_xen改为06_linux_xen 保证在10_linux之前就可以了。
    再用`sudo update-grub`重新生成一下。

2. 将当前用户添加到libvirtd组::

    sudo usermod -aG libvirtd xiehuc

3. 修改/etc/xen/xend-config.sxp::

    (xen-http-server yes)
    (xen-port 8000)

4. 重启xend服务(可选)::

    sudo service xend restart

此时就可以使用virt-manager连接远程xen了。

virt-manager
------------

安装时需要指定内存不得小与1024MB.因为不然无法开启图形安装程序。
进而在指定储存分配的时候，使用控制台的很困难。

guest-configure
---------------

xenstore 无论是hvm还是pvm均可以使用。
在客户机中使用 xenstore 命令失败的时候。如果是提示 No Such File Or Diretory.

1. 查看/proc/xen/xenbus 或者 /dev/xen/xenbus
2. 修改/etc/environment 添加 XENSTORED_PATH=<上面的值>
3. 重启电脑

console-configure
------------------

如果是用的全虚拟化方式安装的, 没有console的话可以尝试以下操作. 编辑`/etc/default/grub`文件::

    GRUB_CMDLINE_LINUX_DEFAULT="console=ttyS0,115200"
    GRUB_TERMINAL=serial
    GRUB_SERIAL_COMMAND="serial --speed=115200 --unit=0 --word=8 --pairty=no --stop=1"

使用 ``update-grub`` 命令生成 *grub.cfg*

创建 ``/etc/init/ttyS0.conf`` 内容如下::

    start on stopped rc or RUNLEVEL=[2345]
    stop on runlevel [!2345]
     
    respawn

    exec /sbin/getty -L 115200 ttyS0 vt102

最后.在物理机上使用`xm console $vm_name` 连接虚拟机即可

需要退出的时候,使用 `Ctrl+]` 即可

------------------------------------------------------------------------------------

如果是用的半虚拟化方式安装, 那么大部分情况下都能够顺利的启用console而无需额外的配置.
但是如果是希望使用 ``virsh console`` 工具链来连接console, 则需要使用 ``virsh
edit <domain>``, 修改console项目如下::

   <console type='pty'>
      <target type='virtio' port='0'/>
    </console>

这个是使用新的virtio虚拟输出, 成功率高.


phoronix-test-suite configure
------------------------------

系统源带的pts可能比较老了.推荐在官方网站上下载deb包.用 ``sudo dpkg -i package`` .

对于一些有选项的测试使用 ``TOTAL_LOOP_COUNT`` 环境变量可能得不到预期的结果.可以设置
``.phoronix-test-suite/test-profiles/pts/[Test]/test-definition.xml`` 的
*TestSettings/Default/AllowCacheShare* 设置为 *FALSE*
