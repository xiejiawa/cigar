#!/usr/bin/expect -f

set devIp [ lindex $argv 0 ]
set userName [ lindex $argv 1 ]
set passwd [ lindex $argv 2 ]
set cmdFile [ lindex $argv 3 ]
set timeout -1

if {$argc != 3} {puts "Usage: $argv0 Device_ip cli_user os_password cmdfile"; exit 1}
spawn ssh root@$devIp
expect {
    "*yes/no" {send "yes\r"; exp_continue}
    "*assword:" {send "root\r"}
}
expect ".*#" {send "telnet localhost 60000\r"}
expect "Login:" {send "$userName\r"}
expect "Password:" {send "111111\r"}
interact
