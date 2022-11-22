import hid
from scapy.all import *
import time

contVID = 0x054c
contPID = 0x09cc
arduino_mac = "e0:98:06:22:fd:ef"
laptop_mac = "12:F4:8D:87:5C:CD"
interface = "Microsoft Wi-Fi Direct Virtual Adapter"
ip = ""
port = 8888
LX = 5
LY = 7
RX = 9
RY = 11
dpad_index = 0

class ControlArray:
	def __init__(self, lx, ly, rx, ry, button, dpad) -> None:
		self.array = [int(lx), int(ly), int(rx), int(ry), button, dpad]
	def __repr__(self) -> str:
		s = ""
		for item in self.array:
			s += f"{item:03}" + ","
		return s[:len(s)-1]

"""with hid.Device(contVID, contPID) as h:
	print(f'Device manufacturer: {h.manufacturer}')
	print(f'Product: {h.product}')
	print(f'Serial Number: {h.serial}')"""

#for device in hid.enumerate():
#    print(f"0x{device['vendor_id']:04x}:0x{device['product_id']:04x} {device['product_string']}")

controller = hid.Device(vid = contVID, pid = contPID)

print("Listening for IP")
#packets = sniff(iface=interface, filter="udp port 8888", count=1)
#ip = packets[0][IP].src
ip = "192.168.137.202"
print("Received", ip)

while True:
	binArray = controller.read(16)
	q = ControlArray(binArray[1], binArray[2], binArray[3], binArray[4], binArray[6], binArray[5])
	print(q)
	send(IP(dst=ip)/UDP(dport=port)/str(q))
	time.sleep(0.01)