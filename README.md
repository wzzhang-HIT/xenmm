xenmm
======

xen memory management project
author: xiehuc<xiehuc@gmail.com>

build
-----

使用cmake构建方式,具体不明白的可以先股沟之.
以下使用ubuntu为例.

### 安装编译包

	$ sudo apt-get install cmake 
	$ sudo apt-get install xenstore-util libxen-dev

其他编译开发包请自行安装.

### 编译

	$ mkdir build
	$ cd build
	$ cmake ..
	$ make
	$ sudo make install

编译的时候可能会提示有一些开发包没有安装.需要自行安装.  
大概这一步不会出现其他问题.  

### 编译选项

可以使用`ccmake ..`来查看支持哪些选项.然后来控制开关等等.按c配置,q退出.  
再用`make`来编译  

* **CMAKE_BUILD_TYPE** 	:	选择Debug,构建调式版本,选择Release,构建发布版本.
* **ONLY_CLIENT** 	 	:	开启已仅仅编译mmclient,可以再虚拟机(guest)中使用该选项.
								不过其实也可以不用
* **UNIT**					:	编译单元测试.虽然现在只有一个测试,而且已经不用再测试了.
* **DEB**					:	开启以支持构建deb包.方便部署.

distribute
----------

要将本地代码部署到服务器和虚拟机群上.有两种方式

* 源代码部署
* 二进制包部署

### 同步到服务器上

在服务器上部署源代码方式,方便直接修正编译运行.不必再来回复制.
这里使用git来完成.对git不熟悉的请自行股沟之.

1. 添加远程reference. 
	$ git remote add origin [ssh addr of server]
_如果是已经有origin了.可以先`git remote rm origin`删除._
例如:
	$ git remote add origin root3@173.26.100.209:xenmm

2. 推送代码
	$ git push origin master

3. 在服务器上刷新
	$ ssh root3@173.26.100.209
	$ cd xenmm
	$ git reset --hard 

4. 	在服务器上编译,测试运行.

### 编译二进制包直接安装

虽然也可以使用git+服务器中介.从虚拟机上pull代码.再编译.  
不过虚拟机10台阿10台,一个一个编译太蛋疼了.所以使用包的方式就很快捷了.  

既然要编译deb包肯定要在ubuntu上编译.或者直接在服务器上编译.  
或者在本地编译在传送到服务器上去.

1. 在服务器上制作deb包
	$ cmake .. -DDEB=ON
	$ make
	$ sudo make package
大概这个时候就可以看到在build文件夹中安静的躺着的*.deb了.

2. 传送deb到虚拟机
这里因为物理机和虚拟机有网络通路(NAT或桥).xen使用的是后者.
在虚拟机中使用`ifconfig`就可以看到了.
可以使用scp等方式来从服务器上获取deb包
例如以下的脚本:
	#!/bin/sh
	scp root3@192.168.122.1:xenmm/xenmm-0.1.1-Linux.deb

3. 安装deb包
	$ sudo dpkg -i xenmm-0.1.1-Linux.deb

useage
------

1. 在虚拟机停机的时候设置好maxmem.
 可以用`xm mem-max`设置,或者是virt-manager.
 max值只能在停机的时候设置,在运行的时候设置下次启动有效.
 当分配的时候超过max值,虽然写入了分配的目标值,但是实际上没有分配.
 会造成数据的非一致性.检测方法见note一节

1. 在虚拟机之中开启client
 因为有脚本.所以就很方便了.
 需要注意/dev/xen/xenbus文件或者是/proc/xen/xenbus
 自己看脚本内容修改.
	$ sudo service mmclientd start

2. 在服务器上运行调节程序
	$ sudo src/mmserver
 之后可以在控制台中看到输出.并且在build文件夹中可以看到log文件.
 log文件的命名规则是:日期_次数_[虚拟机id].log
 次数在这一天之中做试验的次数.如果做了两次试验,那么
 次数有1和2.2是最近做的试验.所以log不会被覆盖.大胆放心的测吧.
 有数据了不管用什么方法.gnuplot,matlab,...或者是excel.
 就可以画图了.

benchmark
---------

### mono test method

1. 在guest端安装deb.见上文.
2. 在host开启mm_server调节程序.最好是在build文件夹用
   `sudo src/mm_server`启用.因为会在当前目录写log记录.
   千万不要用`sudo make install;sudo mm_server`
   这样是使用的/usr/local/bin目录下面的mm_server.
   那么log记录也会写乱.
3. 在guest端使用`mm_test_mono low high`low,high是两个内存数值.
   如`mm_test_mono 50M 400M` M是MB.
4. 观察host端的记录.可以看到某个domain id下面的free会变少.total会变多.
   这个是正在调节.
5. 打开build文件夹下面的log记录.把一个试验里面的所有记录全部复制出来.
   推荐复制到dataset文件夹.然后设置好文件夹名称.例如date_test_param
   date是日期,test是测试项目名称.如mono,dacapo...param是测试参数
   然后再画图什么的...
   如果对mathematica比较熟悉.可以使用notebook文件夹里面已经写好的样本.
   里面也有已经画出来的图,注意不要覆盖了.用追加的方式贴记录.

### dacapo test method

dacapo测试,需要测一下的4组数据.其中每组数据都是时间.
因为dacapo的输出比较乱.推荐手工收集数据.



note
----

### 源码文件夹的组织方式

build		:	cmake编译
notebook	:	mathematica实验nootbook
|--notebook2.nb	:	推导公式合理性相关
|--workload.nb		:	绘制负载图(mono)
script		:	client daemon脚本
src			:	源文件.包含mmserver,mmclient程序
test		:	测试文件,包含mm_test_static,mm_test_mono
unit		:	单元测试

### mm_test_mono

这个程序可以申请内存,从low到high,再从high到low
	mm_test_mono 50M 300M

### mm_test_static

因为就算mono比较好.还是有些太快了.
又或者需要手工设置每个虚拟机的内存.
这个时候就可以使用static了.它能够保持一部分内存.
可以相对比较自由的设置内存.
*13-01-04*:增加不停的写内存的设置.
  因为如果仅仅是申请了而不写入,或者是仅写入一次.那么
  这部分内存就会因为内存替换算法被替换到交换空间.
  达不到我们预期的效果了.

### 记录的说明

记录是每个VM有3个数值,total,used,free.
其中total是从host视角看的.
free是从虚拟机视角看的.
而used是tot-free.其中tot是从虚拟机视角看的总内存.
其中tot!=total,并且他们相差大概有50M左右.是正常范围.

### 不一致性的检测方法

当出现不一致的时候,虚拟机视角的tot不变.而物理机视角的total值不断增加.
造成两者的不一致.这个是程序造成的.但是如果程序不这么写的话.
当出现不一致的时候程序就crash了.所以只能这样.
如果used+free如果和total差距超过正常范围(50M左右)即为不一致.
或者total增加而used+free不变即为不一致.
