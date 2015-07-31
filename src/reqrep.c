#include "common.h"
#include <nanomsg/reqrep.h>

#define NODE0 "node0"
#define NODE1 "node1"
#define DATE "DATE"
#define SOCKET_ADDR "ipc:///tmp/reqrep.ipc"

char *date(void)
{
        time_t raw = time(&raw);
        struct tm *info = localtime(&raw);
        char *text = asctime(info);

        text[strlen(text) - 1] = '\0';

        return text;
}

int node0(const char *url)
{
        int sz_date = strlen(DATE) + 1;
        int sock = nn_socket(AF_SP, NN_REP);

        assert(sock >= 0);
        assert(nn_bind(sock, url) >= 0);

        while (1) {
                char *buf = NULL;
                int bytes = nn_recv(sock, &buf, NN_MSG, 0);

                assert(bytes >= 0);
                if (strncmp(DATE, buf, sz_date) == 0) {
                        printf("NODE0: RECEIVED DATE REQUEST\n");
                        char *d = date();
                        int sz_d = strlen(d) + 1;
                        printf("NODE0: SENDING DATE %s\n", d);
                        bytes = nn_send(sock, d, sz_d, 0);
                        assert(bytes == sz_d);
                }
                nn_freemsg (buf);
        }

        return nn_shutdown (sock, 0);
}

int node1(const char *url)
{
        int sz_date = strlen(DATE) + 1;
        char *buf = NULL;
        int bytes = -1;
        int sock = nn_socket(AF_SP, NN_REQ);

        assert(sock >= 0);
        assert(nn_connect(sock, url) >= 0);

        /* Send */
        printf("NODE1: SENDING DATE REQUEST %s\n", DATE);
        bytes = nn_send (sock, DATE, sz_date, 0);
        assert(bytes == sz_date);

        /* Receive */
        bytes = nn_recv (sock, &buf, NN_MSG, 0);
        assert(bytes >= 0);
        printf("NODE1: RECEIVED DATE %s\n", buf);
        nn_freemsg(buf);

        return nn_shutdown (sock, 0);
}

int main (int argc, char **argv)
{
        if (argc == 2 && strncmp(NODE0, argv[1], strlen(NODE0)) == 0) {
                return node0(SOCKET_ADDR);
        } else if (argc == 2 && strncmp(NODE1, argv[1], strlen(NODE1)) == 0) {
                return node1(SOCKET_ADDR);
        } else {
                fprintf (stderr, "Usage: reqrep %s|%s <ARG> ...\n", NODE0, NODE1);
                return 1;
        }
}
