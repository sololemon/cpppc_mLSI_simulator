#include "graph.h"
#include "flow.h"

std::pair<std::map<Graph::Edge *, double>, double> 
get_flow_rate(std::tuple<std::map<Graph::Edge, std::pair<Graph::Edge *,double>>, 
                         std::map<std::pair<Graph::Node*, Graph::Node *>, 
                         double>,
              double> & seq_par_res){
  double flow_rate_total = calculate_flow_rate(PRESSURE, std::get<2>(seq_par_res));
  std::map<Graph::Edge *, double> flow_rate;
  for (auto & i : std::get<1>(seq_par_res))
    for (auto & j : std::get<0>(seq_par_res))
      if (i.first == j.first.ends())
        flow_rate[j.second.first] = i.second/j.second.second; 
  return std::make_pair(flow_rate, flow_rate_total);     
}

// pressure: Pa, flow_rate: m^3/s
double calculate_flow_rate(double pressure, double resistance){
  return (pressure/resistance);
}
