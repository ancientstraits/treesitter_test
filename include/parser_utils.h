#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <tree_sitter/api.h>

typedef struct ParserUtils {
    TSParser* parser;
    TSTree* tree;
    TSNode root;
} ParserUtils;

ParserUtils parser_utils_create(TSLanguage* l, const char* file);
void parser_utils_destroy(ParserUtils* p);

#endif // !PARSER_UTILS_H