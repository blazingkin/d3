#include <stddef.h>
void *d3_initialize(char *backing_store);
void *persist_alloc(size_t size);
void  d3_sync(void);