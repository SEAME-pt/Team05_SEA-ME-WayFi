  GNU nano 7.2                               speed_can.py                                         
import can  # Python library for CAN communication

# Setup CAN interface
os.system("sudo ip link set can0 up type can bitrate 500000")

# Create CAN bus instance
bus = can.interface.Bus(channel='can0', bustype='socketcan')

print("Listening for CAN messages...")

try:
    while True:
        # Read CAN message
        message = bus.recv()  # Blocking call to receive a message
        if message.arbitration_id == 0x100:  # Check for specific CAN ID
            # Decode the speed (2 bytes)
            speed_scaled = (message.data[0] << 8) | message.data[1]  # Combine high and low bytes
            speed_kmh = speed_scaled / 100.0  # Convert back to float (divide by 100)
            print(f"Speed: {speed_kmh:.2f} m/s")  # Print with 2 decimal places
except KeyboardInterrupt:
    print("\nExiting...")
    os.system("sudo ip link set can0 down")

