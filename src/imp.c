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
bucket_residents_shift_left(struct bucket *bptr, uint32_t start)
{
	/* Rightmost resident index. */
	if (bptr->resident_count - 1 == start) {
		bptr->residents[start] = 0;
		/* No shifting required. */
	} else if (bptr->resident_count != 1) {
		/* Execute the left shift. */
		register hash_t *optr = &bptr->residents[start];
		/* Might be dangerous? Who cares? */
		memcpy(optr, optr + 1, bptr->resident_count - start);
	}
	--bptr->resident_count;
}
void
hash_table_create(struct hash_table *table)
{
	register size_t alloc = sizeof(*table->buckets) * HASH_TABLE_START_SIZE;
	*table = (struct hash_table) {
		.buckets = (struct bucket *)calloc(HASH_TABLE_START_SIZE, sizeof(*table->buckets)),
		.count = HASH_TABLE_START_SIZE,
		.homed = 0,
	};
	memset(table->buckets, 0, alloc);
}
void
hash_table_realloc(struct hash_table *table, uint32_t new_count)
{
	/* Create a tempory storage for all the residents which are dehomed. */	
	hash_t *dehomed = (hash_t *)calloc(table->homed, sizeof(*dehomed));

	/* Increase/decrease the new count of buckets. */
	table->count = new_count;	
	table->buckets = (struct bucket *)realloc(table->buckets, sizeof(*table->buckets) * new_count);


	free(dehomed);
}
void
hash_table_insert(struct hash_table *table, hash_t hash)
{
	/* Find the index of the bucket it is going within. */
	int32_t bucket = hash % table->count;

	/* Pointer the target bucket. */
	struct bucket *bptr = &table->buckets[bucket];

	/* Check if it has been allocated. */
	if (bptr->residents == NULL) {
		bptr->residents = (hash_t *)calloc(HASH_TABLE_START_SIZE, sizeof(*bptr->residents));

		/* Set the alloc count. */
		bptr->alloc_count = HASH_TABLE_START_SIZE;
	}

	bptr->residents[bptr->resident_count] = hash;
	/* Incriment the resident count. */
	++bptr->resident_count;
	bucket_sort(bptr);

	/* Incriment the global count of amount homed, allows for easier reallocation. */
	++table->homed;
}
void
hash_table_insert_batch(struct hash_table *table, hash_t *hashes, uint32_t hash_count)
{
	/* Pretty much the lazy implimentation. */
	uint32_t i = 0;
	for (; i < hash_count; ++i) {
		hash_table_insert(table, hashes[i]);
	}
}
int8_t
hash_table_remove(struct hash_table *table, hash_t hash)
{
	struct bucket *bptr = &table->buckets[hash % table->count];
	/* Validate members actually reside within the bucket. */
	if (bptr->resident_count <= 0 || bptr->residents == NULL) {
		return -1;
	}
	/* Lookup whether it exists. */
	uint64_t index;
	if ((index = binary_search(bptr->residents, hash, 0, bptr->resident_count)) != -1) {
		/* Remove it based on the index, this is done by left shifting the elements and overriding the values. */
		bucket_residents_shift_left(bptr, (uint32_t)index);
		return 0;
	}
	return index;
}
int8_t
hash_table_remove_reference(struct hash_table *table, struct hash_table_home *ref, hash_t *cert)
{
	/* Validate the index of the bucket is no more than the number of buckets. */
	if (table->count < ref->bucket) {
		return -1;
	}

	struct bucket *bref = &table->buckets[ref->bucket];

	/* Validate that the amount of bucket residents is greater than the ref index. */
	if (bref->resident_count < ref->index) {
		return -1;
	}

	/* 
	 * Before removing an resident based on a reference position a lookup is performed to validate the same hash. 
	 * Being an optional parameter if the cert pointer is NULL then the check is ignored and will be removed regardless.
	 * If the cert pointer is neq NULL then a failed match will terminate the function and the end user program should try another lookup.
	 */
	 if (cert != NULL) {
		if (bref->residents[ref->index] != *cert) {
			return -2;
		}
	 }
	/* If all the checks pass then the element is removed, as the array should already be sorted then a simple left shift is run to prevent holes. */
	bucket_residents_shift_left(bref, ref->index);
	
	/* Decriment the resident count, not the alloc count. */
	--bref->resident_count;
	--table->homed;

	return 0;

}
struct hash_table_home
hash_table_find(struct hash_table *table, hash_t hash)
{
	struct hash_table_home home = { .bucket = UINT32_MAX, .index = UINT32_MAX };
	/* Firstly find the bucket where is resides. */
	register uint32_t b_index = hash % table->count;

	struct bucket *bptr = &table->buckets[b_index];
	
	/* Check if it has actually been allocated, if it has not this is a dead giveaway for not a match. */
	if (bptr->residents == NULL) {
		return home;
	}
	/* If then the find function returns < 0, the index was not found, else the casted result is the index. */
	int64_t index = binary_search(bptr->residents, hash, 0, bptr->resident_count);
	
	if (index != -1) {
		home = (struct hash_table_home) {
			.bucket = b_index,
			.index = (uint32_t)index,
		};
	}
	return home;
}

void
hash_table_destroy(struct hash_table *table)
{
	/* Firstly free all the bucket residents. */
	struct bucket *bptr = &(*table->buckets);
	for (; bptr < &table->buckets[table->count]; ++bptr) {
		if (bptr->residents != NULL) {
			free(bptr->residents);
		}
	}
	free(table->buckets);
}


