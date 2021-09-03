#include <stdio.h>
#include <string.h>

#include "c_parser.h"
#include "parser_utils.h"
#include "query_utils.h"

int main(int argc, char* argv[]) {
	if (!argv[1] || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr,
			"%s: A program to test the tree-sitter library\n"
			"\n"
			"Usage: %s [file]\n"
			"\n"
			"Options:\n"
			"--help     Show this message\n"
			"--debug    Print out debug information in YAML\n",
			argv[0], argv[0]
		);
		return 2;
	}

	ParserUtils p = parser_utils_create(tree_sitter_c(), argv[1]);
	QueryUtils q  = query_utils_create("queries/highlights.scm", tree_sitter_c(), p.root);

	if (strcmp(argv[1], "--debug") == 0) {
		query_utils_debug(&q);
		query_utils_print_captures(&q);
	} else {
		query_utils_highlight(&q, argv[1]);
	}
	query_utils_destroy(&q);
	parser_utils_destroy(&p);
	return 0;
}