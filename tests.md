simple
------

dacapo
------

### avrora ###

模拟一组程序在AVR微控制器上运行

### batik ###

根据Apache的Batik产生一组SVG图像

### eclipse ###

为Eclipse IDE执行一些非GUI的jdt性能测试

### fop ###

分析和格式化XSL-FO文件，并生成PDF文件

### h2 ###

执行类似于JDBCbench的内存评估，执行一组和
银行程序模型相关的事务。代替了旧的hsqldb测试

[内存消耗]：巨大

### jython ###

pybench 一个Python的评估解释器。

### luindex ###

使用lucene来建立一些文件的索引;是莎士比亚
和King James Bible的作品

### lusearch ###

使用lucene来做一些文本搜索;基于莎士比亚和
King James Bible作品中的一些词语

### pmd ###

分析一些Java的Classes文件，检查一些源代码问题。

### sunflow ###

使用光线追踪渲染一些图片

### tomcat ###

运行一些Tomcat服务器的检索和验证网页结果的查询。

### tradebeans ###

通过jave beans运行DayTrader基准测试,与H2作为内存中的Geronimo后端底层数据库

JavaBeans是一种规范，一种在Java（包括JSP）中使用可重复使用的Java组件的技术规范。其次，JavaBeans是一个Java的类.

Apache Geronimo 是 Apache 软件基金会的开放源码J2EE服务器

[内存消耗]：巨大

### tradesoap ###

通过SOAP运行DayTrader基准测试,与H2作为内存中的Geronimo后端底层数据库

SOAP（Simple Object Access Protocol）简单对象访问协议,SOAP类似传统的二进制协议IIOP（CORBA）和JRMP（RMI），但它不采用二进制数据表示法，而是采用使用XML的，基于文本的数据表示法。

[内存消耗]：巨大

### xalan ###

把XML转换成HTML

phoronix-test-suite
--------------------

phornix-test-suite是开源跨平台的测试框架，提供方便易用的测试环境，
可以方便的从服务器中下载测试集进行测试，超过100个以上的现成的测试涵盖了
包括cpu,disk,system,gpu,等等方面的测试程序。可以自由选择。
使用系统源提供的依赖，安装方便。

下面是本次测试选用的一些测试程序的说明：

### Apache ###

该测试是使用Apache的ab基准测试程序,该测试测量当处理700000个请求,并且保持100个执行并发请求.系统能够维持每秒种处理多少的请求.

该测试主要体现静态Web服务器的性能.

[内存消耗]：后期较大

### build-linux-kernel ###

该测试测量编译Linux 3.1 内核需要多长时间.

编译Linux内核是非常常用的测试,测试系统综合性能,直观方便.

[内存消耗]：后期巨大

### pgbench ###

该测试是一个简单的PostgreSQL的类似TPC-B基准测试.

是磁盘密集型程序,主要体现测试数据库的性能

[内存消耗]：中

### John-the-ripper ###

是一款密码破解程序,是cpu密集型程序,主要体现cpu性能

[内存消耗]：较小

### scimark2 ###

用于测试科学和计算能力的基准工具

包括下面的项目

FFT:  Fast Foruier Transform  快速傅里叶变换

Jaccobi SOR:  Jacobi Successive Over-relaxation       逐次超松驰迭代法(连续过松弛算法)(Successive Over Relaxation Me thod,简称SOR方法)是高斯\[LongDash]塞德尔方法( Gauss\[Dash]Seidel   method)的一种加速方法，是解大型稀疏矩阵方程组的有效方法之一，它具有计算公式简单，程序设计容易,占用计算机内存较少等优点,但需要较好的加速因子(即最佳松驰因子).   the Jacobi method 是其中的一个方法。

Monte Carlo： 蒙特卡罗算法  也称统计模拟方法，是二十世纪四十年代中期由于科学技术的发展和电子计算机的发明，而被提出的一种以概率统计理论为指导的一类非常重要的数值计算方法。是指使用随机数（或更常见的伪随机数）来解决很多计算问题的方法。与它对应的是确定性算法。

Sparse Matrix Multiply： 稀疏矩阵乘法，

稀疏矩阵向量乘(Sparse Matrix-Vector Multiplication，SpMV)是科学计算领域一个非常重要的内核，在求解稀疏线性方程组的迭代法中占据了主要的计算量。SpMV计算y=Ax，其中A是规模为M*N的稀疏矩阵，x和y分别为长度为N和M的向量。

dense LU matrix factorization： 稠密矩阵的LU分解计算， LU decomposition (也叫 LU factorization)

在线性代数中， LU分解是矩阵分解的一种，可以将一个矩阵分解为一个下三角矩阵和一个上三角矩阵的乘积（有时是它们和一个置换矩阵的乘积）。LU分解主要应用在数值分析中，用来解线性方程、求反矩阵或计算行列式。

主要测试面向科学计算的服务器性能,cpu集中型

[内存消耗]：较小

### system-libxml2 ###

该测试测量使用libxml2库分析一个随机生成的XML文件所用的时间。
通过使用xmllint的streaming接口。

该测试测量从1KB到112MB大小的XML文件处理能力，因为libxml2同时提供DOM方式和SAX方式处理XML文件。

在DOM方式中，需要将XML完全加载到内存中解析，是内存集中型。
在SAX方式中，是流式的处理XML，内存开销小。

可惜该测试明确指明是streaming接口，即SAX方式，所以不是内存集中型

[内存消耗]：较小

### compress-7zip ###

该测试使用p7zip自身集成的基准测试特性来测试7-zip格式压缩性能。

不同压缩格式对内存的压力不同，类似7zip这种高压缩率的格式，可以选择不同的压缩等级。
在高压缩等级消耗的内存是非常可观的。从该测试最终的结果可以看到，实际上是内存密集型。
该基准测试前段时间测试低压缩率，后段时间测试高压缩率，内存消耗非常巨大。

[内存消耗]：巨大

### openssl ###

CPU集中型

### crafty ###

是一款棋类游戏的算法,CPU集中型.

