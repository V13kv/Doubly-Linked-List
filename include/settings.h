#ifndef SETTINGS_H
#define SETTINGS_H


#define nodeDataType_t              int

#define OUTPUT_DOT_FILENAME         "out.dot"
#define OUTPUT_SVG_FILENAME         "graph.svg"
#define CREATE_GRAPH_COMMAND        "dot.exe -Tsvg " OUTPUT_DOT_FILENAME " -o " OUTPUT_SVG_FILENAME

const int POISON                    = -663;
const int MIN_LIST_NODES            = 8;
const int CAPACITY_INCREASE_COEFF   = 2;


#endif  // SETTINGS_H
