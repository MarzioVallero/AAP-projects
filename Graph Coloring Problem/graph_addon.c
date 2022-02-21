#include "graph_addon.h"

int graph_bfs_count(graph_t *g, vertex_t *n){
  int sol=0;
  queue_t *qp;
  vertex_t *d;
  edge_t *e;

  qp = queue_init (g->nv);
  n->color = GREY; //Discovered node, not yet processed
  n->dist = 0; //First node has distance = 0
  n->pred = NULL; //First node has no predecessor
  queue_put (qp, (void *)n); //Insert the new found GREY element into the queue

  while (!queue_empty_m (qp)) { //If elements in the queue are NOT zero i.e. the queue isn't empty
    queue_get (qp, (void **)&n); //Gets first element from the queue, and assigns to it to n
    e = n->head; //Assign head to dummy pointer
    while (e != NULL) { //If there's still an edge to compute, then
      d = e->dst; //Assign destination to dummy pointer
      if (d->color == WHITE) { //If the node has NOT YET been discovered, then
	d->color = GREY; //The node has been discovered, but NOT YET processed
	d->dist = n->dist + 1; //Distance from starting node is [(previous node's distance) + 1]
	d->pred = n; //Previous node is the node we extracted from the queue at this iteration
	queue_put (qp, (void *)d); //Add newly discovered node to the queue
      }
      e = e->next; //Move on to compute the next edge
    }
    n->color = BLACK; //The vertex has been fully processed, all its edges have been found
    ++sol;
  }
  queue_dispose (qp, NULL); //We've processed all nodes, time to free the queue

  return sol;
}

int graph_bfs_distance(graph_t *g, vertex_t *n){
    int sol=0;
    queue_t *qp;
    vertex_t *d;
    edge_t *e;

    qp = queue_init (g->nv);
    n->color = GREY; //Discovered node, not yet processed
    n->dist = 0; //First node has distance = 0
    n->pred = NULL; //First node has no predecessor
    queue_put (qp, (void *)n); //Insert the new found GREY element into the queue

    while (!queue_empty_m (qp)) { //If elements in the queue are NOT zero i.e. the queue isn't empty
    queue_get (qp, (void **)&n); //Gets first element from the queue, and assigns to it to n
    e = n->head; //Assign head to dummy pointer
    while (e != NULL) { //If there's still an edge to compute, then
          d = e->dst; //Assign destination to dummy pointer
          if (d->color == WHITE) { //If the node has NOT YET been discovered, then
        d->color = GREY; //The node has been discovered, but NOT YET processed
        d->dist = n->dist + 1; //Distance from starting node is [(previous node's distance) + 1]
        d->pred = n; //Previous node is the node we extracted from the queue at this iteration
        queue_put (qp, (void *)d); //Add newly discovered node to the queue
          }
          e = e->next; //Move on to compute the next edge
        }
        n->color = BLACK; //The vertex has been fully processed, all its edges have been found
        if(sol<n->dist){
            sol=n->dist;
        }
    }
    queue_dispose (qp, NULL); //We've processed all nodes, time to free the queue


    return sol;
}

void graph_dfs_type(graph_t *g, vertex_t *n){
    int currTime=0, tree=0, forward=0, backward=0, cross=0;
  vertex_t *tmp, *tmp2;

  printf("List of edges:\n");
  currTime = graph_dfs_type_r (g, n, currTime, &tree, &forward, &backward, &cross);
  for (tmp=g->g; tmp!=NULL; tmp=tmp->next) {
    if (tmp->color == WHITE) {
      currTime = graph_dfs_type_r (g, tmp, currTime, &tree, &forward, &backward, &cross);
    }
  }

  printf("List of vertices:\n");
  for (tmp=g->g; tmp!=NULL; tmp=tmp->next) {
    tmp2 = tmp->pred;
    printf("%2d: %2d/%2d (%d)\n",
      tmp->id, tmp->disc_time, tmp->endp_time,
      (tmp2!=NULL) ? tmp->pred->id : -1);
  }

  fprintf(stdout, "\nThere are exactly:\n%d Tree vertexes\n%d Forward vertexes\n%d Backward vertexes\n%d Crossing vertexes\n\n",
          tree, backward, forward, cross);
}

int graph_dfs_type_r(graph_t *g, vertex_t *n, int currTime, int *tree, int *forward, int *backward, int *cross) {
  edge_t *e;
  vertex_t *tmp;

  n->color = GREY; //new node found, yet to be processed
  n->disc_time = ++currTime; //Increase with each new found node iteration
  e = n->head; //Initialize as first edge of the vertex in order to analyze all its possible subsequent edges
  while (e != NULL) {
    tmp = e->dst;
    switch (tmp->color) {
      case WHITE: printf("%d -> %d : Tree\n", n->id, tmp->id); ++(*tree); break;
      case GREY : printf("%d -> %d : Backward\n", n->id, tmp->id); ++(*backward); break;
      case BLACK: if (n->disc_time < tmp->disc_time) {
                    printf("%d -> %d : Forward\n", n->disc_time, tmp->disc_time); ++(*forward);
                  } else {
                    printf("%d -> %d : Crossing\n", n->id, tmp->id); ++(*cross);
                  }
    }
    if (tmp->color == WHITE) {
      tmp->pred = n;
      currTime = graph_dfs_type_r (g, tmp, currTime, tree, forward, backward, cross);
    }
    e = e->next;
  }
  n->color = BLACK;
  n->endp_time = ++currTime;

  return currTime;
}

void graph_dfs_depth(graph_t *g, vertex_t *n){
    int currTime=0, JohnnyDepp=0;
  vertex_t *tmp, *tmp2;

  printf("List of edges:\n");
  currTime = graph_dfs_depth_r (g, n, currTime, &JohnnyDepp);
  for (tmp=g->g; tmp!=NULL; tmp=tmp->next) {
    if (tmp->color == WHITE) {
      currTime = graph_dfs_depth_r (g, tmp, currTime, &JohnnyDepp);
    }
  }

  printf("List of vertices:\n");
  for (tmp=g->g; tmp!=NULL; tmp=tmp->next) {
    tmp2 = tmp->pred;
    printf("%2d: %2d/%2d (%d)\n",
      tmp->id, tmp->disc_time, tmp->endp_time,
      (tmp2!=NULL) ? tmp->pred->id : -1);
  }

  fprintf(stdout, "\nThe maximum depth of the graph is %d\n\n", JohnnyDepp);
}

int graph_dfs_depth_r(graph_t *g, vertex_t *n, int currTime, int *depth) {
  edge_t *e;
  vertex_t *tmp;

  n->color = GREY; //new node found, yet to be processed
  n->disc_time = ++currTime; //Increase with each new found node iteration
  e = n->head; //Initialize as first edge of the vertex in order to analyze all its possible subsequent edges
  while (e != NULL) {
    tmp = e->dst;
    switch (tmp->color) {
      case WHITE: printf("%d -> %d : Tree\n", n->id, tmp->id); break;
      case GREY : printf("%d -> %d : Backward\n", n->id, tmp->id); break;
      case BLACK: if (n->disc_time < tmp->disc_time) {
                    printf("%d -> %d : Forward\n", n->disc_time, tmp->disc_time);
                  } else {
                    printf("%d -> %d : Crossing\n", n->id, tmp->id);
                  }
    }
    if (tmp->color == WHITE) {
      tmp->pred = n;
      currTime = graph_dfs_depth_r (g, tmp, currTime, depth);
    }
    e = e->next;
  }
  n->color = BLACK;
  n->endp_time = ++currTime;
    ++(*depth);
  return currTime;
}

void graph_dfs_max_path_generic(graph_t *g, vertex_t *n){
    int currTime=0, maxD=0;
    vertex_t *tmp, *tmp2;
    edge_t *tmpedge;

    printf("List of edges:\n");
    currTime = graph_dfs_max_path_generic_r (g, n, currTime, &maxD);
    for (tmp=g->g; tmp!=NULL; tmp=tmp->next) {
        if (tmp->color == WHITE) {
            currTime = graph_dfs_max_path_generic_r (g, tmp, currTime, &maxD);
        }
    }

    printf("List of vertices:\n");
    for (tmp=g->g; tmp!=NULL; tmp=tmp->next) {
    tmp2 = tmp->pred;
    printf("%2d: %2d/%2d (%d)\n", tmp->id, tmp->disc_time, tmp->endp_time, (tmp2!=NULL) ? tmp->pred->id : -1);
    }

    for(int i=currTime; i>0; --i){ //Find the vertexes in descending endp_time order
        for (tmp=g->g; tmp!=NULL; tmp=tmp->next) { //Check all vertexes
            if(tmp->endp_time==currTime){ //Case first node, distance = 0
                tmp->scc = 0;
            }
            if(tmp->endp_time==i){ //If the desired vertex is found, then
                tmpedge = tmp->head;
                while (tmpedge != NULL) { //Checks all edges exiting the vertex
                    tmp2 = tmpedge->dst;
                    if(((tmp->scc) + (tmpedge->weight)) > tmp2->scc){ //Inverse relaxation for highest distance
                        tmp2->scc = ((tmp->scc) + (tmpedge->weight));
                    }
                    tmpedge = tmpedge->next;
                }
            }
        }
    }
    maxD = 0;
    for (tmp=g->g; tmp!=NULL; tmp=tmp->next) {
    maxD = maxD + tmp->scc;
    }


    fprintf(stdout, "\nThe longest path from source vertex in the graph is %d\n\n", maxD);
}

int graph_dfs_max_path_generic_r(graph_t *g, vertex_t *n, int currTime, int *maxD) {
    edge_t *e;
    vertex_t *tmp;

    n->color = GREY; //new node found, yet to be processed
    if(n->disc_time==-1){
        n->disc_time = ++currTime; //Increase with each new found node iteration
    }
    e = n->head; //Initialize as first edge of the vertex in order to analyze all its possible subsequent edges
    while (e != NULL) {
        tmp = e->dst;
        switch (tmp->color) {
            case WHITE: printf("%d -> %d : Tree\n", n->id, tmp->id); break;
            case GREY : printf("%d -> %d : Backward\n", n->id, tmp->id); break;
            case BLACK: if (n->disc_time < tmp->disc_time) {
                    printf("%d -> %d : Forward\n", n->disc_time, tmp->disc_time);
                  } else {
                    printf("%d -> %d : Crossing\n", n->id, tmp->id);
                  }
        }
        if (tmp->color == WHITE) {
            tmp->pred = n;
            currTime = graph_dfs_max_path_generic_r (g, tmp, currTime, maxD);
        }
    e = e->next;
    }
    n->color = BLACK;
    n->endp_time = ++currTime;
    ++(*maxD);
    return currTime;
}

void graph_dfs_r_max_path_selective (graph_t *g, vertex_t *n) { //Computes DFS from vertex src given by user
    int currTime=0, maxD=0;
    vertex_t *tmp, *tmp2;
    edge_t *tmpedge;

    printf("List of edges:\n");
    currTime = graph_dfs_r_max_path_selective_r (g, n, currTime, &maxD);
    for (tmp=g->g; tmp!=NULL; tmp=tmp->next) {
        if (tmp->color == WHITE) {
            currTime = graph_dfs_r_max_path_selective_r (g, tmp, currTime, &maxD);
        }
    }

    printf("List of vertices:\n");
    for (tmp=g->g; tmp!=NULL; tmp=tmp->next) {
    tmp2 = tmp->pred;
    printf("%2d: %2d/%2d (%d)\n", tmp->id, tmp->disc_time, tmp->endp_time, (tmp2!=NULL) ? tmp->pred->id : -1);
    }

    for(int i=currTime; i>0; --i){ //Find the vertexes in descending endp_time order
        for (tmp=g->g; tmp!=NULL; tmp=tmp->next) { //Check all vertexes
            if(tmp->endp_time==currTime){ //Case first node, distance = 0
                tmp->scc = 0;
            }
            if(tmp->endp_time==i){ //If the desired vertex is found, then
                tmpedge = tmp->head;
                while (tmpedge != NULL) { //Checks all edges exiting the vertex
                    tmp2 = tmpedge->dst;
                    if(((tmp->scc) + (tmpedge->weight)) > tmp2->scc){ //Inverse relaxation for highest distance
                        tmp2->scc = ((tmp->scc) + (tmpedge->weight));
                    }
                    tmpedge = tmpedge->next;
                }
            }
        }
    }
    maxD = 0;
    for (tmp=g->g; tmp!=NULL; tmp=tmp->next) {
    maxD = maxD + tmp->scc;
    }


    fprintf(stdout, "\nThe longest path from source vertex in the graph is %d\n\n", maxD);
}

int graph_dfs_r_max_path_selective_r(graph_t *g, vertex_t *n, int currTime, int *maxD) { //Recursive dfs from vertex i. labels nodes with td/tq, edges as T/F/B/C
    edge_t *e;
    vertex_t *tmp;

    n->color = GREY; //new node found, yet to be processed
    if(n->disc_time==-1){
        n->disc_time = ++currTime; //Increase with each new found node iteration
    }
    e = n->head; //Initialize as first edge of the vertex in order to analyze all its possible subsequent edges
    while (e != NULL) {
        tmp = e->dst;
        switch (tmp->color) {
            case WHITE: printf("%d -> %d : Tree\n", n->id, tmp->id); break;
            case GREY : printf("%d -> %d : Backward\n", n->id, tmp->id); break;
            case BLACK: if (n->disc_time < tmp->disc_time) {
                    printf("%d -> %d : Forward\n", n->disc_time, tmp->disc_time);
                  } else {
                    printf("%d -> %d : Crossing\n", n->id, tmp->id);
                  }
        }
        if (tmp->color == WHITE) {
            tmp->pred = n;
            currTime = graph_dfs_r_max_path_selective_r (g, tmp, currTime, maxD);
        }
    e = e->next;
    }
    n->color = BLACK;
    n->endp_time = ++currTime;
    ++(*maxD);
    return currTime;
}

void graph_hamilton(graph_t *g, vertex_t *n){
    int currTime=0, cyclefound=0;
    vertex_t *tmp2;

    currTime = graph_hamilton_r (g, n, n, currTime, &cyclefound);

    if(currTime==g->nv){
        fprintf(stdout, "The Hamilton's Path from vertex %d is:\nStart:", n->id);
        for(int i=1; i<(g->nv)+1; ++i){
            for (tmp2=g->g; tmp2!=NULL; tmp2=tmp2->next) {
                if(tmp2->endp_time == i){
                    fprintf(stdout, " --> \"%d\"", tmp2->id); break;
                }
            }
        }
        if(cyclefound==1){
            fprintf(stdout, "\nThe path found is also an Hamilton's Cycle.\n");
        } else {
            fprintf(stdout, "\nThe path found is NOT an Hamilton's Cycle.\n");
        }
    } else {
        fprintf(stdout, "There exists no Hamilton's Cycle from vertex %d.\n", n->id);
    }


}

int graph_hamilton_r(graph_t *g, vertex_t *n, vertex_t *src, int currTime, int *cyclefound){
    edge_t *e;
    vertex_t *tmp;

    n->color = BLACK;
    n->disc_time = 0;
    n->endp_time = ++currTime; //Increase with each new found node iteration
    e = n->head; //Initialize as first edge of the vertex in order to analyze all its possible subsequent edges
    while (e != NULL) {
        tmp = e->dst;
        if (tmp->color == WHITE) {
            tmp->pred = n;
            currTime = graph_hamilton_r (g, tmp, src, currTime, cyclefound);
        }
        e = e->next;
    }

    if(currTime==(g->nv)){
        e = n->head;
        while (e != NULL) {
            tmp = e->dst;
            if (tmp->id == src->id) {
                *cyclefound=1;
            }
            e = e->next;
        }
        if(*cyclefound == 1){
            return currTime;
        } else {
            n->color = WHITE;
            n->disc_time = 0;
            n->endp_time = -1;
            return --currTime;
        }
    } else {
        n->color = WHITE;
        n->disc_time = 0;
        n->endp_time = -1;
        return --currTime;
    }
}

void graph_euler(graph_t *g, vertex_t *n){

}

void graph_disconnect(graph_t *g, vertex_t *n){
    int counter=0, sol=0;
    vertex_t *tmp, *kill=NULL;
    edge_t *tempedge;

    for (tmp=g->g; tmp!=NULL; tmp=tmp->next) {
        tempedge = tmp->head;
        while(tempedge!=NULL){
            ++counter;
            tempedge=tempedge->next;
        }
        if(counter<sol){
            kill=tmp;
        }
        counter=0;
    }

    tempedge=kill->head;
    while(tempedge!=NULL){  //!!! Check out! Not Yet finished!
        edge_t *killedge=tempedge;
        vertex_t *killdest=tempedge->dst;
        tempedge=tempedge->next;
        free(killedge);
    }
}

void graph_disconnect_symmetrical(graph_t *g, vertex_t *n){

}
