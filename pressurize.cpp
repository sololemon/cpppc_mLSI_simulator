#include "graph.h"
#include "pressurize.h"

std::map<int, Graph::Node *> pressurize(std::string filename, Graph & g){
  read_protocols(filename, g);
  std::map<int, Graph::Node *> blocked_nodes;
  block_nodes(blocked_nodes, g);
  return blocked_nodes;
}

void read_protocols(std::string filename, Graph & g){
  std::ifstream input(filename);
  std::string str_temp1, str_temp2;
  int int_temp, int_temp1, int_temp2;
  while (std::getline(input, str_temp1)){
    if (str_temp1 == "end")
      continue;
    std::istringstream line_record(str_temp1);
    line_record >> str_temp2;
    if (str_temp2 == "Time:")
      continue;
    if (str_temp2 == "Inlet-on:"){
      while (line_record >> str_temp2){
	int_temp = std::stoi(str_temp2);
	g.nodes()[int_temp].open();
      }  
    }
    if (str_temp2 == "Pressurize:"){
      while (line_record >> str_temp2){
	int_temp = std::stoi(str_temp2);
        g.cchannels()[int_temp].pressurize();
      }  
    }	
  }
}

void block_nodes(std::map<int, Graph::Node *> & blocked_nodes, Graph & g){
  for (auto & i : g.nodes())
    if (!i.second.status())
      blocked_nodes[i.first] = &i.second;
  size_t blocked = 0;
  while (blocked != blocked_nodes.size()){
    blocked = blocked_nodes.size();
    std::set<Graph::Node * > to_be_added;
    for (auto & i : blocked_nodes)
      for (auto & j : i.second->neighbors()){
	if (j->type() == 'b')
	  continue;
	to_be_added.insert(j);
      }
    for (auto & i : to_be_added)
      blocked_nodes[i->index()] = i;
  }
}
