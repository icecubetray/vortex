#include <stdio.h>

#include <libvortex/vortex.h>




int
main(int argc, char *argv[], char *env[]) {
	struct vortex vtx;
	unsigned int r;

	r = vortex_init(&vtx, NULL, 0, 9999);
	printf("r=%u\n", r);

	fgetc(stdin);

	r = vortex_deinit(&vtx);
	printf("r=%u\n", r);

	return 0;
}
