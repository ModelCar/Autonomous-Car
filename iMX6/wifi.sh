#!/bin/bash
dmesg -n 3
echo "######bringing interface down"
sleep 1
ifdown --force wlan0
echo "######removing rfkill flag"
sleep 1
rfkill unblock all
echo "######bringing interface up"
sleep 1
ifup wlan0 2>&1 >/dev/null &

echo "######establishing connection"
sleep 1
until iw wlan0 connect -w CarNet 2>&1 | grep "ready\|progress"
do
  iw wlan0 connect -w CarNet
done

echo "######checking connection"
sleep 1
until ping -qc 1 -W 1 127.0.0.1 2>&1 | grep "round-trip"
do
  sleep 1
done
dmesg -n 7
sleep 4
echo "####### changing IP to 10.42.1.2"
ifconfig wlan0 10.42.1.2
echo "####### successfully connected"


