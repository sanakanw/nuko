#ifndef BGAME_H
#define BGAME_H

#include "bsp.h"
#include "edict.h"
#include "map_file.h"
#include "../common/nk_math.h"
#include <stdbool.h>

#define MAX_CLIP_PLANES 16

typedef enum {
  BGC_TRANSFORM = (1 << 0),
  BGC_CLIENT    = (1 << 1),
  BGC_CLIP      = (1 << 2),
  BGC_CAPSULE   = (1 << 3),
  BGC_MOTION    = (1 << 4),
  BGC_PMOVE     = (1 << 5),
  BGC_MODEL     = (1 << 5),
  BGC_SHOOT     = (1 << 6),
  AUX_BGC       = (1 << 7)
} bg_component_t;

typedef enum {
  BG_MDL_FUMO_CIRNO,
  MAX_BG_MODEL
} bg_model_t;

typedef struct {
  vec3_t    position;
  quat_t    rotation;
} bg_transform_t;

typedef struct {
  float     forward;
  float     right;
  float     yaw;
  float     pitch;
  float     jump;
  float     attack;
} usercmd_t;

typedef struct {
  usercmd_t usercmd;
} bg_client_t;

typedef struct {
  float     radius;
  float     height;
} bg_capsule_t;

typedef struct {
  plane_t   planes[MAX_CLIP_PLANES];
  int       num_planes;
} bg_clip_t;

typedef struct {
  vec3_t    velocity;
} bg_motion_t;

typedef struct {
  quat_t    move_rot;
  bool      grounded;
  bool      attack;
} bg_pmove_t;

typedef struct {
  edict_t         edict;
  
  bg_pmove_t      cl_pmove;
  bg_motion_t     cl_motion;
  entity_t        cl_entity;
  
  bg_transform_t  sv_transform[MAX_ENTITIES];
  bg_capsule_t    sv_capsule[MAX_ENTITIES];
} snapshot_t;

typedef struct {
  edict_t         *edict;
  bsp_node_t      *bsp;
  
  bg_transform_t  transform[MAX_ENTITIES];
  bg_client_t     client[MAX_ENTITIES];
  bg_clip_t       clip[MAX_ENTITIES];
  bg_capsule_t    capsule[MAX_ENTITIES];
  bg_motion_t     motion[MAX_ENTITIES];
  bg_pmove_t      pmove[MAX_ENTITIES];
  bg_model_t      model[MAX_ENTITIES];
} bgame_t;

void bg_init(bgame_t *bg, edict_t *edict);
void bg_new_map(bgame_t *bg, const map_t *map);
void bg_update(bgame_t *bg);

#endif
