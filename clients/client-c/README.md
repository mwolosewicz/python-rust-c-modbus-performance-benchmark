To run this client:

1. Build it using `make`.
2. Call: `c_modbus_client 127.0.0.1 1502 1000 10`

- `127.0.0.1` address, you can change if server is running on other machine
- `1502` port, you can change if server is listening on other port
- `1000` how many times 10 registers will be read in single measurement
- `10` how many measurements should be done to calculate avarage value.