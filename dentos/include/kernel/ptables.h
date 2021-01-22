#ifndef DENTOS_PTABLES_H
#define DENTOS_PTABLES_H

#define PTABLES_WRITABLE (1 << 1)
#define PTABLES_USER (1 << 2)
#define PTABLES_WRITETHROUGH_CACHE (1 << 3)
#define PTABLES_DISABLE_CACHE (1 << 4)
#define PTABLES_GLOBAL (1 << 8)

unsigned int ptables_init();
int map_page(void* phys_addr, void* virt_addr, int flags);
int unmap_page(void* virt_addr);

#endif //DENTOS_PTABLES_H
