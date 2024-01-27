#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <modbus.h>
#include <sys/time.h>

static uint16_t tab_reg[10];
static struct timeval start, end;
static long mtime, seconds, useconds;
static modbus_t *ctx;

static unsigned int number_of_tests;
static unsigned int times;
static int port;

static unsigned int read_registers() {
    int rc;

    gettimeofday(&start, NULL);
    for (int i = 0; i < times; i++) {
        rc = modbus_read_registers(ctx, 0, 10, tab_reg);
        if (rc == -1) {
            fprintf(stderr, "Register read failed: %s\n", modbus_strerror(errno));
            modbus_close(ctx);
            modbus_free(ctx);
        }
    }
    gettimeofday(&end, NULL);
    
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

    return mtime;
}

static void init_modbus_ctx(const char* ip, int port) {
    // Create TCP context
    ctx = modbus_new_tcp(ip, port);
    if (ctx == NULL) {
        fprintf(stderr, "Can't create libmodbus ctx\n");
        exit(-1);
    }
}

int main(int argc, char *argv[]) {

    if (5 > argc) {
        fprintf(stderr, "Usage: %s <ip> <port> <times> <number_of_tests>\n", argv[0]);
        exit(-1);
    }

    port = atoi(argv[2]);
    times = atoi(argv[3]);
    number_of_tests = atoi(argv[4]);

    init_modbus_ctx(argv[1], port);

    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Can't connect: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    unsigned int cumulative_time = 0;
    unsigned int current_time = 0;

    for (unsigned int i = 0; i < number_of_tests; i++) {
        current_time = read_registers();
        cumulative_time += current_time;
        printf("Time for making %u, readings: %d ms\n", times, current_time); // Test for reading 10 registers 10000 times
    }

    modbus_close(ctx);
    modbus_free(ctx);

    printf("Average time for making %u, readings: %d ms\n", times, cumulative_time / number_of_tests);

    return 0;
}