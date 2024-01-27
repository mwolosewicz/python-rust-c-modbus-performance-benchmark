#include <errno.h>
#include <getopt.h>
#include <modbus.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static char* address = "127.0.0.1";
static int port = 1502;
static int stop = 0;
static int server_socket;
static modbus_t *ctx;

static uint32_t response_timeout_s = 1;
static uint32_t response_timeout_us = 0;

void print_usage(char *program_name) {
    printf("Usage: %s [-i ip_address] [-p port] [--help]\n", program_name);
    printf("  -i, --ip      IP address to use. Default is '127.0.0.1'.\n");
    printf("  -p, --port    Port number to use. Default is '8080'.\n");
    printf("  --help        Display this help and exit.\n");
}

void parse_arguments(int argc, char *argv[]) {

    struct option long_options[] = {
        {"ip", required_argument, 0, 'i'},
        {"port", required_argument, 0, 'p'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "i:p:h", long_options, NULL)) != -1) {
        switch (opt) {
            case 'i':
                address = optarg;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'h':
                print_usage(argv[0]);
                exit(EXIT_SUCCESS);
            default:
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }
}

void signal_handler(int signum) {
    printf("Closing server\n");

    stop = 1;
    if (ctx != NULL) {
        modbus_close(ctx);
        modbus_free(ctx);
    }
    exit(signum);
}

int main(int argc, char *argv[]) {

    modbus_mapping_t *mb_mapping;
    int s = -1;

    parse_arguments(argc, argv);

    printf("Listening on address: %s port %d\n", address, port);
    printf("Please use --help for more information.\n");

    signal(SIGINT, signal_handler);

    // Create TCP context
    ctx = modbus_new_tcp(address, port);
    if (ctx == NULL) {
        fprintf(stderr, "Can't create context libmodbus: %s\n", modbus_strerror(errno));
        return -1;
    }

    // Registers mapping
    mb_mapping = modbus_mapping_new(0, 0, 10, 0);
    if (mb_mapping == NULL) {
        fprintf(stderr, "Can't map: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }

    // Listening
    s = modbus_tcp_listen(ctx, 1);
    if (s == -1) {
        fprintf(stderr, "Can't listen: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        modbus_mapping_free(mb_mapping);
        return -1;
    }

    int server_socket = modbus_get_socket(ctx);

    fd_set refset;
    FD_ZERO(&refset);
    FD_SET(server_socket, &refset);

    // Set TRUE if you wanna see modbus debug info.
    modbus_set_debug(ctx, FALSE);

    while(1) {
        int new_ctx;

        // Accept connections
        new_ctx = modbus_tcp_accept(ctx, &s);
        if (new_ctx == -1) {
            fprintf(stderr, "Can't accept client: %s\n", modbus_strerror(errno));
            continue;
        }

        // Process request
        for(;;) {
            uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
            int rc = modbus_receive(ctx, query);
            if (rc > 0) {
                // Sent the answer
                modbus_reply(ctx, query, rc, mb_mapping);
            } else if (rc == -1) {
                // Connection closed by peer
                break;
            }
        }

        // Close the connection
        modbus_close(ctx);
    }

    printf("Closing server\n");

    // Cleaning
    modbus_close(ctx);
    modbus_free(ctx);
    modbus_mapping_free(mb_mapping);

    return 0;
}