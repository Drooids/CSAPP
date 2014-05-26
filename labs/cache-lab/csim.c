#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#define ToStderr(format, ...) fprintf(stderr, format, __VA_ARGS__)
typedef struct {
  int32_t hits;
  int32_t misses;
  int32_t evictions;
} Stats;

typedef struct {
  uint32_t S;
  uint32_t E;
  uint64_t time_stamp;
} LRUCachePara;

typedef struct {
  bool valid_bit;
  uint32_t tag_bit;
  uint64_t time_stamp;
} LRUCacheLine;

typedef LRUCacheLine LRUCache;

typedef struct {
  bool h;
  bool v;
  uint32_t s;
  uint32_t E;
  uint32_t b;
  const char* trace_file;
} Args;

typedef struct {
  char op;
  uint64_t address;
  uint64_t size;
} MemoryOperation;

extern char *optarg;
extern int optind;

const char* help_str =\
"Usage: ./csim-ref [-hv] -s <num> -E <num> -b <num> -t <file>\n"
"Options:\n"
"  -h         Print this help message.\n"
"  -v         Optional verbose flag.\n"
"  -s <num>   Number of set index bits.\n"
"  -E <num>   Number of lines per set.\n"
"  -b <num>   Number of block offset bits.\n"
"  -t <file>  Trace file.\n"
"\n"
"Examples:\n"
"  linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n"
"  linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace\n";

void ArgsToString(const Args* args) {
  if (args->h) printf("-h\n");
  if (args->v) printf("-v\n");
  printf("-s %d\n", args->s);
  printf("-E %d\n", args->E);
  printf("-b %d\n", args->b);
  printf("-t %s\n", args->trace_file);
}

bool ArgParser(int argc, char* argv[], Args* args) {

  if (argc == 1) {
    ToStderr("%s: Missing required command line argument\n", argv[0]);
    ToStderr("%s", help_str);
    return false;
  }

  args->h = false;
  args->v = false;
  args->s = 0;
  args->E = 0;
  args->b = 0;
  args->trace_file = NULL;

  // Parse Option Arguments
  char c;
  while ((c = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
    switch (c) {
      case 'h':
        args->h = true;
        break;
      case 'v':
        args->v = true;
        break;
      case 's':
        args->s = atoi(optarg);
        break;
      case 'E':
        args->E = atoi(optarg);
        break;
      case 'b':
        args->b = atoi(optarg);
        break;
      case 't':
        args->trace_file = optarg;
        break;
      default:
        return false;
    }
  }

  if (args->h) {
    ToStderr("%s", help_str);
    return false;
  }

  if (args->s == 0 || args->E == 0 ||
      args->b == 0 || args->trace_file == NULL) {
    ToStderr("Arguments are no complete.\n%s", help_str);
    return false;
  }

  // ArgsToString(&_args);
  return true;
}

LRUCache** InitLRUCache(uint32_t s, uint32_t E, LRUCachePara* paras) {
  paras->S = 1 << s;
  paras->E = E;
  paras->time_stamp = 0;
  LRUCache** cache = NULL;

  cache = (LRUCache**)malloc(paras->S * sizeof(LRUCacheLine*));
  if (cache == NULL) {
    ToStderr("Error in allocate memory size: %lu bytes\n",
             paras->S * sizeof(LRUCacheLine));
    return NULL;
  }

  cache[0] = (LRUCache*)malloc(paras->S * paras->E * sizeof(LRUCacheLine));
  if (cache[0] == NULL) {
    ToStderr("Error in allocate memory size: %lu bytes\n",
             paras->S * paras->E * sizeof(LRUCacheLine));
    return NULL;
  }

  size_t stride = paras->E * sizeof(LRUCacheLine);
  for (int i = 1; i < paras->S; ++i) {
    cache[i] = cache[i-1] + stride;
  }
  return cache;
}

void ClearLRUCache(uint32_t S, uint32_t E, LRUCache** cache) {
  if (cache == NULL) return;
  for (int i = 0; i < S; ++i) {
    free(cache[i]);
  }
  free(cache);
}

void LRUCacheSim(const MemoryOperation* memory_op,
                 LRUCache** cache,
                 LRUCachePara* paras,
                 Stats* stats,
                 bool verbose) {
  ;
}

// TODO:
// arg parser
// input file parser
int main(int argc, char* argv[]) {
  Args args;
  if (!ArgParser(argc, argv, &args)) return -1;
  printSummary(0, 0, 0);
  return 0;
}
