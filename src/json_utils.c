#include <stdio.h>

#include <tree_sitter/api.h>
#include <json-c/json.h>

#include "json_utils.h"

void printQueryMatchAsJson(TSQueryCursor* qcursor,
TSQueryMatch* qmatch) {
	json_object* root = json_object_new_object();
	
	while(ts_query_cursor_next_match
	(qcursor, qmatch)) {
		json_object* match = json_object_new_object();
		json_object_object_add(match, "id",
		json_object_new_int(qmatch->id));
		json_object_object_add(match, "pattern_index",
		json_object_new_int(qmatch->pattern_index));
		json_object_object_add(match, "capture_count",
		json_object_new_int(qmatch->capture_count));

		json_object* captures = json_object_new_object();
		json_object_object_add(captures, "index",
		json_object_new_int(qmatch->captures->index));

		json_object* node = json_object_new_object();
		char* str = ts_node_string(qmatch->captures->node);
		json_object_object_add(node, "string",
		);
	}
}
