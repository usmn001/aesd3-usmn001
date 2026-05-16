#!/bin/bash
month=$1
date=$2
journalctl -t aesdsocket | grep "$month $date"
sudo truncate -s 0 /var/log/syslog
#service syslog restart
#cd server
#valgrind --leak-check=full --show-leak-kinds=all ./aesdsocket
exit 0