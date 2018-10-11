#include "construct.cpp"
#include "pressurize.cpp"
#include "validate.cpp"
#include "find_path.cpp"
#include "flow.cpp"
#include "simulate_flow.cpp"
#include "record.cpp"

int main(int argc, char* argv[]) {
  Graph basic_graph = construct(argv[1]);
  std::map<int, Graph::Node *> blocked_nodes = pressurize(argv[2], basic_graph);
  Graph valid_graph = validate(blocked_nodes, basic_graph);
  std::tuple<std::map<Graph::Edge, std::pair<Graph::Edge *, double>>, std::map<std::pair<Graph::Node*, Graph::Node *>, double>, double> seq_par_res = find_path(valid_graph); 
  std::pair<std::map<Graph::Edge *, double>,double> flow_collection = get_flow_rate(seq_par_res);
  std::cout<<"Please give the execution time: ";
  double time;
  std::cin>>time;
  std::map<Graph::Edge *, double> utilization = simulate_flow(flow_collection, valid_graph, time);
  output(valid_graph, seq_par_res, flow_collection, utilization, time);
}
