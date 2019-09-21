#include "./vortex.h"

#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>




unsigned int
vortex_init(struct vortex *const vtx, const unsigned int format) {
	/* Sanity checks. */

	if (vtx == NULL) {
		return VORTEX_ERR_NULL;
	}


	/* Zero out the struct. */
	memset(vtx, 0, sizeof(*vtx));

	/* Specify format. */
	vtx->format = format;


	return VORTEX_SUCCESS;
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




unsigned int
vortex_set_recv_timeout(const struct vortex *const vtx, const unsigned int timeout_ms) {
	if (vtx == NULL) {
		return VORTEX_ERR_NULL;
	}

	if (vtx->sfd < 0) {
		return VORTEX_ERR_INVAL;
	}


	struct timeval timeout;
	memset(&timeout, 0, sizeof(timeout));

	unsigned int timeout_total = timeout_ms;

	if (timeout_total > 1000) {
		timeout.tv_sec = (timeout_total / 1000);
		timeout_total -= (timeout.tv_sec * 1000);
	}

	timeout.tv_usec = (timeout_total * 1000);

    if (setsockopt(vtx->sfd, SOL_SOCKET, SO_RCVTIMEO, (const void *)&timeout, sizeof(timeout)) != 0) {
		perror("setsockopt()");
		return VORTEX_ERR_CONN;
	}


	return VORTEX_SUCCESS;
}




unsigned int
vortex_bind_ex(struct vortex *const vtx, const char *const address, const size_t address_length, const unsigned short int port, const char *const svc) {
	/* Sanity checks. */

	if (vtx == NULL) {
		return VORTEX_ERR_NULL;
	}

	if (address != NULL && address[address_length] != '\0') {
		fputs("not null terminated\n", stderr);
		return VORTEX_ERR_INVAL;
	}


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
vortex_bind(struct vortex *const vtx, const char *const address, const size_t address_length, const unsigned short int port) {
	return vortex_bind_ex(
		vtx,
		address, address_length,
		port, NULL
	);
}




unsigned int
vortex_recv_raw(const struct vortex *const vtx, unsigned int *const length, void *const buffer, const size_t buffer_size) {
	if (vtx == NULL) {
		return VORTEX_ERR_NULL;
	}

	if (vtx->sfd < 0) {
		return VORTEX_ERR_INVAL;
	}

	struct sockaddr sender;
	socklen_t sender_len = sizeof(sender);
	ssize_t len = recvfrom(vtx->sfd, buffer, buffer_size, 0, &sender, &sender_len);
	if (len < 1) {
		if (errno == EAGAIN) {
			return VORTEX_ERR_TIMEOUT;
		} else {
			return VORTEX_ERR_CONN;
		}
	}

	if (length != NULL) {
		*length = len;
	}

	return VORTEX_SUCCESS;
}


unsigned int
vortex_recv_datagram(const struct vortex *const vtx, unsigned int *const format, union fdo_datagram *const datagram) {
	unsigned int len, fmt = vtx->format, off;
	unsigned char buffer[512];

	unsigned int r;
	if ((r = vortex_recv_raw(vtx, &len, buffer, sizeof(buffer))) != VORTEX_SUCCESS) {
		return r;
	}

	if (fmt == VORTEX_FMT_AUTODETECT) {
		switch (len) {
			case sizeof(datagram->motorsport):
				fmt = VORTEX_FMT_MOTORSPORT;
				break;
			case sizeof(datagram->horizon):
				fmt = VORTEX_FMT_HORIZON;
				break;
			default:
				fmt = VORTEX_FMT_INVALID;
				break;
		}
	}

	if (format != NULL) {
		*format = fmt;
	}

	switch (fmt) {
		case VORTEX_FMT_MOTORSPORT:
			off = offsetof(union fdo_datagram, motorsport);
			break;
		case VORTEX_FMT_HORIZON:
			off = offsetof(union fdo_datagram, horizon);
			break;
		default:
			return VORTEX_FMT_INVALID;
	}

	memcpy(
		&((char *)datagram)[off],
		buffer,
		len
	);

	return VORTEX_SUCCESS;
}
