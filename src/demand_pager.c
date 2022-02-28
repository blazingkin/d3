// Data Demand Pager
// export persist_alloc, a malloc that uses file-backed memory to ensure
// that the allocations persist after reboot.
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#define GB (0x1000ull << 18)
// Map 48GB into the process. Hope nothing else is there...
#define BASE_ADDRESS ((void *) (48 * GB))

static bool initialized  = false;
static int  backing_file = 0;
void *d3_initialize(char *backing_store) {

    // Open the backing file, or create it if it doesn't exist
    backing_file = open(backing_store, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

    // We assume the system is using 4k pages
    assert(sysconf(_SC_PAGESIZE) == 0x1000);

    assert(mmap(BASE_ADDRESS, GB, PROT_READ|PROT_WRITE, MAP_FIXED, backing_file, 0) == BASE_ADDRESS);

    
    initialized = true;
    return BASE_ADDRESS;
}

void *persist_alloc(size_t size) {
    if (!initialized) {
        fprintf(stderr, "persist_alloc called before d3_initialize");
        assert(initialized);
    }


}

void d3_sync(void) {
    fsync(backing_file);
}