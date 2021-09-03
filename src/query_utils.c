#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tree_sitter/api.h>

#include "query_utils.h"
#include "util.h"

QueryUtils query_utils_create(const char* query_path, TSLanguage* l, TSNode node) {
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
    ts_query_cursor_exec(q.cursor, q.query, node);
    return q;
}

void query_utils_destroy(QueryUtils* q) {
    ts_query_cursor_delete(q->cursor);
    ts_query_delete(q->query);
}

void query_utils_debug(QueryUtils* q) {
    printf("---\n");

    printf("captures: %d\n\n",
        ts_query_capture_count(q->query));

    while (ts_query_cursor_next_match(q->cursor, &q->match)) {
        unsigned int length;
        char* str = ts_node_string(q->match.captures->node);
        printf(
            "match_%02d:\n"
            "  pattern_index: %d\n"
            "  captures:\n"
            "    count:  %d\n"
            "    index:  %d\n"
            "    string: \"%s\"\n"
            "    node:\n"
            "      string:     %s\n"
            "      start_byte: %d\n"
            "      end_byte:   %d\n"
            "\n"
            ,
            q->match.id,
            q->match.pattern_index,
            q->match.capture_count,
            q->match.captures->index,
            ts_query_capture_name_for_id(
                q->query, q->match.captures->index, &length
            ),
            str,
            ts_node_start_byte(q->match.captures->node),
            ts_node_end_byte(q->match.captures->node)
        );
        free(str);
    }
}

void query_utils_print_captures(QueryUtils* q) {
    printf("All captures:\n");
    for (int i = 0; i < ts_query_capture_count(q->query); i++) {
        unsigned int x;
        printf("  - %s\n", ts_query_capture_name_for_id(q->query, i, &x));
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

    BCOLOR_BLACK = 130,
    BCOLOR_RED,
    BCOLOR_GREEN,
    BCOLOR_YELLOW,
    BCOLOR_BLUE,
    BCOLOR_PURPLE,
    BCOLOR_CYAN,
    BCOLOR_WHITE,
};

unsigned long match_map[] = {
    [CAPTURE_KEYWORD] = COLOR_RED,
    [CAPTURE_OPERATOR] = COLOR_YELLOW,
    [CAPTURE_DELIMETER] = COLOR_BLUE,
    [CAPTURE_STRING] = COLOR_CYAN,
    [CAPTURE_CONSTANT] = COLOR_PURPLE,
    [CAPTURE_NUMBER] = COLOR_GREEN,
    [CAPTURE_FUNCTION] = BCOLOR_RED,
    [CAPTURE_FUNCTION_SPECIAL] = BCOLOR_YELLOW,
    [CAPTURE_PROPERTY] = BCOLOR_BLUE,
    [CAPTURE_LABEL] = BCOLOR_CYAN,
    [CAPTURE_TYPE] = BCOLOR_PURPLE,
    [CAPTURE_VARIABLE] = BCOLOR_GREEN,
    [CAPTURE_COMMENT] = BCOLOR_RED,
};

#define cprintf(color, format, ...) \
    printf("\033[%lu%sm" format "\033[0m", color % 100, \
    color / 100 == 1 ? ";1" : "", __VA_ARGS__);


void query_utils_highlight(QueryUtils* q, const char* file_name) {
    ts_query_cursor_next_match(q->cursor, &q->match);
    int r[2] = {
        ts_node_start_byte(q->match.captures->node),
        ts_node_end_byte(q->match.captures->node),
    };
    char* str = readFile(file_name);
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (i > r[1] && ts_query_cursor_next_match(q->cursor, &q->match)) {
            r[0] = ts_node_start_byte(q->match.captures->node);
            r[1] = ts_node_end_byte(q->match.captures->node);
        }
        if (i >= r[0] && i < r[1]) {
            cprintf(match_map[q->match.captures->index], "%c", str[i]);
            if (i == r[1] - 1) {
                if (!ts_query_cursor_next_match(q->cursor, &q->match)) continue;
                
                r[0] = ts_node_start_byte(q->match.captures->node);
                r[1] = ts_node_end_byte(q->match.captures->node);
            }
            continue;
        }
        cprintf((size_t)COLOR_WHITE, "%c", str[i]);
    }
}