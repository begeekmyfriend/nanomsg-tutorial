#include "common.h"
#include <nanomsg/bus.h>

#define NODE0_SOCKET_ADDR "ipc:///tmp/node0.ipc"
#define NODE1_SOCKET_ADDR "ipc:///tmp/node1.ipc"
#define NODE2_SOCKET_ADDR "ipc:///tmp/node2.ipc"
#define NODE3_SOCKET_ADDR "ipc:///tmp/node3.ipc"

int node0(void)
{
        int sock = nn_socket(AF_SP, NN_BUS);

        assert(sock >= 0);
        assert(nn_bind(sock, NODE0_SOCKET_ADDR) >= 0);
        sleep(1); /* wait for connections */

        assert(nn_connect(sock, NODE1_SOCKET_ADDR) >= 0);
        assert(nn_connect(sock, NODE2_SOCKET_ADDR) >= 0);
        sleep(1); /* wait for connections */

        return sock;
}

int node1(void)
{
        int sock = nn_socket(AF_SP, NN_BUS);

        assert(sock >= 0);
        assert(nn_bind(sock, NODE1_SOCKET_ADDR) >= 0);
        sleep(1); /* wait for connections */

        assert(nn_connect(sock, NODE2_SOCKET_ADDR) >= 0);
        assert(nn_connect(sock, NODE3_SOCKET_ADDR) >= 0);
        sleep(1); /* wait for connections */

        return sock;
}

int node2(void)
{
        int sock = nn_socket(AF_SP, NN_BUS);

        assert(sock >= 0);
        assert(nn_bind(sock, NODE2_SOCKET_ADDR) >= 0);
        sleep(1); /* wait for connections */

        assert(nn_connect(sock, NODE3_SOCKET_ADDR) >= 0);
        sleep(1); /* wait for connections */

        return sock;
}

int node3(void)
{
        int sock = nn_socket(AF_SP, NN_BUS);

        assert(sock >= 0);
        assert(nn_bind(sock, NODE3_SOCKET_ADDR) >= 0);
        sleep(1); /* wait for connections */

        assert(nn_connect(sock, NODE0_SOCKET_ADDR) >= 0);
        sleep(1); /* wait for connections */

        return sock;
}

int bus_on(int sock, const char *name)
{
        int to = 100;
        assert(nn_setsockopt(sock, NN_SOL_SOCKET, NN_RCVTIMEO, &to, sizeof(to)) >= 0);

        /* SEND */
        int sz_n = strlen(name) + 1;
        printf("%s: SENDING '%s' ONTO BUS\n", name, name);
        int send = nn_send(sock, name, sz_n, 0);
        assert (send == sz_n);

        while (1) {
                /* RECV */
                char *buf = NULL;
                int recv = nn_recv(sock, &buf, NN_MSG, 0);
                if (recv >= 0) {
                        printf("%s: RECEIVED '%s' FROM BUS\n", name, buf);
                        nn_freemsg(buf);
                }
        }

        return nn_shutdown(sock, 0);
}

int node(const char *name)
{
        int sock;

        if (!strcmp(name, "node0")) {
                sock = node0();
        } else if (!strcmp(name, "node1")) {
                sock = node1();
        } else if (!strcmp(name, "node2")) {
                sock = node2();
        } else if (!strcmp(name, "node3")) {
                sock = node3();
        } else {
                return -1;
        }

        return bus_on(sock, name);
}

int main(int argc, char **argv)
{
        if (argc == 2) {
                return node(argv[1]);
        } else {
                fprintf (stderr, "Usage: bus <NODE_NAME> ...\n");
                return 1;
        }
}
