/*
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Copyright (c) 2018 Marcelo Tosatti <mtosatti@redhat.com>

 * Open a socket, and enable timestamping on it.
 *
 * This is to avoid Chrony from changing timestamping
 * user count from 0->1 and vice-versa, causing
 * static key enable/disable IPIs.
 *
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <linux/errqueue.h>
#include <linux/ethtool.h>
#include <linux/net_tstamp.h>
#include <linux/sockios.h>
#include <net/if.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

int main (void)
{
	int option = SOF_TIMESTAMPING_OPT_TX_SWHW;
	int sock_fd;
	int ret;
        int pid_fd;
        pid_t pid;
        char buf[50];

	/* open a datagram socket */
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd < 0) {
		printf("Error opening the socket\n");
		return 0;
	}

	/* set the timestamping option
	 * this is to trigger the IPIs that notify all cpus of the change
	 */
	if (setsockopt(sock_fd, SOL_SOCKET, SO_TIMESTAMP, &option, sizeof (option)) < 0) {
		printf("Could not enable timestamping option %x", (unsigned int)option);
		close(sock_fd);
		return 0;
	}

	/* dameonize our task */
	ret = daemon(0, 0);

	if (ret == -1) {
		perror("daemon");
		exit(0);
	}

	/* write our pid file to make systemd happy */
        pid_fd = open("/run/rt-entsk.pid", O_RDWR|O_CREAT|O_TRUNC, 0644);
	if (pid_fd < 0) {
		perror("open of /run/rt-entsk.pid failed:");
		exit(errno);
	}
        pid = getpid();

        sprintf(buf, "%d\n", pid);

        ret = write(pid_fd, buf, strlen(buf));
        if (ret == -1) {
                perror("write");
                exit(1);
        }
	close(pid_fd);

	/* now just pause forever */
	while (1) {
		pause();
	}
}
