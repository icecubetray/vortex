#include <stdio.h>
#include <signal.h>
#include <string.h>

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
	signal(SIGINT, sighandler);

	struct vortex vtx;
	unsigned int r;

	vortex_init(&vtx, VORTEX_FMT_AUTODETECT);

	if ((r = vortex_bind(&vtx, NULL, 0, 9999)) != VORTEX_SUCCESS) {
		fprintf(stderr, "failed to init: %u\n", r);
		return 1;
	} else {
		puts("initialized");
	}

	if ((r = vortex_set_recv_timeout(&vtx, 5000)) != VORTEX_SUCCESS) {
		fprintf(stderr, "failed to set timeout: %u\n", r);
		return 1;
	}

	unsigned int fmt;
	union fdo_datagram dg_test;
	while (running) {
		r = vortex_recv_datagram(&vtx, &fmt, &dg_test);

		if (r == VORTEX_ERR_TIMEOUT) {
			fputs("timed out waiting for data\n", stderr);
			continue;
		}

		if (r != VORTEX_SUCCESS) {
			perror("recvfrom()");
			break;
		}

		puts("");
		printf("format: %u\n", fmt);
		printf("isRaceOn: %i\n", dg_test.horizon.v1.isRaceOn);
		printf("timestampMs: %u\n", dg_test.horizon.v1.timestampMs);
		printf("engineRpm (max/idle/current): %f/%f/%f\n", dg_test.horizon.v1.engineMaxRpm, dg_test.horizon.v1.engineIdleRpm, dg_test.horizon.v1.engineCurrentRpm);
		printf("accel (x,y,z): %f,%f,%f\n", dg_test.horizon.v1.accelerationX, dg_test.horizon.v1.accelerationY, dg_test.horizon.v1.accelerationZ);
		printf("velocity (x,y,z): %f,%f,%f\n", dg_test.horizon.v1.velocityX, dg_test.horizon.v1.velocityY, dg_test.horizon.v1.velocityZ);
		printf("ang. velocity (x,y,z): %f,%f,%f\n", dg_test.horizon.v1.angularVelocityX, dg_test.horizon.v1.angularVelocityY, dg_test.horizon.v1.angularVelocityZ);
		printf("yaw/pitch/roll: %f/%f/%f\n", dg_test.horizon.v1.yaw, dg_test.horizon.v1.pitch, dg_test.horizon.v1.roll);
		printf("susp compr. (fl/fr/rl/rr): %f/%f/%f/%f\n", dg_test.horizon.v1.suspensionCompressionFrontLeft, dg_test.horizon.v1.suspensionCompressionFrontRight, dg_test.horizon.v1.suspensionCompressionRearLeft, dg_test.horizon.v1.suspensionCompressionRearRight);
		printf("pos (x,y,z): %f,%f,%f\n", dg_test.horizon.v2.positionX, dg_test.horizon.v2.positionY, dg_test.horizon.v2.positionZ);
		printf("speed: %f m/s (%f km/h)\n", dg_test.horizon.v2.speed, 0.0f);
		printf("fuel: %f\n", dg_test.horizon.v2.fuel);
		printf("distance: %f\n", dg_test.horizon.v2.distanceTraveled);
		printf("brake (foot,hand): %u/%u\n", dg_test.horizon.v2.brake, dg_test.horizon.v2.handBrake);
		printf("steer: %u\n", dg_test.horizon.v2.steer);
		printf("drivingLine: %u\n", dg_test.horizon.v2.drivingLine);
		printf("aiBrakeDiff: %u\n", dg_test.horizon.v2.aiBrakeDifference);
	}

	puts("deinitializing...");

	if ((r = vortex_deinit(&vtx)) != VORTEX_SUCCESS) {
		fprintf(stderr, "deinit failed: %u\n", r);
		return 1;
	}
	puts("done");

	return 0;
}
