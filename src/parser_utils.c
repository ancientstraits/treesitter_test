#include <stdio.h>
#include <string.h>
#include <tree_sitter/api.h>

#include "parser_utils.h"
#include "util.h"

ParserUtils parser_utils_create(TSLanguage* l, const char* file) {
    ParserUtils p;
    p.parser = ts_parser_new();

    ts_parser_set_language(p.parser, l);

    char* source_code = readFile(file);
    p.tree = ts_parser_parse_string(
        p.parser,
        NULL,
        source_code,
        strlen(source_code)
    );

    p.root = ts_tree_root_node(p.tree);

    return p;
}

void parser_utils_destroy(ParserUtils *p) {
    ts_tree_delete(p->tree);
    ts_parser_delete(p->parser);
}