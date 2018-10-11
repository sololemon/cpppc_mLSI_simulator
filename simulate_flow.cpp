#include "graph.h"
#include "simulate_flow.h"

std::map<Graph::Edge *, double> 
simulate_flow (std::pair<std::map<Graph::Edge*,double>,double> & flow_collection, Graph & g, double time){
  double flow_volumn = get_volumn(flow_collection.second, time);
  std::set<Graph::Edge *> processed;
  std::map<Graph::Edge *, double> utilization;
  std::map<Graph::Edge *, double> remain;
  int departure = find_departure(g)->index();
  int cur_node = departure;
  while (cur_node != 0){
    int next_node = 0;
    double cur_volumn = get_cur_volumn(flow_volumn, departure, cur_node, remain, g);
    for (auto & i : g.nodes()[cur_node].out()){
      check_capacity(cur_volumn, i, utilization, remain, flow_collection.first);
      processed.insert(i);
      next_node = find_next(cur_node, processed, g);
    }
    cur_node = next_node;
  }
  return utilization;
}

double get_volumn(double flow_rate, double time){
  return flow_rate * time;
}

Graph::Node * find_departure(Graph & g){
  Graph::Node * target;
  for (auto & i : g.nodes()){
    if (i.second.type()=='i'){
      target = &i.second;
      break;
    }
    else
      std::cerr<<"There is no inlet and thus no flow movement."<<std::endl;
  }
  return target;
}

double get_cur_volumn(double total_volumn, int departure, int cur_node, std::map<Graph::Edge*, double> & remain, Graph & g){
  if (cur_node == departure)
    return total_volumn;
  else{
    double cur_volumn = 0;
    for (auto & i : g.nodes()[cur_node].in())
      if (remain.find(i) != remain.end())
        cur_volumn += remain[i];
    return cur_volumn;
  }
}

void check_capacity(double cur_volumn, Graph::Edge * cur_edge, std::map<Graph::Edge *, double> & utilization, std::map<Graph::Edge*, double> & remain, std::map<Graph::Edge*,double> & flow_rate){
  double remain_volumn = cur_volumn * distribution(cur_edge, flow_rate) - channel_volumn(WIDTH, HEIGHT, cur_edge->length());
  if (remain_volumn >= 0){
    utilization[cur_edge] = 1;
    remain[cur_edge] = remain_volumn;
  }
  else{
    utilization[cur_edge] = (cur_volumn * distribution(cur_edge, flow_rate)) / channel_volumn(WIDTH, HEIGHT, cur_edge->length());
    remain[cur_edge] = 0;
  }
}

double distribution(Graph::Edge * cur_edge, std::map<Graph::Edge*, double> & flow_rate){
  if (flow_rate.find(cur_edge) == flow_rate.end())
    return 1;
  else
    return flow_rate[cur_edge];
}

double channel_volumn(double width, double height, double length){
  return width * height * length;
}

int find_next(int cur_node, std::set<Graph::Edge *> & processed, Graph & g){
  int next = 0;
  for (auto i : g.nodes()[cur_node].out()){
    bool predecessor_stable = true;
    for (auto j : i->ends().second->in()){
      if (processed.find(j) == processed.end())
        predecessor_stable = false;
    }
    if (predecessor_stable == true)
      next = i->ends().second->index();
  }
  return next;
}






