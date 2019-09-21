#ifndef __VORTEX_ABI_H
#define __VORTEX_ABI_H




#include <stddef.h>




#define VORTEX_SUCCESS			0
#define VORTEX_ERR_NULL			1
#define VORTEX_ERR_INVAL		2
#define VORTEX_ERR_CONV			3
#define VORTEX_ERR_ADDRINFO		4
#define VORTEX_ERR_CONN			5




struct vortex {
	int sfd;
};




#ifdef __cplusplus
extern "C" {
#endif

unsigned int vortex_init_ex(struct vortex *const vtx, const char *const address, const size_t address_length, const unsigned short int port, const char *const svc);
unsigned int vortex_init(struct vortex *const vtx, const char *const address, const size_t address_length, const unsigned short int port);
unsigned int vortex_deinit(struct vortex *const vtx);

unsigned int vortex_recv_raw(const struct vortex *const vtx, unsigned int *const length, void *const buffer, const size_t buffer_size);

#ifdef __cplusplus
}
#endif




#endif
