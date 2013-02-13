% 多负载调节 开题报告
% 谢虎成

第1章 研究目的和意义
==================

1.1 课题来源
----------

本课题是 973 项目 2005CB321806“虚拟计算环境聚合与协同机制研究”课题“虚拟计算环境实验床与仿真平台”的子课题。
本文主要就虚拟机资源管理中的内存管理进行研究。

1.2 研究意义与目的
---------------

现如今,随着硬件性能的不断提升,特别是在服务器上体现得尤为明显.
单台大型服务器所能够提供的计算能力远远超过它实际所承担的工作.
造成一种利用效率不高的现象.浪费资源和金钱.

为了提高服务器的利用率.虚拟化技术是一种简单方便的方案.
使用虚拟化技术,将一台大型服务器同时虚拟化成数台甚至数十台性能弱的虚拟机.
方便的增加原来数十倍的任务量.真正有效的把服务器充分利用.
这样的成本远远比购买数十台价格低廉的计算机要便宜.在占地和管理方面也更加集中.

因此.虚拟化技术的研究,特别是虚拟化群技术的研究任务尤为迫切.

虚拟化技术通过分时的机制,使得未被充分利用的系统资源在多个虚拟机之间共享,从而节约成本。
而高效的 CPU 和 I/O 设备分时复用已经被广泛的应用,但分时的内存共享实现却很难。
因此,物理内存的管理正日益成为系统虚拟化的一个瓶颈,限制了在物理机上运行多个虚拟机的效率。
一台虚拟机的内存开始的时候一般都是静态分配的,虚拟机运行的过程中分配的总内存的大小不发生变化。
使得对于一些情况,如一台虚拟机高负荷运作,需要更多的内存却不可得,
另外一些虚拟机内存空闲,不需要过多的内存却不得减少.虚拟机资源并不均衡.
没有达到更高的利用率.

通过研究虚拟机的内存负载平衡.能够比较好的解决这类情况的问题.使得进一步的提高服务器的利用率.
节省成本.

第2章 研究现状及分析
=================

2.1 虚拟化技术的历史
-----------------

虚拟化技术已经有 40 多年的历史,它起源于对分时系统的需求。早期的
操作系统只能够处理单个任务,为了能够同时处理多个任务,虚拟化技术作为
最早的分时系统解决方案被提出。IBM 最早发明了一种可以在一台主机上运
行多个操作系统的虚拟化技术,让用户尽可能充分地利用昂贵的大型机资源。
随着计算机技术的不断发展,虚拟化技术又重新成为了商业和学术研究的热点 。

因为虚拟化技术高效的安全性,节能性和对资源的有效利用性,使得它在数据
中心、web 主机,甚至桌面计算中都被广为使用。虚拟机( Virtual Machine)
在服务器虚拟化中使用最为广泛,每个虚拟机都被假想地运行在一个专门的物
理机器上,这些虚拟机受到充分的保护,并且和其他虚拟机是相互隔离的。虚
拟机使抽象服务器的负载更加方便,因为它很好的封装了整个系统的运行状态 ,
包括用户级的应用和操作系统级的服务。

在很多计算环境中,单个的服务器使用效率都很低。而把他们以虚拟的方
式运行在一个物理机器上也不会有太大的性能损失。因此,很多小的服务器,
可以综合起来,以虚拟机的方式运行在很多大的服务器上,从而简化管理、节
省开支。

随着计算机技术的不断发展,x86 处理器的性能不断提升,应用也不断普
及,虚拟化技术的发展也逐渐涉及到 x86 架构。起初 x86 架构的设计者并没有
考虑支持虚拟化技术,它本身的结构和复杂性使得在其之上进行虚拟化非常困
难。在 20 世纪 90 年代末期,VMware 和其他虚拟化软件厂商率先为虚拟化技
术在 x86 下的应用发展开辟了道路,使得虚拟化应用的前景更加广阔。他们实
施的是一种软件解决方案,以 VMM(Virtual Machine Monitor)为中心,使 PC
服务器平台实现虚拟化。然而,这种纯软件的“全虚拟化”模式中,每个客户操
作系统(Guest OS)获得关键平台资源都需要由 VMM 控制和分配,以避免发
生冲突。为此,须要利用二进制转换,而二进制转换的开销又使得“全虚拟化”
的性能大打折扣。为了解决这个问题,引出了一种新的虚拟化模式,即在
Denali 项目和 Xen 项目中出现的“半虚拟化”技术。半虚拟化技术不需要二进制
转换,而是通过对 Guest OS 进行代码修改,从而为操作系统提供了新的接口,
以使新的、定制的 Guest OS 获得额外的性能和高扩展性。但是,修改 Guest
OS 非常繁琐,带来了对一些系统指令级别的冲突以及运行效率问题。

硬件辅助虚拟化技术是,Intel/AMD等硬件厂商通过对部分全虚拟化和半虚拟化使用到的软件技术进行硬件化来提高性能.
硬件辅助虚拟化技术常用于优化全虚拟化和半虚拟化产品,例如Intel的VT-x和AMD的AMD-V.
不仅能提高速度,而且能简化虚拟化技术的架构.具有非常不错的前景.

Xen 虚拟机监视器位于操作系统和硬件之间,为运行在它上面的操作系统
提供虚拟化的硬件环境。Xen 采用混合的虚拟化模式,因此在 Xen 众多的
Domain 中,存在一个特权域用来辅助 Xen 管理其他 Domain,提供相应的虚拟
资源服务,特别是其他Domain对I/O设备的访问。这个特权域称为
Domain0,而其他则称为 DomainU。Xen 的体系结构如下图所示[^archi]。

![architecture](graph/architecture.png "xen 架构")

[^archi]: the graph is referenced from\
http://www.intel.com/technology/itj/2006/v10i3/3-xen/figures/figure_1_lg.gif\
https://community.emc.com/servlet/JiveServlet/downloadImage/38-3466-30315/620-465/Xen.png\
http://docs.fedoraproject.org/zh-CN/Fedora/12/html/Virtualization_Guide/images/Xen_full_virtualization_architecture.png

2.2 国外研究现状及分析
--------------------

Carl A. Waldspurger 研究的 VMware ESX Server 中的多种内存管理机制[@Waldspurger2002]引
导了很多研究人员对虚拟机的内存的研究工作。他提出的气球驱动机制被引入
了 Xen 虚拟机监视器;另外,VMware ESX Server 使用了基于共享的分配策略,
每个虚拟机的共享由它正在使用中的内存以及先前管理员设定的最大最小内存
值决定。VMware ESX Server 通过取样的方法决定正在使用中的内存大小,而
本文采用的 Linux 操作系统的内核对外统计接口决定正在使用中的内存量。本
文参考了他提出的基于共享的空闲内存税方法,来计算分配给每个客户操作系统的内存值。

Stephen T. Jones 等人在 Xen 虚拟机监视器上实现了 Geiger[@Jones2006] 系统,该系统通
过监视缓冲区高速缓存,精确地推断页面什么时候被插入系统高速缓存,什么
时候从高速缓存中驱逐出来。系统还实现了一组预测机制,使得虚拟机监视器
能够做出更加明智的内存分配策略。但是,当一个客户操作系统分配多余的内
存时,Geiger 无法决定从怎么回收空闲内存供其他客户操作系统使用。

与本文工作比较相似的有 Dan Magenheimer 的内存超额提交机制[@Magenheimer2008]和 Weiming
Zhao 等人实现的虚拟机的动态内存平衡机制[@Zhao2009]。内存超额提交机制利用了操作
系统本身的内存数据统计信息来指导内存资源的管理,但内存超额提交机制只
是从每个虚拟机角度来进行内存管理,无法从全局的角度考虑内存资源的使用
情况。内存超额提交机制也是本文自发调节机制的原型,本文工作也是对
Magenheimer 工作的延伸。
Weiming Zhao 等人实现的虚拟机动态内存平衡机制,通过修改虚拟机监视
器的内存管理代码来获得每个虚拟机对物理内存的访问信息,也即拦截虚拟机
的内存访问。然后构造基于 LRU(Least Recently Used)的内存预测器预测每
个虚拟机所需的内存,从而可以知道从哪些虚拟机中回收内存。另外,动态内
存平衡机制也采用了操作系统本身的内存统计信息,通过操作系统交换空间的
使用情况检测内存不足。
总之,多人的研究工作都是从虚拟机内核或者操作系统内核的角度完成对
内存的管理工作,而本文利用了现有的驱动和内核接口,从应用的角度完成了
虚拟机的内存管理工作,也是对虚拟机内存管理的一类探索。

第3章 主要研究内容
================

虚拟机资源管理包括对内存、CPU 和 I/O 设备等资源的管理。本文主要是
基于 Xen 虚拟机监视器,研究开发一种多个虚拟机之间平衡内存资源的系统。

3.1 内存超额机制
-----------

因为我们主要是直接承接Dan Magenheimer的内存超额机制的工作.
所以这里有必要先描述清楚Dan Magenheimer的内容以及我们与之的异同.

Dan Magenheimer的工作已经被收录在了Xen源代码之中`/tools/xenballoon/*`.
主要是提供了一个shell脚本文件.作为daemon程序在后台运行.

他的工作基于早期的Xen版本.现在已经无法直接使用了.
其核心思想是通过读取`/proc/meminfo`里的`Committed_AS`值作为目标内存大小.
然后根据设置好的maxmem和minmem,将结果强制调节到大小区间之内.
另外还设置了上界磁滞值和下界磁滞值.可以理解为当前内存到目标结果的速度.
并不是一下子将总内存设置为目标值大小.而是渐渐的接近它.这样的设计对于振荡现象能够
比较好的解决.
最后.将得到的结果实际写入`/proc/xen/balloon`文件调用气球驱动进行实际的调节.

总的说来.Dan Magenheimer的工作作为尝试性质的Demo,其思路简洁.程序短小.
并且有人反映其程序的确具有一定的效果.但是他的测试和研究工作都比较少.之后就没有在
继续发展维护代码了.导致他的程序已经不能在高版本的Xen上面运行了.因为最核心的文件
`/proc/xen/balloon`已经不再提供了.

另外,他的程序是单客户端的设计.其实际调节公式非常简单.是在Domain-U和Domain-0之间进行
内存调节.并不在Domain-U和Domain-U之间进行直接的内存平衡.引用Magenheimer的原话:

>We have not yet implemented a directed ballooning management 
>tool in domain0 as selfballooning seems to be working very nicely so far.

在其程序中预留了direct balloon的位置.但是他并没有给出direct balloon的实现.

我们的设计借鉴了Magenheimer的思想.即通过/proc文件系统来获取虚拟机内部信息.
并且通过XenStore来暴露给仲裁端.

并且我们再其基础之上设计了等效于direct balloon地位的平衡算法.并且完全移除了self balloon.
因为新的Xen没有`balloon`文件.导致了self balloon完全无法实现.如果是将selfballoon的工作
转移到仲裁端.那么self balloon和direct balloon的相互关系就会变得混乱不堪.
并且我们认为self balloon的调节工作在有了平衡算法之后完全没有必要.因为其调节过于简单.既然有了
平衡算法.那么在进行自发调节就显得多余了.

为此我们设计为仅仅在Domain-U之间进行内存调节.而不再支持Domain-0与Domain-U的内存调节.也就是
说所有虚拟机的可调配内存之和在一开始的时候已经固定了.Domain-0的内存也固定不变了.这是根据Xen Best
Practices[^xbp]中的第一条`Xen dom0 dedicated memory and preventing dom0 memory ballooning`

> You should always dedicate a fixed amount of RAM for Xen dom0.

和第二条`Why should I dedicate fixed amount of memory for Xen dom0?`

>Now, if you boot up the system with dom0 having all the memory visible to it, and then balloon down
> dom0 memory every time you start up a new guest, you end up having only a small amount of the
> original (boot time) amount of memory available in the dom0 in the end. This means the calculated
> parameters are not correct anymore, and you end up wasting a lot of memory for the metadata for a
> memory you don't have anymore. Also ballooning down busy dom0 might have bad side effects.

作为依据的.

[^xbp]: http://wiki.xen.org/wiki/Xen_Best_Practices

3.2 XenStore
------------

在XenStoreReference[^1]页面中的Oerview中介绍到:

[^1]: http://wiki.xen.org/wiki/XenStore_Reference

>XenStore 是一种树型的在域之间共享的命名空间(类似于sysfs或者Open Firmware).Xen提供的最基本的域之间的交互是非常低等级的(虚拟IRQ和共享内存).XenStore是在其之上构建的更加高级的操作(读写键值,枚举目录,当一个键值改变的时候提供通知).
>
>XenStore是一个提供事务和原子操作的数据库,由domain0持有.它能够通过在Domain0里面的socket来访问,或者通过ioctl界面操作/proc/xen/xenbus.XenStore可以通过在<xs.h>定义的函数来访问.XenStore被用来储存域运行时候的信息并且作为一种提供可控的Domain-U设备的机制.
>

XenStore为虚拟机之间的交互提供了一种方便快捷的通道.

XenStore是树型的文件结构.其跟目录下面有3个主要的分支:

    /vm               ---   保存域的配置信息
    /local/domain     ---   保存本地节点的域的信息
    /tool             ---   保存各种工具的信息

我们主要使用`/local/domain`路径下的内容[^2]

    /local/domain/<domid>/
        └---memory/        ---   内存信息的目录
              └---target   ---   域的目标内存大小(KB为单位)
              └---tot*     ---   域的总内存大小
              └---free*    ---   域的可用内存大小

[^2]: 其中加*为新创建的文件 

target和tot的区别是:target是仲裁端写入客户端,tot是客户端写入仲裁端.

为了使用XenStore,其主要的API如下:

-----------------------------------------------------------------------
function        prototype                              descript
---------       -----------                            -------------
xs_daemon_open  struct xs_handle* \xs_daemon_open();   打开Domain-0的句柄

xs_domain_open  struct xs_handle* xs_domain_open();    打开Domain-U的句柄

xs_close        void xs_close(struct xs_handle*);      关闭一个打开的句柄

xs_transaction  xs_transaction_t xs_transaction_start  开启一个事务,在此事务中其他更改不可见,
_start          (struct xs_handle *h);                 并且自身的更改对其他也不可见,直到结束.失败时返回NULL.

xs_transaction  bool xs_transaction_end(               结束一个事务,如果abort为true,事务被取消而不是提交.
_end            struct xs_handle *h,                   失败时返回false,如果errno==EAGAIN,需要重新开始
                xs_transaction_t t,                    事务.
                bool abort);


xs_write        bool xs_write(struct xs_handle *h,     将值写入单个文件,
                                xs_transaction_t t,    失败时返回false.
                                const char *path, 
                                const void *data, 
                                unsigned int len
                                );

xs_read         void *xs_read(struct xs_handle *h,     获取单个文件的值,
                               xs_transaction_t t,     以NULL结束.
                               const char *path,       返回一个malloced的值,
                               unsigned int *len);     使用完后调用free()释放.

xs_directory    char **xs_directory(struct xs_handle*  获得一个目录的内容,
                               h,                      返回一个malloced的数组.
                               ,xs_transaction_t t,    使用完后用free()释放.
                               const char *path,       num为数组的大小.
                               unsigned int *num);      
             
---------------------------------------------------------------------------

3.3 气球驱动
-----------

3.4 libxenctrl
--------------

> libxenctrl 提供最低级的对虚拟机监控程序的C语言函数库接口.对于大多数超级调用,
> 它只是基础地直接将超级调用暴露出来作为简单的C API.

![xen architecture](graph/xen_architecture.png "xen architecture")

libxenctrl作为底层API提供了非常多的功能.其中我们需要的是和气球驱动相关的API:

-----------------------------------------------------------------------
function            prototype                              descript
---------           -----------                            -------------
xc_interface_open   int xc_interface_open(void);           打开一个句柄

xc_interface_close  int xc_interface_close(                关闭一个句柄
                       int xc_handle);

xc_domain_setmaxmem int xc_domain_setmaxmem(int xc_handle, 设置最大内存
                        uint32_t domid,
                        unsigned int max_memkb);

xc_domain_set_pot_  int xc_domain_memory_set_pod_target(   设置目标内存
target                          int xc_handle,             pod=populate on demand
                                uint32_t domid,
                                uint64_t target_pages,
                                uint64_t *tot_pages,
                                uint64_t *pod_cache_pages,
                                uint64_t *pod_entries);

----------------------------------------------------------------------------------

3.5 proc文件系统
--------------

proc文件系统[@M.TimJones2006]是一个虚拟文件系统,是进程文件系统的缩写(process file system).
最大的特点是动态生成,不占用储存空间,仅仅占用有限的内存资源.

proc文件系统使用方便.因为是文本格式.可以使用多种工具(如`cat`命令)来修改或者查看.要用编程方法来使用,
也非常简单.只需要使用基本的c语言的文件操作就可以完成了.

proc文件系统一个最常用的功能就是查看系统信息.例如著名的procps工具集[^procps](提供top,ps,free等系统工具)
就是完全从proc文件系统获取信息.

[^procps]: the /proc file system utilities . 
      url:http://procps.sourceforge.net/

proc文件系统提供两个标准文件`/proc/cpuinfo`和`/proc/meminfo`.分别用于查看cpu基本信息和内存基本信息.
我们在这里主要使用`meminfo`文件中的部分信息.

一个典型的`/proc/meminfo`文件:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MemTotal:        5076744 kB
MemFree:          527404 kB
Buffers:          220688 kB
Cached:          1479788 kB
SwapCached:            0 kB
Active:          3374232 kB
Inactive:         835848 kB
Active(anon):    2515728 kB
Inactive(anon):    12972 kB
Active(file):     858504 kB
Inactive(file):   822876 kB
Unevictable:          80 kB
Mlocked:              80 kB
SwapTotal:             0 kB
SwapFree:              0 kB
Dirty:               108 kB
Writeback:             0 kB
AnonPages:       2509748 kB
Mapped:           379144 kB
Shmem:             19080 kB
Slab:             165852 kB
SReclaimable:     128968 kB
SUnreclaim:        36884 kB
KernelStack:        4600 kB
PageTables:        50208 kB
NFS_Unstable:          0 kB
Bounce:                0 kB
WritebackTmp:          0 kB
CommitLimit:     2538372 kB
Committed_AS:    6174040 kB
VmallocTotal:   34359738367 kB
VmallocUsed:      326708 kB
VmallocChunk:   34359400444 kB
HardwareCorrupted:     0 kB
AnonHugePages:         0 kB
HugePages_Total:       0
HugePages_Free:        0
HugePages_Rsvd:        0
HugePages_Surp:        0
Hugepagesize:       2048 kB
DirectMap4k:      411024 kB
DirectMap2M:     4829184 kB
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


第4章 研究方案
============

我们理论设计包括:

* 公式的设计
* 理论评估

我们的目标系统基本设计为2个部分:

* 客户端: 主要负责记录需要的信息,包括内存使用量等等.定时发送给仲裁端.
* 仲裁端: 主要负责使用客户端收集到的信息.并且集中运算.并把运算结果即
  分配方案应用于各个虚拟机.

还有一些小部分包括:

* 测试样例: 方便完成测试之用
* 实验评估

4.1 研究方案
-----------

研究方案主要分为2个部分:

(1)  架构设计
(2)  公式设计
(3)  代码编写
(4)  数据测量

<!-- -->

### 4.1.1 架构设计 ###

架构上主要分为客户端和仲裁端.
其中客户端运行在Domain-U上.定时收集内存信息.然后通过调用XenStore API.写入共享数据库.
仲裁端运行在Domain-0上.定时通过调用XenStore API读取各个虚拟机的内存信息.进行运算之后.
根据结果调用libxenctrl的接口.使用气球驱动来完成内存的分配.

这样一个简洁的模型.

![framework](graph/framework.png)

特别的.为什么采用这样的C/S模型.而不是仅仅的只有仲裁端,或只有客户端.
是因为,对于Xen来说.运行着的虚拟机就是一个黑箱.Xen无法得知虚拟机内存的使用情况.
所以需要在虚拟机内部运行一个本地程序.这样程序就能够通过调用操作系统的接口来获得内存的使用信息.
所以客户端的存在是必要的.

另外.类似于这种客户端模型的还有VirtualBox虚拟机.使用VirtualBoxAddition程序套件(也是运行
在虚拟机内运行的驱动程序和应用程序集).来为虚拟机提供增强功能.如更好的图形显示和文件共享.
如果没有安装VirtualBoxAddition套件或者是对于某个操作系统没有套件.那么虚拟该操作系统的性能就会
十分低效.

另外.在早期的Xen.在为半虚拟化的操作系统提供了/proc/xen/ballon文件.可以通过向该文件写入内存大小
来直接使用气球驱动.在Dan Magenheimer的内存超额机制[@Magenheimer2008]中.就是主要使用了这个特性.所以它能够在Domain-U中直接完成内存的更改.

但是后期的Xen已经没有该文件了.所以Magenheimer提供的程序已经不能够直接运行了.
因此必须在Domain-0中通过调用Xen的API接口.来使用气球驱动.所以仲裁端的存在也是必要的.

另外.如果仅仅是在客户端执行内存的更改.也无法方便的获取其他虚拟机的内存信息.
所以这样的设计并没有什么可借鉴之处.

### 4.1.2 公式设计 ###

公式是整个工作的核心.一个好的公式决定了可用性和性能.以及未来发展的潜力.

整个公式的核心使用了Carl A. Waldspurger在VMware ESX Server中定义的空闲内存税
的概念.

其描述如下:

$\rho=\frac{S}{P(f+k(1-f))}$

$\rho$为每页股份率\
S为客户的股份\
P为当前分配的内存页\
f为处于活动状态的内存百分比\
k为空闲页开销,$k=\frac 1 {1-\tau}$
$\tau$为空闲内存税,指定了可以回收多少空闲内存资源.

该公式将不同虚拟机的内存用一个统一等价量来表示.使得内存之间的比较成为可能.
通过安排不同的虚拟机的内存,使得每个虚拟机的每页股份率接近相等,来达到平衡的状态.

### 4.1.3 代码编写 ###

为了更加高效的完成代码的编写.使用git来进行代码的版本管理.它能够使得在非破坏性的基础之上不断扩充代码.

### 4.1.4 数据测量 ###

4.2 进度安排
-----------

4.3 预期达到的目标
----------------

第5章 所需条件及经费
==================

第6章 预计困难和问题
==================

6.1 可能遇到的困难和问题
---------------------

6.2 解决措施
-----------

参考文献
=======

