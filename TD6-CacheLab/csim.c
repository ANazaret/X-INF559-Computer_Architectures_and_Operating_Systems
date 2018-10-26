// Achille Nazaret: achille.nazaret
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>

#include "cachelab.h"

typedef long long WORD;

typedef struct CacheBlock {
    int isValid;
    WORD tag;
    int time;
} CacheBlock;


/* Globals set by command line args */
int verbosity = 0; /* print trace if set */
int s = 0;         /* set index bits */
int b = 0;         /* block offset bits */
int E = 0;         /* associativity */

int t = 0; // Tag size
char *trace_file = NULL;
CacheBlock *cache;

int hit_count = 0;
int miss_count = 0;
int eviction_count = 0;

int current_time = 0;


int hit = 0;
int miss = 0;
int eviction = 0;

//int get_b_bits(int )

/*
 * printUsage - Print usage info
 */
void printUsage(char *argv[]) {
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\nExamples:\n");
    printf("  linux>  %s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  %s -v -s 8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}


WORD getSetIndex(WORD address) {
    return ( (address >> b) & ((1 << s) - 1));
}

WORD getTag(WORD address) {
    return address >> (s + b);
}



/*
 * “L” a data load,
 * “S” a data store
 * “M” a data modify (i.e., a data load followed by a data store).
 *
 * But here S acts as a L and M as L L
 */

/*void print_binary(WORD number)
{
    if (number) {
        print_binary(number >> 1);
        putc((number & 1) ? '1' : '0', stdout);
    }
}*/

void process_L(WORD address) {
    // Check if in the cache
    WORD set = getSetIndex(address);
    WORD tag = getTag(address);


    int oldest_time = current_time+10;

    // Iterate over set and match tag
    CacheBlock *cacheBlock = cache + (set * E);
    CacheBlock *oldest_block = cacheBlock;

    for (WORD i = 0; i < E; i++, cacheBlock++) {
        if (!cacheBlock->isValid) {
            //oldest_time = -1;
            oldest_block = cacheBlock;
            break;
        }

        if (cacheBlock->tag == tag) {
            hit += 1;
            // Update block time
            cacheBlock->time = current_time;
            break;
        } else {
            if (cacheBlock->time < oldest_time) {
                oldest_time = cacheBlock->time;
                oldest_block = cacheBlock;
            }
        }
    }
//    printf("\n%d Set: %lld\n",current_time, set);
//    print_binary(address);
//    printf("\n");
//    print_binary(tag);
//    printf("\n");


    if (!hit) {
        miss = 1;
        if (oldest_block->isValid)
            eviction = 1;
        else
            oldest_block->isValid = 1;

        oldest_block->tag = tag;
        oldest_block->time = current_time;
    }
}

void print_state(){
    CacheBlock *cacheBlock = cache;

    for (WORD i = 0; i < 1<<s; i++) {
        int valids = 0;
        for (WORD j = 0; j < E; j++, cacheBlock++) {
            if (cacheBlock->isValid)
                valids++;

        }
        printf("Set %d: %d/%d\n", (int) i, valids, E);
    }
}

int process_line(char *line) {
    char operation;
    WORD address;
    int size;

    if (line[0] == 'I')
        return 0;

    hit = eviction = miss = 0;

    sscanf(line, "%c %llx, %d", &operation, &address, &size);

    switch (operation) {
        case 'L':
            process_L(address);
            break;
        case 'M':
            process_L(address);
            process_L(address);
            break;
        case 'S':
            process_L(address);
            break;
        default:
            break;
    }

    hit_count += hit;
    miss_count += miss;
    eviction_count += eviction;
    current_time ++;

    //print_state();

    if (verbosity){
        printf("%c %llx,%d ", operation, address, size);
        if (miss)
            printf("miss ");
        if (eviction)
            printf("eviction ");
        if (hit == 1)
            printf("hit ");
        if (hit == 2)
            printf("hit hit ");
        printf("\n");
    }

    return 0;
}

char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s));
}

/*
 * main - Main routine 
 */
int main(int argc, char *argv[]) {
    char c;

    while ((c = getopt(argc, argv, "s:E:b:t:vh")) != -1) {
        switch (c) {
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                trace_file = optarg;
                break;
            case 'v':
                verbosity = 1;
                break;
            case 'h':
                printUsage(argv);
                exit(0);
            default:
                printUsage(argv);
                exit(1);
        }
    }

    /* Make sure that all required command line args were specified */
    if (s == 0 || E == 0 || b == 0 || trace_file == NULL) {
        printf("%s: Missing required command line argument\n", argv[0]);
        printUsage(argv);
        exit(1);
    }

    FILE *pFILE = fopen(trace_file, "r");

    char buff[256];
    cache = malloc(sizeof(CacheBlock) * (E << s));
    memset(cache, 0, sizeof(CacheBlock) * (E << s));


    while (fgets(buff, 256, pFILE)) {
        process_line(trim(buff));
    }

    printSummary(hit_count, miss_count, eviction_count);

    free(cache);
    return 0;
}

