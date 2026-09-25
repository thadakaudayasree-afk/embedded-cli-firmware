#ifndef CLI_H
#define CLI_H

// This defines a new TYPE called "cmd_handler" -
// it represents "a function that takes a char* and returns nothing"
typedef void (*cmd_handler)(char *args);

typedef struct {
    char name[20];
    char description[50];
    cmd_handler handler;   // NEW: stores the address of a function
} Command_t;

#endif