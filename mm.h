/* $begin mallocinterface */
int mm_init(void);
void *mm_malloc(size_t size);
void mm_free(void *bp);
/* $end mallocinterface */

int mm_read_num_headers(void);
void mm_read_headers(unsigned int *buf);
int mm_read_size(unsigned int header);
int mm_read_alloc(unsigned int header);
void mm_checkheap(int verbose);
void *mm_realloc(void *ptr, size_t size);

/* Unused. Just to keep us compatible with the 15-213 malloc driver */
typedef struct
{
    char *team;
    char *name1, *email1;
    char *name2, *email2;
} team_t;

extern team_t team;
