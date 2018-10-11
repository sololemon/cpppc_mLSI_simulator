#ifndef FIND_PATH_H
#define FIND_PATH_H

std::tuple<std::map<Graph::Edge, std::pair<Graph::Edge*, double>>, std::map<std::pair<Graph::Node *, Graph::Node *>, double>, double > 
find_path(Graph &);

std::map<std::pair<Graph::Node *,Graph::Node *>, std::pair<Graph::Edge *,double> > 
initialize(std::map<Graph::Edge, std::pair<Graph::Edge*, double> > &, std::map<std::pair<Graph::Node *, Graph::Node *>, double> &, Graph &);

std::pair< std::map<Graph::Edge, std::pair<Graph::Edge*, double>>, std::set<std::pair<Graph::Node*,Graph::Node*> > > 
single_refine(std::map<std::pair<Graph::Node*,Graph::Node* >, double > &);

void refine(std::map<std::pair<Graph::Node *,Graph::Node *>, std::pair<Graph::Edge* ,double> > &, std::map<Graph::Edge, std::pair<Graph::Edge*, double>> &, std::map<std::pair<Graph::Node *, Graph::Node *>, double> & );

double get_seq_resist(double);

double get_par_resist(double, double);

#endif
