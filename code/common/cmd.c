#include "cmd.h"

#include "log.h"
#include "string.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TEXT        256
#define MAX_STR         256
#define MAX_ARGS        8
#define MAX_FUNCTIONS   32

typedef struct {
  const char  *name;
  void        *d;
  xcommand_t  xcommand;
} cmd_function_t;

static char            cmd_text[MAX_TEXT];
static char            *cmd_text_ptr;

static int             cmd_arg_count;
static char            *cmd_arg_value[MAX_ARGS];

static cmd_function_t  cmd_functions[MAX_FUNCTIONS];
static int             cmd_num_functions;

void cmd_init()
{
  cmd_text_ptr = cmd_text;
  cmd_num_functions = 0;
  
  memset(cmd_text, 0, sizeof(cmd_text));
}

bool cmd_execute()
{
  if (cmd_text_ptr == cmd_text)
    return true;
  
  char *cmd_queue = cmd_text;
  
  char *str_cmd;
  while ((str_cmd = strtok_r(cmd_queue, ";\n", &cmd_queue))) {
    cmd_arg_count = 0;
    
    while ((cmd_arg_value[cmd_arg_count] = strtok_r(str_cmd, " ", &str_cmd))) {
      cmd_arg_count++;
      
      if (*str_cmd == '"') {
        ++str_cmd;
        cmd_arg_value[cmd_arg_count] = str_cmd;
        
        while (*str_cmd != '"') {
          if (*str_cmd == '\n' || *str_cmd == 0) {
            log_printf(LOG_ERROR, "cmd_execute(): unescaped '\"'");
            return false;
          }
          ++str_cmd;
        }
        
        *str_cmd = 0;
        cmd_arg_count++;
      }
    }
    
    for (int i = 0; i < cmd_num_functions; i++) {
      if (strcmp(cmd_arg_value[0], cmd_functions[i].name) == 0) {
        cmd_functions[i].xcommand(cmd_functions[i].d);
        break;
      }
    }
  }
  
  cmd_text_ptr = cmd_text;
  
  return true;
}

void cmd_puts(const char *text)
{
  int len = strlen(text);
  
  if (cmd_text_ptr + len >= &cmd_text[MAX_TEXT]) {
    log_printf(LOG_ERROR, "cmd_puts(): ran out of memory");
    return;
  }
  
  strcpy(cmd_text_ptr, text);
  cmd_text_ptr += len;
}

void cmd_add_command(const char *text, xcommand_t xcommand, void *d)
{
  const char *name = string_copy(text);
  
  if (!name) {
    log_printf(LOG_ERROR, "cmd_add_command(): could not allocate string");
    return;
  }
  
  if (cmd_num_functions + 1 >= MAX_FUNCTIONS) {
    log_printf(LOG_ERROR, "cmd_add_command(): ran out of memory");
    return;
  }
  
  cmd_function_t *cmd_function = &cmd_functions[cmd_num_functions++];
  cmd_function->name = name;
  cmd_function->xcommand = xcommand;
  cmd_function->d = d;
}

int cmd_argc()
{
  return cmd_arg_count;
}

const char *cmd_argv(int n)
{
  return cmd_arg_value[n];
}