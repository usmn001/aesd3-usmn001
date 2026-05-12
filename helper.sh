#!/bin/bash
journalctl -t aesdsocket
sudo truncate -s 0 /var/log/syslog
service syslog restart
cd server
valgrind --leak-check=full --show-leak-kinds=all ./aesdsocket
exit