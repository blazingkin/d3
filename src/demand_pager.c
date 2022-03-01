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
#include <stdint.h>

#define GB (0x1000ull << 18)
// Map 48GB into the process. Hope nothing else is there...
#define BASE_ADDRESS ((void *) (48 * GB))

static bool   initialized  = false;
static int    backing_file = 0;
static size_t extents      = 0;
void *d3_initialize(char *backing_store) {

    // Open the backing file, or create it if it doesn't exist
    backing_file = open(backing_store, O_CREAT|O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (backing_file <= 0) {
        perror("backing file");
        assert(backing_file > 0);
    }
    // We assume the system is using 4k pages
    assert(sysconf(_SC_PAGESIZE) == 0x1000);

    struct stat the_stat = {0};
    assert(fstat(backing_file, &the_stat) == 0);
    extents = the_stat.st_size;

    if (mmap(BASE_ADDRESS, GB, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, backing_file, 0) != BASE_ADDRESS) {
        perror("MMAP");
        assert(false);
    }

    
    initialized = true;
    return BASE_ADDRESS;
}

void *persist_alloc(size_t size) {
    if (!initialized) {
        fprintf(stderr, "persist_alloc called before d3_initialize");
        assert(initialized);
    }

    void *result =  (void *) (BASE_ADDRESS + extents);
    extents += size;
    assert(ftruncate(backing_file, extents) == 0);
    return result;
}

bool d3_any_allocated(void) {
    if (!initialized) {
        fprintf(stderr, "persist_alloc called before d3_initialize");
        assert(initialized);
    }

    return extents != 0;
}

void d3_sync(void) {
    fsync(backing_file);
}