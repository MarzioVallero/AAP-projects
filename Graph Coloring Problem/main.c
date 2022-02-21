#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "graph_addon.h"

void vertex_color(graph_t *, vertex_t *, int *);
void add_queue(vertex_t *, vertex_t **);
vertex_t * queue_next(vertex_t **, graph_t *);

int main(int argc, char **argv)
{
    int sol = 0;
    graph_t *graph=NULL;
    if(argc!=2){
        fprintf(stdout, "Error. Source filename missing.\n");
        return EXIT_FAILURE;
    } else {
        graph = graph_load(argv[1]);
    }
    graph_attribute_init(graph);
    vertex_t *src = graph_find_id(graph, 0);

    vertex_color(graph, src, &sol);
    vertex_t *tmp = src;
    while(tmp!=NULL){
        if(tmp->color==WHITE){
            vertex_color(graph, tmp, &sol);
        }
        tmp=tmp->next;
    }
    if(sol==0){
        fprintf(stdout, "Error. The function couldn't solve the given problem.\n");
    } else {
        fprintf(stdout, "%d colors were needed to fulfill the property.\nHere's the vertex list:\n\n", sol+1);
        tmp = src;
        while(tmp!=NULL){
            fprintf(stdout, "Vertex %s, color: %d\n", tmp->name, tmp->dist);
            tmp=tmp->next;
        }
    }

    graph_dispose(graph);
    return 0;
}

void vertex_color(graph_t *graph, vertex_t *src, int *sol){
    vertex_t *v_ptr = src;
    vertex_t *head = NULL;
    while(v_ptr!=NULL){
        edge_t *tempedge = v_ptr->head;
        v_ptr->dist = 0;
        while(tempedge!=NULL){
            if(tempedge->dst->color == WHITE){
                tempedge->dst->color = GREY;
                add_queue(tempedge->dst, &head);
                tempedge = tempedge->next;
            } else if (tempedge->dst->dist == v_ptr->dist){
                (v_ptr->dist)++;
                if(v_ptr->dist > *sol){
                    *sol = v_ptr->dist;
                }
                tempedge = v_ptr->head;
            } else {
                tempedge = tempedge->next;
            }
        }
        v_ptr->color = BLACK;
        if(head == NULL){
            v_ptr = NULL;
        } else {
            v_ptr = queue_next(&head, graph);
        }
    }
    return;
}

void add_queue(vertex_t *new_v, vertex_t **head){
    if(*head == NULL){
        *head = new_node(NULL, new_v->id);
    } else {
        vertex_t *tmp = *head;
        while(tmp!=NULL){
            if(tmp->next == NULL){
                tmp->next = new_node(NULL, new_v->id);
                tmp = NULL;
            } else {
                tmp=tmp->next;
            }
        }
    }
    return;
}

vertex_t * queue_next(vertex_t **head, graph_t *graph){
    vertex_t *tmp = *head;
    vertex_t *temp_ret = graph_find_id(graph, (*head)->id);
    *head = (*head)->next;
    free (tmp);
    return temp_ret;
}
