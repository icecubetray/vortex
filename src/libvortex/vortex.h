#ifndef __VORTEX_ABI_H
#define __VORTEX_ABI_H




#include <stddef.h>

#include "./datagram.h"




#define VORTEX_SUCCESS			0
#define VORTEX_ERR_NULL			1
#define VORTEX_ERR_INVAL		2
#define VORTEX_ERR_CONV			3
#define VORTEX_ERR_ADDRINFO		4
#define VORTEX_ERR_CONN			5
#define VORTEX_ERR_TIMEOUT		6

#define VORTEX_FMT_INVALID		~0U
#define VORTEX_FMT_AUTODETECT	0
#define VORTEX_FMT_MOTORSPORT	1
#define VORTEX_FMT_HORIZON		2




struct vortex {
	int sfd;
	unsigned int format;
};




#ifdef __cplusplus
extern "C" {
#endif

unsigned int vortex_init(struct vortex *const vtx, const unsigned int format);
unsigned int vortex_deinit(struct vortex *const vtx);

unsigned int vortex_set_recv_timeout(const struct vortex *const vtx, const unsigned int timeout_ms);

unsigned int vortex_bind_ex(struct vortex *const vtx, const char *const address, const size_t address_length, const unsigned short int port, const char *const svc);
unsigned int vortex_bind(struct vortex *const vtx, const char *const address, const size_t address_length, const unsigned short int port);

unsigned int vortex_recv_raw(const struct vortex *const vtx, unsigned int *const length, void *const buffer, const size_t buffer_size);
unsigned int vortex_recv_datagram(const struct vortex *const vtx, unsigned int *const format, union fdo_datagram *const datagram);

#ifdef __cplusplus
}
#endif




#endif
