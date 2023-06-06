#ifndef __S_ERROR__
#define __S_ERROR__

#if !defined(__S_SRC_FILE__)
#error "Private header, only <stest.h> can be included directly."
#endif

#include <stdio.h>
#include <stdlib.h>

/*
 * Prints error msg to stderr and exits with failure.
 * Used if process runs out of memory.
 */
#ifndef ERROR_OOM_AND_EXIT
#define ERROR_OOM_AND_EXIT                                                     \
  fprintf(stderr, "out of memory\n");                                          \
  exit(EXIT_FAILURE)
#endif

/*
 * Prints error msg to stderr and exits with failure.
 * Used if global table memory is exceeded (bigger than 56599 entries/tests).
 */
#ifndef ERROR_TABLE_MEMORY_EXCEEDED_AND_EXIT
#define ERROR_TABLE_MEMORY_EXCEEDED_AND_EXIT                                   \
  fprintf(stderr, "global table memory exceeded\n");                           \
  exit(EXIT_FAILURE)
#endif

#endif
