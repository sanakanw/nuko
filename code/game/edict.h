#ifndef EDICT_H
#define EDICT_H

#define MAX_ENTITIES 8

typedef struct {
  int entities[MAX_ENTITIES];
  int num_entities;
} edict_t;

typedef int entity_t;

void      edict_init(edict_t *edict);
entity_t  edict_add_entity(edict_t *edict, int state);
void      edict_remove_entity(edict_t *edict, entity_t entity);

#endif
