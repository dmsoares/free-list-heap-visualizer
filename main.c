#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./csapp.h"
#include "./mm.h"

char *sprint_bookend(size_t size, int alloc);
char *sprint_payload(size_t size);
char *sprint_block(size_t size, int alloc);
char *sprint_prologue();
char *sprint_epilogue();
void print_heap();

void sig_handler()
{
    printf("\b\b\nThank you, bye\n");
    exit(0);
}

void main()
{
    Signal(2, sig_handler);

    mm_init();

    int ptrName = 0;

    while (1)
    {
        print_heap();

        char line[256];
        int i = 0;

        printf("> ");

        Fgets(line, 256, stdin);

        if (line[0] == 'm')
        {
            char verb[7];
            strncpy(verb, line, 6);
            verb[6] = '\0';

            if (!strcmp(verb, "malloc"))
            {
                char noun[262];
                strcpy(noun, line + 7);

                char *stopstr;
                printf("p%d: %p\n", ptrName++, mm_malloc((int)strtol(noun, &stopstr, 10)));
            }
        }

        if (line[0] == 'f')
        {
            char verb[5];
            strncpy(verb, line, 4);

            if (!strcmp(verb, "free"))
            {
                char noun[262];
                strcpy(noun, line + 7);

                char *stopstr;
                mm_free((void *)strtoul(noun, &stopstr, 16));
            }
        }

        if (line[0] == 'h')
        {
            char verb[5];
            strncpy(verb, line, 4);

            if (!strcmp(verb, "help"))
            {
                printf("Type one of two possible commands:\n- \"malloc <bytes>\"\n- \"free <block address>\"\nOtherwise, type \"help\" to see this message again :)\n");
            }
        }
    }
}

char *sprint_bookend(size_t size, int alloc)
{
    char *bookend = Malloc(1 << 10);
    sprintf(bookend, "|%d:%d|", size, alloc);
    return bookend;
}

char *sprint_payload(size_t size)
{
    char *payload = Malloc(16);

    sprintf(payload, size ? "---%d---" : "", size);

    return payload;
}

char *sprint_block(size_t size, int alloc)
{
    char *str = Malloc(1 << 10);
    sprintf(str, "%s%s%s", sprint_bookend(size, alloc), size >= 8 ? sprint_payload(size - 8) : "", sprint_bookend(size, alloc));
    return str;
}

char *sprint_prologue()
{
    return sprint_block(8, 1);
}

char *sprint_epilogue()
{
    return sprint_bookend(0, 1);
}

void print_heap()
{
    int num_headers = mm_read_num_headers();

    unsigned int *headers = Malloc(num_headers * sizeof(char *));

    mm_read_headers(headers);

    printf("\n");

    for (int i = 0; i < num_headers - 1; i++)
    {
        char *block = sprint_block(mm_read_size(headers[i]), mm_read_alloc(headers[i]));
        printf("%s", block);
    }

    printf("%s\n\n", sprint_epilogue());

    free(headers);
}