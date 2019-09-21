#include "./vortex.h"

#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>




unsigned int
vortex_init_ex(struct vortex *const vtx, const char *const address, const size_t address_length, const unsigned short int port, const char *const svc) {
	/* Sanity checks. */

	if (vtx == NULL) {
		return VORTEX_ERR_NULL;
	}

	if (address != NULL && address[address_length] != '\0') {
		return VORTEX_ERR_INVAL;
	}


	memset(vtx, 0, sizeof(*vtx));

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));

	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;


	char svcbuf[5 + 1];
	const char *const service = (svc ? svc : svcbuf);
	if (svc == NULL) {
		if (snprintf(svcbuf, sizeof(svcbuf) - 1, "%u", port) < 0) {
			perror("snprintf()");
			return VORTEX_ERR_CONV;
		}

		hints.ai_flags |= AI_NUMERICSERV;
	}


	struct addrinfo *ai_root = NULL;
	if (getaddrinfo(address, service, &hints, &ai_root) != 0) {
		perror("getaddrinfo()");
		return VORTEX_ERR_ADDRINFO;
	}


	int sfd = -1;
	struct addrinfo *ai_ptr = NULL;
	for (ai_ptr = ai_root; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next) {
		sfd = socket(ai_ptr->ai_family, ai_ptr->ai_socktype, ai_ptr->ai_protocol);
		if (sfd < 0) {
			perror("socket()");
			continue;
		}

		if (bind(sfd, ai_ptr->ai_addr, ai_ptr->ai_addrlen) != 0) {
			perror("bind()");
			close(sfd);
			continue;
		}

		break;
	}
	freeaddrinfo(ai_root);

	if (ai_ptr == NULL) {
		return VORTEX_ERR_CONN;
	}

	vtx->sfd = sfd;
	return VORTEX_SUCCESS;
}


unsigned int
vortex_init(struct vortex *const vtx, const char *const address, const size_t address_length, const unsigned short int port) {
	return vortex_init_ex(
		vtx,
		address, address_length,
		port, NULL
	);
}


unsigned int
vortex_deinit(struct vortex *const vtx) {
	if (vtx == NULL) {
		return VORTEX_ERR_NULL;
	}

	if (vtx->sfd < 0) {
		return VORTEX_ERR_INVAL;
	}

	if (close(vtx->sfd) != 0) {
		perror("close()");
		return VORTEX_ERR_CONN;
	}

	vtx->sfd = -1;
	return VORTEX_SUCCESS;
}
