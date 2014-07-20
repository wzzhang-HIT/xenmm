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
-  **TAX_RATE** : set tax rate directly, the default value is **AUTO** the
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

benchmark
==========

the benchmark explain how to collect test data

mono test 
----------

1.  do the steps in usage_
2.  run ``mm_test_mono <low> high`` on guest vm, <low> and <high> are numbers
    of memory, for example ``mm_test_mono 50M 400M`` 
3.  see the terminal output changes on host, you can see one domain `free`
    memory turn less, `total` turn more. that proves the adjust is working.

dacapo test
------------

dacapo test runs under 4 situations, **low load & no adjust**, **low load &
adjust**, **high load & no adjust**, **high load & adjust**. watch execution
time of dacapo. 

high load is simulated by running load program ``mm_test_static`` 

to record swap usage, can run ``mm_util_swap`` as well

if need run dacapo test many times ( to work with `phoronix-test-suite test`_ )
use ``dacapo_test.sh`` script, for example::

    $ ./dacapo_test.sh h2 10  # run h2 10 times
    $ ./dacapo_test.sh all    # run all test once
    $ ./dacapo_test.sh 'h2 tradebeans' 5 # run h2 and tradebeans 5 times

phoronix-test-suite test
-------------------------

the test under multi vms is based on ``phoronix-test-suite`` test suites.
phoronix-test-suite runs under interactive mode, which doesn't we want. so we
need use ``phoronix-test-suite batch-steup`` to finish steup, and use
``phoronix-test-suite batch-run`` to runs under automatic mode.

to increase a test running time(ensure all vms test runs enough long), need use
`TOTAL_LOOP_COUNT` environment. for example::

    $TOTAL_LOOP_COUNT=10 phoronix-test-suite batch-run nginx

or::

    $ export TOTAL_LOOP_COUNT=10
    $ phoronix-test-suite batch-run nginx

this would run nginx test for 10 times, because phoronix would give out every
run used time. the total run time could be estimated out. Also,
phoronix-test-suite couldn't use ``Ctrl+C`` to force stop during running. we
should ``touch ~/.phoronix-test-suite/halt-testing`` to stop after this run
end. And produce test result.

dynamic display
================

dynamic display is view changes dynamicly in a webpage.

should open **SOCK** option when compile, then use ./start.sh under canvas
directory.

source directories
====================

+  build : cmake compile director
+  notebook : mathematica experiment notebook
+  script : client daemon script
+  src : source , include ``mmserver`` , ``mmclient`` program.
+  test : test program, include ``mm_test_static`` , ``mm_test_mono``
