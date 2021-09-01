#include <stdio.h>
#include <string.h>
#include <tree_sitter/api.h>
#include <assert.h>

#include "c_parser.h"
#include "util.h"
#include "json_utils.h"

static unsigned long* getRange(TSQueryMatch* qmatch) {
	return (unsigned long[]) {
		ts_node_start_byte(qmatch->captures->node),
		ts_node_end_byte(qmatch->captures->node),
	};
}

int main(int argc, char* argv[]) {
	
	TSParser* parser = ts_parser_new();
	ts_parser_set_language(parser, tree_sitter_c());

	char* source_code = readFile("examples/one.c");
	if (!source_code) return 1;

	TSTree* tree = ts_parser_parse_string(
		parser,
		NULL,
		source_code,
		strlen(source_code)
	);

	TSNode root = ts_tree_root_node(tree);

	char* query_source = readFile("queries/one.scm");
	uint32_t qoffset;	
	TSQueryError qerror;
	TSQuery* query = ts_query_new(
		tree_sitter_c(),
		query_source,
		strlen(query_source),
		&qoffset,
		&qerror
	);
	free(query_source);

	TSQueryCursor* qcursor = ts_query_cursor_new();
	ts_query_cursor_exec(qcursor, query, root);

	TSQueryMatch qmatch;

// #define DBG_HL
#ifdef DBG_HL
	while (ts_query_cursor_next_match(qcursor, &qmatch)) {
		printf("Query %d:\n", qmatch.id);
		printf("\tPattern Index: %d\n", qmatch.pattern_index);
		char* str = ts_node_string(qmatch.captures->node);
		printf("\tString: %s\n", str);
		printf("\tCapture count: %d\n", qmatch.capture_count);
		free(str);
		unsigned int x;
		printf("\tCapture name: %s\n", ts_query_capture_name_for_id(query, qmatch.captures->index, &x));
		printf("\tCapture length: %d\n", x);
		printf("\n");
	}
#else
	ts_query_cursor_next_match(qcursor, &qmatch);
	unsigned long* range = getRange(&qmatch);
	
	for (unsigned long i = 0; source_code[i] != '\0'; i++) {
		if (i >= range[0] && i <= range[1] - 1) {
			cprintf((int)type_map[qmatch.captures->index], "%c", source_code[i]);

			if (i == range[1] - 1) {
				if (!ts_query_cursor_next_match(qcursor, &qmatch)) continue;
				
				range = getRange(&qmatch);
			}
		} else
			cprintf(COLOR_WHITE, "%c", source_code[i]);
	}
#endif

	free(source_code);
	ts_query_cursor_delete(qcursor);
	ts_query_delete(query);
	ts_tree_delete(tree);
	ts_parser_delete(parser);
	return 0;
}