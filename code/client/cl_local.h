/*
-- cl_local.h --

The local definitions of the client module.
*/

#ifndef CL_LOCAL_H
#define CL_LOCAL_H

#include "client.h"

#include "input.h"
#include "renderer.h"
#include "../game/frame.h"
#include "../game/map_file.h"
#include "../common/net.h"
#include "../common/cmd.h"
#include "../common/log.h"
#include "../common/sys.h"
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MAX_CMD_QUEUE   128

typedef struct {
  // Network socket for TCP/WebSocket connection
  sock_t      sock;
  bool        connected;
  
  // The last snapshot received from the server
  int         snapshot_ack;
  snapshot_t  snapshot;
  
  // The client's usercmd set by polling peripheral input
  usercmd_t   usercmd;
  
  // A cache of unacknowledged usercmds used in cl_predict()
  int         cmd_tail;
  int         cmd_head;
  usercmd_t   cmd_queue[MAX_CMD_QUEUE];
} client_t;

extern client_t cl;

//
// - cl_main.c -
// Main functions called by other modules
//

// Parse input from the console
void  cl_console();

//
// - cl_cgame.c -
// Synchronise with the server and produce a smooth game, responsive game state
// for the renderer
//

// - Take a snapshot of the game state to be used for interpolation -
void  cl_snapshot();

// - Interpolate between two snapshots to produce a smooth inbetween state -
// This is necessary as the game will run at a lower update rate than frame
// rate. To produce produce smooth movement, two states of the game are
// interpolated between.
void  cl_interpolate(float interp);

// - Orient the camera based on the current usercmd -
// While this is also done through game logic, rotation is difficult to
// interpolate between and is therefore not considered in interpolation.
// Instead, the player's rotation is updated every frame. This is neceesary the
// camera's orientation is the most salient change between states.
void  cl_view_look();

// - "Predict" the future state of the game -
// Because of network latency, the game state received by the client will be
// behind by some amount. For the game to feel responsive the client simulates
// usercmds unacknolwedged by the server to produce a game state ahead of what
// is received.
void  cl_predict();

// - Set the game state to the last snapshot received -
void  cl_reconcile();

// - Load a map into the game state -
void  cl_load_map(const char *path);

//
// - cl_net.c -
// Maintain a network connection with the server/host. This involves parsing
// packets sent by the server and sending packets created by the client, 
//

// - Connect to a host address -
void  cl_connect(const char *host);

// - Receive and parse network packets from the server -
void  cl_parse();

// - Parse a snapshot packet -
void  cl_parse_snapshot(const frame_t *frame);

// - Parse a client info packet -
void  cl_parse_client_info(const frame_t *frame);

// - Parse a chat packet -
void  cl_parse_chat(const frame_t *frame);

// - Send a usercmd -
void  cl_send_cmd();

// - Send a chat message -
void  cl_send_chat(const char *text);

// - Send a name change request -
void  cl_send_name(const char *name);

// - Send a scoreboard request -
void  cl_send_score();

#endif
