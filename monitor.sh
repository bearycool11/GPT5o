#!/bin/bash

# Define the log file and the network interface
LOGFILE="/var/log/network_monitor.log"
INTERFACE="eth0"

# Start monitoring the network using tcpdump and log the output with timestamps
sudo tcpdump -i $INTERFACE -nn | while read packet
do
  echo "$(date '+%Y-%m-%d %H:%M:%S') - $packet" >> $LOGFILE
done
