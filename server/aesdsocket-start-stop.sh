#!/bin/sh

case "$1" in
    start)
        echo "Starting simple server"
        start-stop-daemon -S -n simpleserver -a /usr/bin/simpleserver -d
        ;;
    stop)
        echo "Stopping simple server"
        start-stop-daemon -K -n simpleserver
        ;;
    *)
        echo "Usage : $0 {start|stop}"
        exit 1
        ;;
esac

exit 0     