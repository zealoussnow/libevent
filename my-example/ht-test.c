// Last Update:2015-10-20 15:33:53

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ht-internal.h"

struct data_entry
{
	HT_ENTRY(data_entry) map_node;
	int data;
};

HT_HEAD(data_map, data_entry);

static inline unsigned hash_fn(struct data_entry *e)
{
	unsigned h = (unsigned)e->data;
	h += (h >> 2) | (h << 30);
	return h;
}

static inline int eq_fn(struct data_entry *e1, struct data_entry *e2)
{
	return e1->data == e2->data;
}

HT_PROTOTYPE(data_map, data_entry, map_node, hash_fn, eq_fn)

int main(int argc, const char *argv[])
{
	struct data_map *ctx = (struct data_map *)malloc(sizeof(struct data_map));
	struct data_entry item1;
	memset(&item1, 0, sizeof(item1));
	item1.data = 1;
	HT_INIT(data_map, ctx);
	/*HT_INSERT(data_map, data_map, item1);*/
	
	return 0;
}
