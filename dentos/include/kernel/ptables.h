#ifndef DENTOS_PTABLES_H
#define DENTOS_PTABLES_H

unsigned int ptables_init();
int map_page(void* phys_addr, void* virt_addr);
int unmap_page(void* virt_addr);

#endif //DENTOS_PTABLES_H
