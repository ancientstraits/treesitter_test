#include <stdio.h>
#include <string.h>
#include <tree_sitter/api.h>

#include "c_parser.h"
#include "util.h"

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
	printf("%s\n", str);
	free(str);
	
	ts_tree_delete(tree);
	ts_parser_delete(parser);
	return 0;
}