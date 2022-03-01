#include <stddef.h>
#include <stdbool.h>
void *d3_initialize(char *backing_store);
void *persist_alloc(size_t size);
void  d3_sync(void);
bool  d3_any_allocated(void);