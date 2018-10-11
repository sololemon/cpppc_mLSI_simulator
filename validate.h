#ifndef VALIDATE_H
#define VALIDATE_H

#include<map>
#include "graph.h"

Graph validate(std::map<int, Graph::Node *> &, Graph &);

Graph initialize(std::map<int, Graph::Node *> &, Graph &);

void delete_invalid_branch(Graph &);

void delete_intermediate_node(Graph &);

void guide_direction(Graph &);

Graph::Edge merge(int, Graph::Edge &, Graph::Edge &);

void refine_edges(Graph &);

template<typename Iterator>
std::set<Graph::Edge *> get_related_edges(Iterator, Iterator);




#endif
