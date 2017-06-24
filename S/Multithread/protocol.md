# Protocol

#### message protocol

++++++++++++++++++++++++++++++++++++++++++++++++++++</br>
##### raw message
filename: ip &lt;binary file> (ip is the client ip, not the ip in packet)</br>
format: length[2 bytes] + packet[ethernet][length bytes] + length[2 bytes] + packet[ethernet][length bytes]...</br>

##### Processed meaasge
filename: source ip_destination ip &lt;binary file> (for eaxmple: 192.168.1.1_57.63.37.21)</br>

problem: IP fragmentation and reassembly</br>
id + flag + offset</br>
format: tcp[74 63 70 00] + length[2 bytes] + packet[tcp][length bytes]+ length[2 bytes] + packet[tcp][length bytes]...</br>
format: udp[75 64 70 00]

++++++++++++++++++++++++++++++++++++++++++++++++++++</br>
