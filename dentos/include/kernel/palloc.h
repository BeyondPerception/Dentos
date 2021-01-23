#ifndef DENTOS_PALLOC_H
#define DENTOS_PALLOC_H

void* next_page();
void free_page();
void palloc_init(multiboot_info_t* multiboot_info);

#endif //DENTOS_PALLOC_H
