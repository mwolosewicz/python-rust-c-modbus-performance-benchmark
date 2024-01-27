CC = gcc
CFLAGS = -I/usr/include/modbus
LIBS = -lmodbus

TARGET = c_modbus_client
SRCS = c_modbus_client.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)
