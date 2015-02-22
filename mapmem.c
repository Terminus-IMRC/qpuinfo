#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>
#include "pagesize.h"

#ifndef DEVMEM
#define DEVMEM "/dev/mem"
#endif /* DEVMEM */

#ifndef V3D_LENGTH
#define V3D_LENGTH ((0x00f20-0x00000+0x4))
#endif /* V3D_LENGTH */

static uint32_t *p_orig;

uint32_t* mapmem();
void unmapmem();

uint32_t* mapmem(const off_t V3D_OFFSET)
{
	int fd;
	long pagesize;
	uint32_t *p;
	off_t offset_from_v3d;

	if((fd=open(DEVMEM, O_RDWR|O_SYNC/*|O_DIRECT*/))==-1){
		fprintf(stderr, "%s:%d: error: open: %s: %s\n", __FILE__, __LINE__, DEVMEM, strerror(errno));
		exit(EXIT_FAILURE);
	}

	pagesize=get_pagesize();

#ifdef DEBUG
	printf("%s:%d: pagesize: %ld\n", __FILE__, __LINE__, pagesize);
#endif /* DEBUG */

	offset_from_v3d=-(V3D_OFFSET%pagesize);

#ifdef DEBUG
	printf("%s:%d: offset_from_v3d: %ld\n", __FILE__, __LINE__, offset_from_v3d);
#endif /* DEBUG */

	if((p=(uint32_t*)mmap(0, V3D_LENGTH-offset_from_v3d, PROT_READ|PROT_WRITE, MAP_SHARED, fd, V3D_OFFSET+offset_from_v3d))==MAP_FAILED){
		fprintf(stderr, "%s:%d: error: mmap: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(EXIT_FAILURE);
	}
	p_orig=p;

	p+=-offset_from_v3d;

	return p;
}

void unmapmem()
{
	int r;

	r=munmap(p_orig, V3D_LENGTH);
	if(r==-1){
		fprintf(stderr, "%s:%d: error: munmap: %s\n", __FILE__, __LINE__, strerror(errno));
		exit(EXIT_FAILURE);
	}

	return;
}
