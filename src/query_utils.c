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
            "    node:\n"
            "      start_byte: %d\n"
            "      end_byte:   %d\n"
            ,
            q->match.id,
            q->match.pattern_index,
            q->match.capture_count,
            q->match.captures->index,
            ts_query_capture_name_for_id(
                q->query, q->match.captures->index, &length
            ),
            ts_node_start_byte(q->match.captures->node),
            ts_node_end_byte(q->match.captures->node)
        );
    }
}

enum Captures {
    CAPTURE_KEYWORD = 0,
    CAPTURE_OPERATOR,
    CAPTURE_DELIMETER,
    CAPTURE_STRING,
    CAPTURE_CONSTANT,
    CAPTURE_NUMBER,
    CAPTURE_FUNCTION,
    CAPTURE_FUNCTION_SPECIAL,
    CAPTURE_PROPERTY,
    CAPTURE_LABEL,
    CAPTURE_TYPE,
    CAPTURE_VARIABLE,
    CAPTURE_COMMENT,
};

enum Colors {
    COLOR_BLACK = 30,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_PURPLE,
    COLOR_CYAN,
    COLOR_WHITE,
};

unsigned long match_map[] = {
    [CAPTURE_KEYWORD] = COLOR_RED,
    [CAPTURE_OPERATOR] = COLOR_YELLOW,
    [CAPTURE_DELIMETER] = COLOR_BLUE,
    [CAPTURE_STRING] = COLOR_CYAN,
    [CAPTURE_CONSTANT] = COLOR_PURPLE,
    [CAPTURE_NUMBER] = COLOR_GREEN,
    [CAPTURE_FUNCTION] = COLOR_RED,
    [CAPTURE_FUNCTION_SPECIAL] = COLOR_YELLOW,
    [CAPTURE_PROPERTY] = COLOR_BLUE,
    [CAPTURE_LABEL] = COLOR_CYAN,
    [CAPTURE_TYPE] = COLOR_PURPLE,
    [CAPTURE_VARIABLE] = COLOR_GREEN,
    [CAPTURE_COMMENT] = COLOR_RED,
};

#define cprintf(color, format, ...) \
    printf("\033[0;%lum" format "\033[0m", color, __VA_ARGS__)

void query_utils_print_captures(QueryUtils* q) {
    printf("All captures:\n");
    for (int i = 0; i < ts_query_capture_count(q->query); i++) {
        unsigned int x;
        printf("  - %s\n", ts_query_capture_name_for_id(q->query, i, &x));
    }
}

static unsigned long* get_range(QueryUtils* q) {
    return (unsigned long[]) {
        ts_node_start_byte(q->match.captures->node),
        ts_node_end_byte(q->match.captures->node),
    };
}

void query_utils_highlight(QueryUtils* q, const char* file_name) {
    if (!ts_query_cursor_next_match(q->cursor, &q->match)) return;
    unsigned long* range = get_range(q);

    char* str = readFile(file_name);
    printf("range = {%lu, %lu}\n", range[0], range[1]);
    for (int i = 0; str[i] != '\0'; i++) {
        if (i >= range[0] && i < range[1]) {
            cprintf(match_map[q->match.captures->index], "%c", str[i]);
            continue;
        }
        cprintf((unsigned long)COLOR_WHITE, "%c", str[i]);
    }
    free(str);
}
