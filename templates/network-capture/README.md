# Network Capturing

All programs uses raw sockets to capture incoming packets.

## ICMP Sniffer

This program listens to incoming `ICMP` packets and prints information about them including the packet sender's IP address, packet type and Time To Live (TTL) value.

## TCP Sniffer

This program listens to incoming `TCP` packets and prints the data in the payload of each packet.

## UDP Sniffer

This program listens to incoming `UDP` packets and prints information about them including the sender's IP address, sender and receiver ports, and length of the payload.
