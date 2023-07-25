#!/usr/bin/bash
#
# SPDX-License-Identifier: GPL-2.0-or-later
#
# the following is the chkconfig init header
#
# rtapp-example: processor frequency scaling support
#
# chkconfig: 345 99 01
# description: Run realtime application
#

# the following is the LSB init header see
# http://www.linux-foundation.org/spec//booksets/LSB-Core-generic/LSB-Core-generic.html#INITSCRCOMCONV
#
### BEGIN INIT INFO
# Provides: rtapp-example
# Should-Start:
# Default-Start: 3 4 5
# Short-Description: launcher script for realtime application
# Description: This script performs all system manipulations required for
#              starting/stopping rtapp-example (i.e. isolating cpus, moving IRQ
#              affinities, etc). and then starts or stops the rt applications
#              using the tuna command line tool

### END INIT INFO

# Source function library.
. /etc/rc.d/init.d/functions

prog="rtapp-example"

prep() {
    # do core isolation and IRQ affinity mainipulations here
}

rollback() {
    # undo core isolation and IRQ affinity changes here
}

startapp() {
    # start application processes
}

stopapp() {
    # stop application processes
}

start() {
    prep
    startapp
}

stop() {
    rollback
    stopapp
}

status() {
}

usage() {
    echo "$0: start|stop|status|help"
    exit 1
}


RETVAL=0
case "$1" in
    start)
	start
	;;
    stop)
	stop
	;;
    status)
	;;
    *)
esac
exit $RETVAL
