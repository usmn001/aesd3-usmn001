#!/bin/sh
case "$1" in
    start)
     echo "Starting simple server"
     start-stop-daemon -S -n simpleserver -a /etc/init.d/S99aesdsocket -d
     ;;
     stop)
     echo "Stopping simple server"
     start-stop-daemon -K -n simpleserver
     ;;
     *)
     echo "Usage : $0 {start|stop}"
    exit 1
easc
exit 0     