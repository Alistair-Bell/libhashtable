/* 
 * Copyright (c) 2021 Alistair Bell <alistair@alistairbell.xyz>
 *
 * lib hashtable is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef HASHTABLES_EXTERN_H
#define HASHTABLES_EXTERN_H

#include "inc.h"

/* Hash table implimentation information.
 * ======================================
 *
 * The hash-type is defined in src/inc.h and is not directly controlled by this implimentation.
 * Each hash table is batch reallocated defined by a constant.
 * Each element within the root table stores a bucket, this helps deal with conflicts caused by likewise elements.
 * Until a conflict has occurred the bucket is void and instead the hash resides.
 * Searches consist of a single 64 bit unsigned integer. The lower 32 represent the root table index, upper the bucket index.
 *     If the root index does not contain an bucket then the hash_table_home->bucket will be UINT32_MAX,
 * Hash table buckets use sorted array of hashes, binary search is used to find the resident.
 */

#define HASH_TABLE_START_SIZE 50

struct hash_table {
	struct bucket *buckets;
	uint32_t count;
	uint32_t homed;
};
struct bucket {
	hash_t *residents;
	uint32_t resident_count;
	uint32_t alloc_count;
};

struct hash_table_home {
	uint64_t combined;
	union {
		struct {
			uint32_t bucket;
			uint32_t index;
		};
	};
};

/* Sorts a single bucket. */
void bucket_sort(struct bucket *);
/* Searches a target bucket for an element, returning -1 on failure. */
int64_t binary_search(hash_t *, hash_t, uint32_t, uint32_t);

/* Creates and allocates buckets for the hashtable, should be destroyed with hash_table_destroy to avoid leaks. */
void hash_table_create(struct hash_table *);
/* Resizes the hashtable bucket count rehoming all members, never automatically called. */
void hash_table_realloc(struct hash_table *, uint32_t);
/* Inserts a member into a bucket. */
void hash_table_insert(struct hash_table *, hash_t);
/* Will batch insert elements all at once. */
void hash_table_insert_batch(struct hash_table *, hash_t *, uint32_t);
/* Removes an hash table element based on a hash value. */
int8_t hash_table_remove(struct hash_table *, hash_t);
/* Removes a hash table element based on reference indexing, an optional cert hash can be used to authenticated before removing. */
int8_t hash_table_remove_reference(struct hash_table *, struct hash_table_home *, hash_t *);
/* Returns the bucket and index into the bucket an member lives, UINT32_MAX will be returned if searches fail. */
struct hash_table_home hash_table_find(struct hash_table *, hash_t);
/* Frees all the allocated memory within a hashtable, will segfault if create was not called. */
void hash_table_destroy(struct hash_table *);

/* Misc algorithms, pretty self explanatory. */
void bucket_sort(struct bucket *);
int64_t binary_search(hash_t *, hash_t, uint32_t, uint32_t);

#endif
