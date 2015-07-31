#include "common.h"
#include <nanomsg/pair.h>

#define NODE0 "node0"
#define NODE1 "node1"
#define SOCKET_ADDR "ipc:///tmp/pair.ipc"

int send_name(int sock, const char *name)
{
        printf("%s: SENDING \"%s\"\n", name, name);
        int sz_n = strlen(name) + 1;
        return nn_send(sock, name, sz_n, 0);
}

int recv_name(int sock, const char *name)
{
        char *buf = NULL;
        int result = nn_recv(sock, &buf, NN_MSG, 0);
        if (result > 0)
        {
                printf("%s: RECEIVED \"%s\"\n", name, buf);
                nn_freemsg(buf);
        }
        return result;
}

int send_recv(int sock, const char *name)
{
        int to = 100;
        assert(nn_setsockopt (sock, NN_SOL_SOCKET, NN_RCVTIMEO, &to, sizeof (to)) >= 0);
        while(1)
        {
                recv_name(sock, name);
                sleep(1);
                send_name(sock, name);
        }
}

int node0(const char *url)
{
        int sock = nn_socket(AF_SP, NN_PAIR);
        assert(sock >= 0);
        assert(nn_bind (sock, url) >= 0);
        send_recv(sock, NODE0);
        return nn_shutdown (sock, 0);
}

int node1(const char *url)
{
        int sock = nn_socket(AF_SP, NN_PAIR);
        assert(sock >= 0);
        assert(nn_connect(sock, url) >= 0);
        send_recv(sock, NODE1);
        return nn_shutdown (sock, 0);
}

int main(int argc, char **argv)
{
        if (argc == 2 && strncmp(NODE0, argv[1], strlen(NODE0)) == 0) {
                return node0(SOCKET_ADDR);
        } else if (argc == 2 && strncmp (NODE1, argv[1], strlen (NODE1)) == 0) {
                return node1(SOCKET_ADDR);
        } else {
                fprintf (stderr, "Usage: pair %s|%s <ARG> ...\n", NODE0, NODE1);
                return 1;
        }
}
