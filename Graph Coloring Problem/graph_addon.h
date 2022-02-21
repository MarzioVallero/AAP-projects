#ifndef _GRAPHADDON
#define _GRAPHADDON

#include "graph.h"
#include "queuePublic.h"

extern int graph_bfs_count(graph_t *, vertex_t *);
extern int graph_bfs_distance(graph_t *, vertex_t *);
extern void graph_dfs_type(graph_t *, vertex_t *);
extern int graph_dfs_type_r(graph_t *, vertex_t *, int, int*, int*, int*, int*);
extern void graph_dfs_depth(graph_t *, vertex_t *);
extern int graph_dfs_depth_r(graph_t *, vertex_t *, int, int*);
extern void graph_dfs_max_path_generic(graph_t *, vertex_t *);
extern int graph_dfs_max_path_generic_r(graph_t *, vertex_t *, int, int*);
extern void graph_dfs_r_max_path_selective(graph_t *, vertex_t *);
extern int graph_dfs_r_max_path_selective_r(graph_t *, vertex_t *, int, int*);
extern void graph_hamilton(graph_t *, vertex_t *);
extern int graph_hamilton_r(graph_t *, vertex_t *, vertex_t *, int, int*);
extern void graph_euler(graph_t *, vertex_t *);
extern void graph_disconnect(graph_t *, vertex_t *);
extern void graph_disconnect_symmetrical(graph_t *, vertex_t *);

#endif
