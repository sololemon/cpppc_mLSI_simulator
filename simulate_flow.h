#ifndef SIMULATE_FLOW_H
#define SIMULATE_FLOW_H

std::map<Graph::Edge *, double> 
simulate_flow (std::pair<std::map<Graph::Edge*,double>,double> &, Graph &, double);

double get_volumn(double, double);

Graph::Node * find_departure(Graph &);

double get_cur_volumn(double, int, int, std::map<Graph::Edge*, double> &, Graph &);

void check_capacity(double, Graph::Edge *, std::map<Graph::Edge *, double> &, std::map<Graph::Edge*, double> &, std::map<Graph::Edge*,double> &);

double distribution(Graph::Edge *, std::map<Graph::Edge*, double> &);

double channel_volumn(double, double, double);

int find_next(int, std::set<Graph::Edge *> &, Graph &);

#endif
