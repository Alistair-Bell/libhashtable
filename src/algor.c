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


#define VC(ptr) (*(int *)ptr)

static int
qsort_callback_func(const void *first, const void *second) 
{
	return (int)VC(first) - VC(second);
}

int64_t
binary_search(hash_t *hashes, hash_t search, uint32_t min, uint32_t max)
{
	register uint32_t middle = (uint32_t) (max - min) / 2;
	if (min <= max) {
		if (hashes[middle] == search) {
			return middle;
		}
		if (search < hashes[middle]) {
			return binary_search(hashes, search, min, middle - 1);
		}
		return binary_search(hashes, search, middle + 1, max);
	}
	return -1;
}

void
bucket_sort(struct bucket *bucket)
{
	hash_t *br = bucket->residents;
	switch (bucket->resident_count) {
		case 1: {
			/* For a single element then it is already sorted. */
			break;
		}
		/* Simple comparison . */
		case 2: {
			/* Check if the second element is less than the first. */
			if (*br > *(br + 1)) {
				register hash_t temp = *br;
				*br = *(br + 1);
				*(br + 1) = temp;
			}
			break;
		}
		default: {
			qsort(bucket->residents, bucket->resident_count, sizeof(*br), qsort_callback_func);
		}
	}
}
