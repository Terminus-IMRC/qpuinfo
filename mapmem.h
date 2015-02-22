#ifndef __MAPMEM_H_INCLUDED__
#define __MAPMEM_H_INCLUDED__

#include <stdint.h>

#ifndef BCM2835_V3D_OFFSET
#define BCM2835_V3D_OFFSET 0x20c00000
#endif /* BCM2835_V3D_OFFSET */

#ifndef BCM2836_V3D_OFFSET
#define BCM2836_V3D_OFFSET (0x20c00000 + 0x1f000000)
#endif /* BCM2836_V3D_OFFSET */

	uint32_t* mapmem(const off_t V3D_OFFSET);
	void unmapmem();

#endif /* __MAPMEM_H_INCLUDED__ */
