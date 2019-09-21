#include <stdio.h>
#include <signal.h>

#include <libvortex/vortex.h>
#include <libvortex/datagram.h>




static volatile unsigned int running = 1;

void sighandler(const int sig) {
	if (sig == SIGINT) {
		running = 0;
	}
}

int
main(int argc, char *argv[], char *env[]) {
	struct vortex vtx;
	unsigned int r;

	if ((r = vortex_init(&vtx, NULL, sizeof(addr) - 1, 9999)) != VORTEX_SUCCESS) {
		fprintf(stderr, "failed to init: %u\n", r);
		return 1;
	} else {
		puts("initialized");
	}

	unsigned int len = 0;
	unsigned char buffer[4096];
	while (running) {
		r = vortex_recv_raw(&vtx, &len, buffer, sizeof(buffer));
		if (r > 0) {
			printf("recv failed: %u\n", r);
		} else {
			printf("r=%u recv=%u/%u\n", r, len, sizeof(struct fdo_datagram));
		}
	}

	puts("deinitializing...");

	if ((r = vortex_deinit(&vtx)) != VORTEX_SUCCESS) {
		fprintf(stderr, "deinit failed: %u\n", r);
		return 1;
	}
	puts("done");

	return 0;
}
