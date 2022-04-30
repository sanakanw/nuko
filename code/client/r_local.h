#ifndef R_LOCAL_H
#define R_LOCAL_H

#include "gl.h"
#include "cgame.h"
#include "../game/map_file.h"
#include <stdbool.h>

#include <stdbool.h>

#define VERTEX_ATTRIB_0 3
#define VERTEX_ATTRIB_1 2

typedef GLuint texture_t;
typedef GLuint shader_t;

typedef struct {
  GLuint offset;
  GLuint size;
} mesh_t;

typedef struct {
  vec3_t pos;
  vec2_t uv;
} vertex_t;

void  mesh_draw(const mesh_t *mesh);

bool  texture_load(texture_t *texture, const char *path);
void  texture_bind(texture_t texture);

bool  shader_init(shader_t *shader, const char *src_vertex, const char *src_fragment);
void  shader_bind(shader_t shader);

typedef struct {
  texture_t       texture;
} r_material_t;

typedef struct {
  GLuint          program;
  GLuint          ul_mvp;
} r_shader_t;

typedef struct {
  int             material_id;
  mesh_t          mesh;
} r_mesh_group_t;

typedef struct {
  r_mesh_group_t  *mesh_groups;
  int             num_mesh_groups;
  
  r_material_t    *materials;
  int             num_materials;
} r_model_t;

typedef struct {
  r_shader_t      shader;
  
  mat4x4_t        projection_matrix;
  mat4x4_t        view_projection_matrix;
  
  r_model_t       map_model;
  
  r_model_t       bg_models[MAX_BG_MODEL];
  int             bg_models_vbo_ptr;
  
  GLuint          vbo;
  int             vbo_size;
  int             vbo_ptr;
} renderer_t;

extern renderer_t r;

bool  r_shader_init();

void  r_vbo_init();
void  r_vbo_reset(int vbo_ptr);
bool  r_new_mesh(mesh_t *mesh, const vertex_t *vertices, int num_vertices);
void  r_draw_mesh(const mesh_t *mesh);

bool  r_load_model(r_model_t *model, const char *path);
void  r_draw_model(const r_model_t *model);

#endif