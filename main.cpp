#include "graph.h"
#include "construct.h"
#include "construct.cpp"
#include "pressurize.h"
#include "pressurize.cpp"
#include "validate.h"
#include "validate.cpp"
#include "find_path.h"
#include "find_path.cpp"

int main(int argc, char* argv[]) {
  Graph basic_graph = construct(argv[1]);
  std::map<int, Graph::Node *> blocked_nodes = pressurize(argv[2], basic_graph);
  Graph valid_graph = validate(blocked_nodes, basic_graph);
  for (auto i : valid_graph.nodes())
    std::cout<<"Node "<<i.first<<" is connected to "<<i.second.connections().size()<<" edges"<<std::endl;
  for (auto i : valid_graph.edges())
    std::cout<<"Edge ("<<i.second.ends().first->index()<<", "<<i.second.ends().second->index()<<")"<<std::endl;
  std::tuple<std::map<Graph::Edge, double>, std::map<std::pair<Graph::Node*, Graph::Node *>, double>, double> seq_par_res = find_path(basic_graph); 
}
