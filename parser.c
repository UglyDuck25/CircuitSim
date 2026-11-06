#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "model.h"

circuit* my_circuit = NULL;

circuit* create_circuit() {
    circuit *new_circuit = (circuit *)malloc(sizeof(circuit));
    if (new_circuit == NULL) {
        fprintf(stderr, "Memory allocation failed for circuit\n");
        return NULL;
    }
    new_circuit->components = NULL;
    new_circuit->component_count = 0;
    new_circuit->nodes = NULL;
    new_circuit->node_count = 0;
    return new_circuit;
}

int add_component(char* line) {
    char *tokens[4];
    component *new_component;
    char *token = strtok(line, " \t\r\n");

    int i = 0;
    while (token != NULL && i < 4) {
        tokens[i++] = token;
        token = strtok(NULL, " \t\r\n");
    }

    new_component = (component *)malloc(sizeof(component));
    if (new_component == NULL) {
        fprintf(stderr, "Memory allocation failed for component\n");
        return 1;
    }

    printf("All components.\n");

    return 0;
}

int read_input(char* filename){
   FILE* file;
   file = fopen(filename, "r");
   if (file == NULL) {
       fprintf(stderr, "Error opening file: %s\n", filename);
       return 1;
   }

   char line[256];

   while (fgets(line, sizeof(line), file)) {
       // Process each line of the input file
       printf("Read line: %s", line);
       add_component(line);
   }
   fclose(file);
   return 0;
}

int main(int argc, char *argv[]){
   if (argc < 2) {
       fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
       return 1;
   }
   char* filename = argv[1];
   my_circuit = create_circuit();

   read_input(filename);


   return 0;
}
