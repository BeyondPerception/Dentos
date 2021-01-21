#ifndef DENTOS_PTABLES_H
#define DENTOS_PTABLES_H

#include <kernel/multiboot.h>

unsigned int ptables_init(multiboot_info_t* multiboot_info);
int map_page(void* phys_addr, void* virt_addr);
int unmap_page(void* virt_addr);

#endif //DENTOS_PTABLES_H
