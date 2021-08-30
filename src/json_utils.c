#include <stdio.h>

#include <tree_sitter/api.h>

#include "json_utils.h"

void printQueryMatchAsJson(TSQueryCursor* qcursor,
TSQueryMatch* qmatch) {
	
	printf("[");
	while(ts_query_cursor_next_match
	(qcursor, qmatch)) {
		printf("\t{\n");
		printf("\t\t\"id\": %d\n", qmatch->id);
		printf("\t\t\"pattern_index\": %d\n", qmatch->pattern_index);
		printf("\t\t\"capture_count\": %d\n", qmatch->capture_count);
		printf("\t},\n");
	}
	printf("]\n");
}
