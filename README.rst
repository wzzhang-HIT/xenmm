================================
xenmm --- xen memory management
================================

:Authors: -  wzzhang-HIT <wzzhang@hit.edu.cn>
          -  xiehuc      <xiehuc@gmail.com>
:License: GPLv3

this simple program is used for adjust memory of xen virtual machines
it is based on xen balloon driver and adjust balance algorithm.

build
======

depends on ``xenstore-util`` and ``libxen-dev`` packages.
use ``apt-get install`` to install them.

.. code:: bash

   $ mkdir build
   $ cd build
   $ cmake ..
   $ make
   $ sudo make install

other build option
--------------------

use ``cmake .. -D<<OPTION>>=<<VALUE>>`` to add more control. such as ``cmake ..
-DONLY_CLIENT=On``

-  **ONLY_CLIENT** : only compile ``mmclient`` program, you should consider this
   when build on guest virtual machines to simplify build
-  **DEB** : make a deb package to directly install for many virtual machines
-  **RPM** : make a rpm package to directly install for many virtual machines
-  **TAX_RATE** : set tax rate directly, the default value is **0.75** the
   possible values are:

   - **AUTO** : enable dynamic tax rate ability
   - **0.0~1.0** : set tax rate as a static constant value

-  **SOCK** : enable socket, that means you can visit **127.0.0.1:9091** to get
   vms' memory footprint as JSON format. and ``canvas/start.sh`` is avaliable

usage
=======

to use program correctly, you should do following steps:

1.  use ``xm mem-max`` to set maxmem when halt virtual machines. the program
    adjust effect wouldn't exceed maxmem, this is strict limit.

2.  start client program in guest vms, because there is a script to help do
    this work, just simply use ``sudo service mmclientd start`` to start
    client. 

.. tip::
   if your machine is systemd, you can just copy commands in mmcliend and enter
   in terminal by hand.

3.  run adjust program in host vm::

        cd build
        sudo src/mmserver
    
    then you can see adjust outputs on terminal. and the log files in ``build``
    directory. log files' name is based on ``date_times_[vm's id].log`` pattern.
    so, it would **not** override previous log files accidently. don't be worry.
    since you have logs, you can draw curves with tools such as gnuplot,
    matlab, ... as well as libreoffice draw. 
