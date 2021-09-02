#ifndef QUERY_UTILS_H
#define QUERY_UTILS_H

#include <tree_sitter/api.h>

typedef struct QueryUtils {
    TSQuery* query;
    TSQueryCursor* cursor;
    TSQueryMatch match;
    unsigned int error_offset;
    TSQueryError error_type;
} QueryUtils;

QueryUtils query_utils_create(const char* query_path,
                                TSLanguage* l,
                                TSNode root);
void query_utils_destroy(QueryUtils* q);
void query_utils_debug(QueryUtils* q);
void query_utils_highlight(QueryUtils* q, const char* file_name);

#endif