#ifndef FLOW_H
#define FLOW_H

std::pair<std::map<Graph::Edge *, double>,double> 
get_flow_rate(std::tuple<std::map<Graph::Edge, std::pair<Graph::Edge *, double>>, std::map<std::pair<Graph::Node*, Graph::Node *>, double>, double> & );

double calculate_flow_rate(double, double);

#endif
