#!/bin/sh
user=root
password=uj123456
ip=192.168.0.101
port=10059

echo geting logs from $ip:$port... 
sshpass -p $password scp -P $port $user@$ip://home/access001/*.log ./
exit 0