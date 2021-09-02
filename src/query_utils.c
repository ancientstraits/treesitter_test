#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tree_sitter/api.h>

#include "query_utils.h"
#include "util.h"

QueryUtils query_utils_create(const char* query_path, TSLanguage* l,
TSNode root) {
    QueryUtils q;

    char* query_source = readFile(query_path);

    q.query = ts_query_new(
        l,
        query_source,
        strlen(query_source),
        &q.error_offset,
        &q.error_type
    );
    free(query_source);

    q.cursor = ts_query_cursor_new();
    ts_query_cursor_exec(q.cursor, q.query, root);

    return q;
}

void query_utils_destroy(QueryUtils* q) {
    ts_query_cursor_delete(q->cursor);
    ts_query_delete(q->query);
}

void query_utils_debug(QueryUtils* q) {
    printf("---\n");

    printf("captures: %d\n",
        ts_query_capture_count(q->query));

    while (ts_query_cursor_next_match(q->cursor, &q->match)) {
        unsigned int length;
        printf(
            "match_%02d:\n"
            "  pattern_index: %d\n"
            "  captures:\n"
            "    count:  %d\n"
            "    index:  %d\n"
            "    string: \"%s\"\n\n"
            ,
            q->match.id,
            q->match.pattern_index,
            q->match.capture_count,
            q->match.captures->index,
            ts_query_capture_name_for_id(
                q->query, q->match.captures->index, &length
            )
        );
    }
}

void query_utils_highlight(QueryUtils* q, const char* file_name) {
    char* str = readFile(file_name);
    for (int i = 0; str[i] != '\0'; i++) {
        printf("%c", str[i]);
    }
    free(str);
}
