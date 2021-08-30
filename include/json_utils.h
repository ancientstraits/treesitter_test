#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <tree-sitter/api.h>

void printQueryMatchAsJson(TSQueryCursor* qcursor,
TSQueryMatch* qmatch);

#endif // !JSON_UTILS_H