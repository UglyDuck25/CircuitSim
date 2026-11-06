#include <stddef.h>

typedef enum{
    RESISTOR,
    VOLTAGE_SOURCE,
    CURRENT_SOURCE
} component_type;

typedef struct{
    char name[100];
    char point1[100];
    char point2[100];
} node;

typedef struct{
    char name[100];
    component_type type;
    node n1;
    node n2;
    int value;
} component;

typedef struct{
    component *components;
    int component_count;

    node *nodes;
    int node_count;
} circuit;

