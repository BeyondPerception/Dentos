#ifndef DENTOS_PALLOC_H
#define DENTOS_PALLOC_H

void* next_page();
void* fetch_pages(unsigned int n);
void free_page();
void free_pages();
void palloc_init(multiboot_info_t* multiboot_info);

#endif //DENTOS_PALLOC_H
