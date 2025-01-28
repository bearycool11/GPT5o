import socket
import struct

# Create a raw socket to monitor ICMP requests (pings)
sock = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_ICMP)
while True:
    packet, addr = sock.recvfrom(1024)
    print(f"Ping detected from: {addr}")
