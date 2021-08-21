#ifndef HASHTABLES_INC_DEF_H
#define HASHTABLES_INC_DEF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>

/* 
 * As this project expands, users of the library should keep an eye for new configuration options.
 * These options dictate how this library functions.
 */

/* Defines the type that the hash uses. */
typedef uint64_t hash_t;

/* Defines the starting amount of buckets the hashtable allocates. */
#define HASHTABLE_STARTING_BUCKET_COUNT 50


#endif
