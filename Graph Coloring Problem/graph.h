#ifndef _GRAPH
#define _GRAPH

#include "limits.h"
#include "util.h"

/* constant declaration */
#define MAX_LINE 100
enum {WHITE, GREY, BLACK};

typedef struct graph_s graph_t;
typedef struct vertex_s vertex_t;
typedef struct edge_s edge_t;

/* graph wrapper */
struct graph_s {
  vertex_t *g; //First vertex of the graph
  int nv; //number of vertices of the graph
};

/* list (vertices) of lists (edges): edges */
struct edge_s {
  int weight; //Weight of the edge (link between nodes)
  vertex_t *dst; //Destination node pointed by the vertex
  edge_t *next; //Next edge of the vertex we're evaluating
};

/* list (vertices) of lists (edges): vertices */
struct vertex_s {
  int id; //name of the vertex (node)
  char *name;
  int color; //Either Undiscovered (WHITE), Discovered(GREY) or Processed(BLACK)
  int dist; //Number of vertexes from the starting vertex
  int disc_time; //Discovery time
  int endp_time; //Processing time
  int scc;
  vertex_t *pred; //Previous node
  edge_t *head; //list of all the edges of the vertex
  vertex_t *next; //Next node
};

/* extern function prototypes */
extern graph_t *graph_load (char *);
extern void graph_attribute_init (graph_t *);
extern graph_t *graph_transpose (graph_t *);
extern vertex_t *graph_find (graph_t *, char*);
extern vertex_t *graph_find_id (graph_t *, int);
extern void graph_dispose (graph_t *);
extern vertex_t *new_node (vertex_t *, int);
extern void new_edge (graph_t *, char*, char*, int);

#endif
