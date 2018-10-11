#include<iostream>
#include<fstream>
#include<sstream>
#include "graph.h"
#include "construct.h"

Graph construct(std::string filename){
  Graph g;
  read_design(filename, g);
  connect(g);
  return g;
}

void read_design(std::string filename, Graph & g){
  std::ifstream input(filename);
  std::string str_temp;
  int int_temp, int_temp1, int_temp2;
  double double_temp;
  while (input >> str_temp){
    // read Inlets
    if (str_temp == "Inlet:"){
      while (input >> str_temp){
	if (str_temp == "end")
	  break;
	int_temp = std::stoi(str_temp);
	g.nodes().insert({int_temp, Graph::Node(int_temp, 'i')});
      }
    }
    // read Outlets
    if (str_temp == "Outlet:"){
      while (input >> str_temp){
	if (str_temp == "end")
	  break;
	int_temp = std::stoi(str_temp);
	g.nodes()[int_temp] = Graph::Node(int_temp,'o');
      }
    }
    // read Branches
    if (str_temp == "Branch:"){
      while (input >> str_temp){
	if (str_temp == "end")
	  break;
        int_temp = std::stoi(str_temp);
	g.nodes()[int_temp] = Graph::Node(int_temp,'b');
      }
    }
    // read Valves
    if (str_temp == "Valve:"){
      while (input >> str_temp){
	if (str_temp == "end")
	  break;
	int_temp = std::stoi(str_temp);
	g.nodes()[int_temp] = Graph::Node(int_temp,'v');
      }
    }
    // read Edges
    if (str_temp == "Edge:"){
      int index_temp = 0;
      while (std::getline(input, str_temp)){
	if (str_temp == "end")
	  break;
        if (str_temp.empty())
	  continue;
	index_temp++;
	std::istringstream edge_record(str_temp);
	edge_record >> int_temp1 >> int_temp2 >>double_temp;
	g.edges()[index_temp] = Graph::Edge(index_temp, &g.nodes()[int_temp1], &g.nodes()[int_temp2], double_temp);
      }
    }
    // read Control Channels
    if (str_temp == "CChannel:"){
      int id_temp = 0;
      int v_id_temp = 0;
      while (std::getline(input, str_temp)){
	if (str_temp == "end")
	  break;
	if (str_temp.empty())
	  continue;
	std::istringstream cchannel_record(str_temp);
	if (cchannel_record >> id_temp){
	  g.cchannels()[id_temp] = Graph::CChannel(id_temp);
	  while (cchannel_record >> v_id_temp)
	    g.cchannels()[id_temp].set_cValves(&g.nodes()[v_id_temp]);
	}
      }
    }
  }
}

void connect(Graph & g){
  for (auto & i: g.edges()){
    i.second.ends().first->connect(&g.edges()[i.second.index()]);
    i.second.ends().second->connect(&g.edges()[i.second.index()]);
  }
}
