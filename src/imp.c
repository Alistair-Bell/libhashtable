/* 
 * Copyright (c) 2021 Alistair Bell <alistair@alistairbell.xyz>
 *
 * Lib hashtable is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY
 * without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "extern.h"

static void
bucket_realloc(struct bucket *bucket)
{
	fprintf(stderr, "here!\n");
	/* Increase the bucket allocation count. */
	bucket->alloc_count += 15;
	register size_t size = sizeof(*bucket->residents) * bucket->alloc_count;
	bucket->residents = realloc(bucket->residents, size);
}

void
hash_table_create(struct hash_table *table)
{
	register size_t alloc = sizeof(*table->buckets) * HASH_TABLE_START_SIZE;
	*table = (struct hash_table) {
		.buckets = (struct bucket *)calloc(HASH_TABLE_START_SIZE, sizeof(*table->buckets)),
		.bucket_count = HASH_TABLE_START_SIZE,
	};
	memset(table->buckets, 0, alloc);
}

void
hash_table_insert(struct hash_table *table, hash_t *hash)
{
	/* Find the index of the bucket it is going within. */
	uint32_t bucket = *hash % table->bucket_count;

	/* Pointer the target bucket. */
	struct bucket *b = &table->buckets[bucket];

	/* Check if it has been allocated. */
	if (b->residents == NULL) {
		b->residents = (hash_t *)calloc(HASH_TABLE_START_SIZE, sizeof(*b->residents));

		/* Set the alloc count. */
		b->alloc_count = HASH_TABLE_START_SIZE;
	}
	
	b->residents[b->resident_count] = *hash;
	/* Incriment the resident count. */
	++b->resident_count;
	bucket_sort(b);
}
void
hash_table_insert_batch(struct hash_table *table, hash_t *hashes, uint32_t hash_count)
{
	/* Pretty much the lazy implimentation. */
	uint32_t i = 0;
	for (; i < hash_count; ++i) {
		hash_table_insert(table, &hashes[i]);
	}
}

struct hash_table_home
hash_table_find(struct hash_table *table, hash_t *hash)
{
	struct hash_table_home home = { 0 };
	/* Firstly find the bucket where is resides. */
	struct bucket *bptr = &table->buckets[*hash % table->bucket_count];
	
	register uint32_t b_index = *hash % table->bucket_count;
	/* Check if it has actually been allocated, if it has not this is a dead giveaway for not a match. */
	if (bptr->residents == NULL) {
		return home;
	}
	/* If then the find function returns < 0, the index was not found, else the unsigned 32bit integer is the index. */
	int64_t index = binary_search(bptr->residents, *hash, 0, bptr->resident_count);
	
	if (index != -1) {
		home = (struct hash_table_home) {
			.root_index = b_index,
			.bucket_index = (uint32_t) index,
		};
	}
	return home;
}

void
hash_table_destroy(struct hash_table *table)
{
	/* Firstly free all the bucket residents. */
	struct bucket *b = &(*table->buckets);
	for (; b < &table->buckets[table->bucket_count]; ++b) {
		if (b->residents != NULL) {
			free(b->residents);
		}
	}
	free(table->buckets);
}


