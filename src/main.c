#include <stdio.h>
#include <string.h>
#include <tree_sitter/api.h>

#include "c_parser.h"
#include "util.h"
#include "json_utils.h"

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
	free(source_code);

	TSNode root = ts_tree_root_node(tree);

	char* str = ts_node_string(root);
	printf("%s\n\n", str);
	free(str);
	
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

	printQueryMatchAsJson(qcursor, &qmatch);

	// while(1) {
	// 	if (!ts_query_cursor_next_match(qcursor, &qmatch))
	// 		break;
	
	// 	printf("id = %d\n", qmatch.id);
	// 	printf("pattern_index = %d\n",
	// 		qmatch.pattern_index);
	// 	printf("capture_count = %d\n",
	// 		qmatch.capture_count);
		
	// 	if (qmatch.captures == NULL) {
	// 		fprintf(stderr, "No 'captures' :(\n");
	// 		printf("\n\n");
	// 		continue;
	// 	}
	// 	printf("Capture index: %d\n",
	// 	qmatch.captures->index);

	// 	char* s = ts_node_string(qmatch.captures->node);
	// 	printf("%s\n", s);
	// 	free(s);

	// 	printf("\n\n");
	// }

	ts_query_cursor_delete(qcursor);
	ts_query_delete(query);
	ts_tree_delete(tree);
	ts_parser_delete(parser);
	return 0;
}