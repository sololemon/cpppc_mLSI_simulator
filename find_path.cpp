#include<tuple>
#include "graph.h"
#include "find_path.h"

std::tuple<std::map<Graph::Edge, std::pair<Graph::Edge*, double>>, std::map<std::pair<Graph::Node *, Graph::Node *>, double>, double > find_path(Graph & g){
  std::map<Graph::Edge, std::pair<Graph::Edge*, double>> seq_res;
  std::map<std::pair<Graph::Node *, Graph::Node *>, double> par_res;
  std::map<std::pair<Graph::Node *,Graph::Node *>, std::pair<Graph::Edge *, double>> resistance = initialize(seq_res, par_res, g);
  refine(resistance, seq_res, par_res);
  double total_resistance = resistance.begin()->second.second; 
  return std::make_tuple(seq_res, par_res, total_resistance); 
}

std::map<std::pair<Graph::Node *,Graph::Node *>, std::pair<Graph::Edge *,double> > initialize(std::map<Graph::Edge, std::pair<Graph::Edge*, double> > & seq_res, std::map<std::pair<Graph::Node *, Graph::Node *>, double> & par_res, Graph & g){
  std::map<std::pair<Graph::Node *,Graph::Node *>, std::pair<Graph::Edge *, double>> resistance;
  for (auto & i : g.edges()){
    seq_res[Graph::Edge(seq_res.size()+1, i.second.ends(), 0)] = std::make_pair(&(i.second), get_seq_resist(i.second.length()));
    if (resistance.find(i.second.ends()) == resistance.end()){
      resistance[i.second.ends()] = std::make_pair(&(i.second), get_seq_resist(i.second.length()));
    }
    else{
      par_res[i.second.ends()] = get_par_resist(get_seq_resist(i.second.length()), resistance[i.second.ends()].second);
      resistance[i.second.ends()].second = par_res[i.second.ends()];
    }  
  }
  return resistance;
}

std::pair< std::map<Graph::Edge, std::pair<Graph::Edge*, double>>, std::set<std::pair<Graph::Node*,Graph::Node*> > > single_refine(std::map<std::pair<Graph::Node*,Graph::Node* >, std::pair<Graph::Edge*, double> > & resistance){
  std::map<Graph::Edge, std::pair<Graph::Edge*, double> > to_be_added;
  std::set<std::pair<Graph::Node*, Graph::Node*> > to_be_erased;
  std::map<Graph::Node *, int> node_count;
  std::set<Graph::Node *> settled_node;
    // Build the node_count map to count the occurence of each node in the whole bunch of edges
    for (auto & i : resistance){
      if (node_count.find(i.first.first) != node_count.end())
	node_count[i.first.first] += 1;
      else
	node_count[i.first.first] = 1;
      if (node_count.find(i.first.second) != node_count.end())
	node_count[i.first.second] += 1;
      else
        node_count[i.first.second] = 1;
    }
    // If two edges share a node, and the occurence of the node is exactly two (which means that this is a node between sequentially connected edges), merge the two edges as a new edge with the resistance calculated as the sum. Push_back the new edge to to_be_added and add the two original edges to to_be_erased.
    for (auto & i : resistance){
      // ensure that neither end of i has been involved in this iteration
      if (settled_node.find(i.first.first)==settled_node.end() && settled_node.find(i.first.second)==settled_node.end()){
	// compare i with j
	for (auto & j : resistance){
	  // ensure that i and j do not indicate the same edge
	  if (i.first != j.first)
	  // ensure that neither end of j has been involved in this iteration
          if (settled_node.find(j.first.first)==settled_node.end()&&settled_node.find(j.first.second)==settled_node.end()){
	    if (i.first.first == j.first.second && node_count[i.first.first]==2){
	      to_be_erased.insert(i.first);
	      to_be_erased.insert(j.first);
              to_be_added[Graph::Edge(0, i.first, 0)] = std::make_pair(j.second.first, i.second.second + j.second.second);
	      settled_node.insert(i.first.first);
	      settled_node.insert(i.first.second);
	      settled_node.insert(j.first.first);
	    }
	    if (i.first.second == j.first.first && node_count[j.first.first]==2){
	      to_be_erased.insert(i.first);
	      to_be_erased.insert(j.first);
              to_be_added[Graph::Edge(0, std::make_pair(i.first.first, j.first.second), 0)] = std::make_pair(i.second.first, i.second.second + j.second.second);
	      settled_node.insert(i.first.first);
	      settled_node.insert(i.first.second);
	      settled_node.insert(j.first.second);
	    }
	  }
	}
      }
    }
  return std::make_pair(to_be_added, to_be_erased);
}

void refine(std::map<std::pair<Graph::Node *,Graph::Node *>, std::pair<Graph::Edge* ,double> > & resistance, std::map<Graph::Edge, std::pair<Graph::Edge*, double>> & seq_res, std::map<std::pair<Graph::Node *, Graph::Node *>, double> & par_res){
  int valid = 0;
  while (valid != resistance.size()){
    valid = resistance.size();
    std::pair< std::map<Graph::Edge, std::pair<Graph::Edge*, double> >, std::set<std::pair<Graph::Node*,Graph::Node*> > > add_erase_pair = single_refine(resistance); 
    for (auto i : add_erase_pair.second)
    resistance.erase(i);
    for (auto i : add_erase_pair.first){
      seq_res[Graph::Edge(seq_res.size()+1, i.first.ends(), 0)] = i.second;
      if (resistance.find(i.first.ends()) == resistance.end()){
        resistance[i.first.ends()] = i.second;
      }
      else{
        par_res[i.first.ends()] = get_par_resist(i.second.second, resistance[i.first.ends()].second);
        resistance[i.first.ends()].second = par_res[i.first.ends()];        
      }
    }
  }
}

double get_seq_resist(double length){
  return (12 * VISCOSITY * length) / (HEIGHT * HEIGHT * HEIGHT * WIDTH);
}

double get_par_resist(double r1, double r2){
  return (r1 * r2)/(r1 + r2);
}

