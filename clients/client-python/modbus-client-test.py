from pymodbus.client import ModbusTcpClient
import time
import sys

# Check if sufficient arguments are provided (including script name)
if len(sys.argv) < 5:
    print(f"Usage: python {sys.argv[0]} <ip_address> <port> <times> <number_of_tests>")
    sys.exit(1)

# Function that read holding registers from a Modbus server
def read_holding_registers():
    start_time = time.time()

    # Read 10 registers x times.
    for _ in range(TIMES):
        response = client.read_holding_registers(address=0, count=10)
        if not response.isError():
            pass
        else:
            print(f"Reading error: {response}")

    end_time = time.time()
    elapsed_time = end_time - start_time
    return elapsed_time


HOST = sys.argv[1]
PORT = int(sys.argv[2])
TIMES = int(sys.argv[3])
NUMBER_OF_TESTS = int(sys.argv[4])

client = ModbusTcpClient(HOST, PORT)
client.connect()

AVARAGE = 0

for _ in range(NUMBER_OF_TESTS):
    CURRENT_READING_TIME = read_holding_registers()
    AVARAGE += CURRENT_READING_TIME
    print(f"Time for making {TIMES} readings: {CURRENT_READING_TIME*1000:.0f} ms")

print(f"Average time for making {TIMES} readings: {(AVARAGE/NUMBER_OF_TESTS)*1000:.0f} ms")

client.close()
