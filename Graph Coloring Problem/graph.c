#include "graph.h"


/*
 *  load a graph from file: adjacency lists
 */
graph_t *graph_load (char *filename) {
  graph_t *g;
  char line[MAX_PATH];
  char node1[MAX_LINE];
  char node2[MAX_LINE];
  int i, j, weight, dir;
  FILE *fp;

  g = (graph_t *) util_calloc (1, sizeof(graph_t)); //Dynamic calloc

  fp = util_fopen(filename, "r"); //file open (prints error on stdout in case of failure)
  fgets(line, MAX_LINE, fp);
  if (sscanf(line, "%d%d", &g->nv, &dir) != 2) { //scan for NumberOfVertices (nv) and know if it's
    sscanf(line, "%d", &g->nv); //                 directed (dir=1) or undirected (dir=0)
    dir = 1; // If the first sscanf doesn't detect two elements, scans only (nv) and sets the
  } //          Graph as directed (dir=1)

  /* create initial structure for vertices */
  for (i=g->nv-1; i>=0; i--) { //Build graph in a FIFO way. Last element inserted has id=0
    g->g = new_node (g->g, i); //New node on (GraphWrapper)->(GraphVertex)
  }

  /* load edges */
  while (fgets(line, MAX_LINE, fp) != NULL) { //Get line
    if (sscanf(line, "%s%s%d", node1, node2, &weight) != 3) { //if vertex has weight, put it!
      sscanf(line, "%s%s", node1, node2); //Otherwise, all edges have weight 1
      weight = 1;
    }
    vertex_t *tmptr = graph_find(g, node1);
    if(tmptr==NULL){
        tmptr = graph_find(g, "nan");
        tmptr->name = util_strdup(node1);
    }
    tmptr = graph_find(g, node2);
    if(tmptr==NULL){
        tmptr = graph_find(g, "nan");
        tmptr->name = util_strdup(node2);
    }

    new_edge (g, node1, node2, weight);
    if (dir == 0) {
      new_edge (g, node2, node1, weight);
    }
  }
  fclose(fp);

  return g;
}

/*
 *  init all graph attributes
 */
void graph_attribute_init (graph_t *g) {
  vertex_t *v;

  v = g->g;
  while (v!=NULL) {
    v->color = WHITE;
    v->dist = INT_MAX;
    v->disc_time = -1;
    v->endp_time = -1;
    v->scc = -1;
    v->pred = NULL;
    v = v->next;
  }

  return;
}

/*
 *  transpose a given graph
 */
graph_t *graph_transpose (graph_t *g) {
  graph_t *h = NULL;
  vertex_t *tmp;
  edge_t *e;
  int i;

  h = (graph_t *) util_calloc (1, sizeof(graph_t));
  h->nv = g->nv;
  for (i=h->nv-1; i>=0; i--) {
    h->g = new_node (h->g, i);
  }
  tmp = g->g;
  while (tmp != NULL) {
    e = tmp->head;
    while (e != NULL) {
      new_edge (h, e->dst->name, tmp->name, e->weight);
      e = e->next;
    }
    tmp = tmp->next;
  }

  return h;
}

/*
 *  find the vertex element with a given id
 */
vertex_t *graph_find (graph_t *g, char *name) {
  vertex_t *v;

  v = g->g;
  while (v != NULL) {
    if (strcmp(v->name, name)==0) {
      return v;
    }
    v = v->next;
  }

  return NULL;
}

vertex_t *graph_find_id (graph_t *g, int id) {
  vertex_t *v;

  v = g->g;
  while (v != NULL) {
    if (v->id == id) {
      return v;
    }
    v = v->next;
  }

  return NULL;
}
/*
 *  quit the memory associated to a graph
 */
void graph_dispose (graph_t *g) {
  vertex_t *v=NULL;
  edge_t *e=NULL;

  v = g->g;
  while (v != NULL) {
    while (v->head != NULL) {
      e = v->head;
      v->head = e->next;
      free (e);
    }
    v = v->next;
  }
  v=g->g;
  while(v != NULL){
    vertex_t* sus=v;
    v=v->next;
    free(sus);
  }

  return;
}

/*
 *  initialize the list element for a new vertex
 */
vertex_t *new_node (vertex_t *g, int id) {
  vertex_t *v;

  v = (vertex_t *) util_malloc (1 * sizeof(vertex_t));
  v->id = id;
  v->name = util_strdup("nan");
  v->color = WHITE; //WHITE = Undiscovered node
  v->dist = INT_MAX; //maximum possible distance
  v->disc_time = -1; //Distance from starting node
  v->endp_time = -1; //Number of iterations required to process the node
  v->head = NULL; //
  v->pred = NULL;
  v->scc = -1;
  v->next = g; //Links the new node backward towards the previous node

  return v;
}

/*
 *  insert a new edge in the graph
 */
void new_edge (graph_t *g, char*node1, char*node2, int weight)  { //Generates the edge from file
  vertex_t *src, *dst;
  edge_t *e;

  src = graph_find (g, node1); //Find source vertex
  dst = graph_find (g, node2); //Find destination vertex

  e = (edge_t *) util_malloc (1 * sizeof(edge_t));
  e->dst = dst;
  e->weight = weight;
  e->next = src->head;
  src->head = e;

  return;
}
