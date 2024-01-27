CC = gcc
CFLAGS = -I/usr/include/modbus
LIBS = -lmodbus

TARGET = c_modbus_server
SRCS = c_modbus_server.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)
