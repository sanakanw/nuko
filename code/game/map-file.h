#ifndef MAP_FILE_H
#define MAP_FILE_H

#include "bsp.h"
#include "../common/lump.h"
#include "../common/nk-math.h"
#include <stdio.h>
#include <stdbool.h>

typedef enum {
  MAP_LUMP_VERTICES,
  MAP_LUMP_BSP_NODES,
  MAP_LUMP_MESH_GROUPS,
  MAP_LUMP_MATERIALS,
  MAX_MAP_LUMPS
} map_lump_t;

typedef struct {
  int           type;
  plane_t       plane;
  int           behind;
  int           ahead;
} map_bsp_node_t;

typedef struct {
  vec3_t        pos;
  vec2_t        uv;
} map_vertex_t;

typedef struct {
  int           material_id;
  int           vertexofs;
  int           vertexlen;
} map_mesh_group_t;

typedef struct {
  char          name[32];
} map_material_t;

typedef struct {
  lump_t        lumps[MAX_MAP_LUMPS];
} map_header_t;

typedef struct {
  FILE          *file;
  map_header_t  header;
} map_t;

bool              map_load(map_t *map, const char *path);
map_vertex_t      *map_load_vertices(const map_t *map, int *num_vertices);
map_mesh_group_t  *map_load_mesh_groups(const map_t *map, int *num_mesh_groups);
map_material_t    *map_load_materials(const map_t *map, int *num_materials);
bsp_node_t        *map_load_bsp(const map_t *map);
void              map_free_bsp(bsp_node_t *bsp);

#endif
