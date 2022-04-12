#ifndef MAP_FILE_H
#define MAP_FILE_H

#include "nk-math.h"
#include "bsp.h"
#include <stdio.h>
#include <stdbool.h>

#define MAX_MAP_LUMPS 3
#define MAX_MAP_BRUSH_MTLS 4

typedef struct {
  int fileofs;
  int filelen;
} lump_t;

typedef enum {
  LUMP_VERTICES,
  LUMP_BSP_NODES,
  LUMP_BRUSH_GROUPS
} map_lump_t;

typedef enum {
  MTL_GRASS,
  MTL_CONCRETE,
  MTL_BRICK,
  MTL_BUILDING
} map_brush_mtl_t;

typedef struct {
  int type;
  plane_t plane;
  int behind;
  int ahead;
} map_bsp_node_t;

typedef struct {
  vec3_t pos;
  vec2_t uv;
} map_vertex_t;

typedef struct {
  map_brush_mtl_t mtl;
  int vertexofs;
  int vertexlen;
} map_brush_group_t;

typedef struct {
  lump_t lumps[MAX_MAP_LUMPS];
} map_header_t;

typedef struct {
  FILE *file;
  map_header_t map_header;
} map_t;

bool map_load(map_t *map, const char *path);
map_vertex_t *map_load_vertices(const map_t *map, int *num_vertices);
map_brush_group_t *map_load_brush_groups(const map_t *map, int *num_brush_groups);
bsp_node_t *map_load_bsp_nodes(const map_t *map);

#endif
