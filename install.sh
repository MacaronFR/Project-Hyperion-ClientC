#!/bin/bash
#must run in sudo
DIR=$(dirname "$0")
if [ "$EUID" -ne 0 ]
	then echo "Please run script as root"
	#exit error
	exit 1
fi
apt install libxml2-dev libcurl4-gnutls-dev libmariadb-dev cmake
#change install dir (default /usr/bin)
read -p "Where install binaries ? (default in /usr/bin/)" bindir
if [ -z "$bindir" ]
then bindir="/usr/bin/"
fi
if [ "${bindir: -1}" != "/" ]
then bindir="${bindir}/"
fi
#cmake command
cmake "$DIR/CMakeLists.txt"
#create bin dir if not exist
mkdir -p "$bindir"
#make
cd $DIR
make
#move result in bin dir
mv "$DIR/clientc" "${bindir}hyperion_database_saver"
#delete all build file
rm "$DIR/CMakeCache.txt" "$DIR/CMakeFiles/" "$DIR/cmake_install.cmake" "$DIR/Makefile" -R

#########################################
#CREDENTIALS
#########################################
#deposit credentials
read -p "Deposit ID : " id
read -p "Deposit name : " name
read -p "XML saving path (default /var/hyperion/xml/) : " xml
read -p "Remote XML saving path : " remoteXML

if [ -z "$xml" ]
then xml="/var/hyperion/xml/"
fi

if [ "${xml: -1}" != "/" ]
then xml="${xml}/"
fi
if [ "${remoteXML: -1}" != "/" ]
then remoteXML="${remoteXML}/"
fi
mkdir -p "$xml"

depositcredentials="[DEPOSIT]\n${id}\n${name}\n${xml}\n${remoteXML}\n\n"

#DB credentials
echo -e "\n"
read -p "Database host : " bddhost
read -p "Database port : " bddport
read -p "Database name : " bdd
read -p "Database username : " bdduser
read -s -p "Database password : " bddpasswd
bddcredentials="[BDD]\n${bddhost}\n${bdduser}\n${bddpasswd}\n${bdd}\n${bddport}\n\n"

#SFTP credentials
echo -e "\n\n"
read -p "SFTP Host : " sftphost
read -p "SFTP port : " sftpport
read -p "SFTP username : " sftpuser
read -s -p "SFTP password : " sftppasswd
sftpcredentials="[SFTP]\n${sftphost}\n${sftpuser}\n${sftppasswd}\n${sftpport}\n\n"

credentials="${bddcredentials}${sftpcredentials}${depositcredentials}"

echo -e "\n\nIf you misspelled something, all setup info is in the file located at /etc/hyperion/database_saver.conf\n\n"

# Write credentials to conf file
mkdir -p "/etc/hyperion"
echo -e "$credentials" > "/etc/hyperion/database_saver.conf"

touch "${xml}sftp.test"
echo "Deposit ${id}${name} test SFTP connection" > "${xml}sftp.test"

hyperion_database_saver --test BDD
if [ $? != 0 ]
then echo "Cannot connect Database"
else echo "Connection successful to Database"
fi
hyperion_database_saver --test SFTP
if [ $? != 0 ]
then echo "Cannot connect SFTP"
else echo "Connection successful to SFTP"
fi

#create cron task

echo "30 23 * * * hyperion_database_saver" >> /var/spool/cron/crontabs/root

#exit OK
exit 0
