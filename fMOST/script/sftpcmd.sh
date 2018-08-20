#!/usr/bin/expect

# usage:
# expect sftcmd.sh
# nohup expect sftcmd.sh &
# "get" download a file to current folder
# "put" upload a file from current folder
# mget *

spawn sftp -oPort=22 ${Usrname}@${ServerIP}
expect "password:"
send "${Password}\n"
expect "sftp>"
send "cd ${Folder}\n"
expect "sftp>"
send "get ${File}\n"
expect "sftp>"
send "bye\n"
