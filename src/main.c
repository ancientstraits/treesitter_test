#include <stdio.h>
#include <string.h>

#include "c_parser.h"
#include "parser_utils.h"
#include "query_utils.h"

int main(int argc, char* argv[]) {
	ParserUtils p = parser_utils_create(tree_sitter_c(), "examples/one.c");
	QueryUtils q  = query_utils_create("queries/one.scm", tree_sitter_c(), p.root);
	// query_utils_debug(&q);
	// query_utils_print_captures(&q);
	query_utils_highlight(&q, "examples/one.c");
	parser_utils_destroy(&p);
}