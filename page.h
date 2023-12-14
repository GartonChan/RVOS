#ifndef __PAGE_H__
#define __PAGE_H__

#include "types.h"

/* Following global vars are defined in mem.S */
extern uint32_t TEXT_START;
extern uint32_t TEXT_END;
extern uint32_t DATA_START;
extern uint32_t DATA_END;
extern uint32_t RODATA_START;
extern uint32_t RODATA_END;
extern uint32_t BSS_START;
extern uint32_t BSS_END;
extern uint32_t HEAP_START;
extern uint32_t HEAP_SIZE;

/* base configuration of pages */
#define PAGE_SIZE 4096  /* (2 << 12) */
#define PAGE_ORDER 12
#define NUM_RESERVED_PAGES 8

/* bits in the flag */
#define PAGE_TAKEN (uint8_t)(1 << 0)  /* bit-0 indicates the allocated page */
#define PAGE_LAST (uint8_t)(1 << 1)   /* bit-1 indicates the last page */

/* Page Descriptor */
struct page {
    uint8_t flags;
};

void page_init(void);
void *page_alloc(int npages);
void page_free(void *p);
void page_test(void);

#endif  /* __PAGE_H__ */
