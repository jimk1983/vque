#!/bin/sh
workdir=$(cd $(dirname $0); pwd)
echo $workdir/AccessGateway

user=root
password=uj123456
ip=192.168.0.101
port=10059

filename=("AccessGateway" "install_access.sh")
echo filename is ${filename[@]}
echo file nums is ${#filename[@]}

for name in ${filename[@]}
do
echo uploading $name
sshpass -p $password scp -P $port -r ./$name $user@$ip:/tmp
done

sshpass -p $password ssh -p $port $user@$ip << !SSH!
if [ ! -d "/home/access001" ];then
mkdir /home/access001
fi
cd /home/access001
echo stop accessgw service
#关闭远程服务
service accessgw stop
mv ./*.log /tmp
cp -f /tmp/AccessGateway ./
echo start accessgw service
#启动远程服务
service accessgw start
!SSH!

exit 0
