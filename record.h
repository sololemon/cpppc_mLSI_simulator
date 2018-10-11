#ifndef RECORD_H
#define RECORD_H

void output(Graph &, std::tuple<std::map<Graph::Edge, std::pair<Graph::Edge *, double>>, std::map<std::pair<Graph::Node*,Graph::Node*>, double>, double> &, std::pair<std::map<Graph::Edge*, double>, double> &, std::map<Graph::Edge*, double> &, double);

void output_path(std::ofstream &, Graph &);

void output_parallel_path(std::ofstream & output, std::tuple<std::map<Graph::Edge, std::pair<Graph::Edge *, double>>, std::map<std::pair<Graph::Node*,Graph::Node*>, double>, double> &);

void output_flow_rate_distribution(std::ofstream &, std::pair<std::map<Graph::Edge*, double>, double> &, Graph &);

void output_channel_utilization(std::ofstream &, Graph &, std::map<Graph::Edge *, double> &, double);

#endif
