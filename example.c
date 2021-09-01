#include "src/extern.h"

static struct hash_table t = { 0 };
static struct hash_table_home hh = { 0 };

#define fnd 1010

int 
main(void)
{
	hash_table_create(&t);
	hash_table_insert(&t, fnd);
	hh = hash_table_find(&t, fnd);
	fprintf(stdout, "[%u : %u]\n", hh.bucket, hh.index);
	hash_table_remove(&t, fnd);
	hh = hash_table_find(&t, fnd);
	fprintf(stdout, "[%u : %u]\n", hh.bucket, hh.index);

	hash_table_destroy(&t);
}
