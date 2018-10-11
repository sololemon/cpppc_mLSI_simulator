#include<iostream>
#include<fstream>
#include<tuple>
#include "graph.h"
#include "record.h"

void output(Graph & g, std::tuple<std::map<Graph::Edge, std::pair<Graph::Edge *, double>>, std::map<std::pair<Graph::Node*,Graph::Node*>, double>, double> & seq_par_res, std::pair<std::map<Graph::Edge*, double>, double> & flow_collection, std::map<Graph::Edge *, double> & utilization, double time){
  std::ofstream output("output_info.txt");
  output_path(output, g);
  output_parallel_path(output, seq_par_res); 
  output_flow_rate_distribution(output, flow_collection, g);
  output_channel_utilization(output, g, utilization, time);
  std::cout<<"Please open the following file to see the results: output_info.txt ."<<std::endl;
}

void output_path(std::ofstream & output, Graph & g){
  output<<"Valid nodes include: "<<std::endl;
  for (auto i : g.nodes()){
    output<<i.second.index()<<", which is connected with the following edges: ";
    for (auto & j : i.second.in())
      output<<"("<<j->ends().first->index()<<", "<<j->ends().second->index()<<") ";
    for (auto & j : i.second.out())
      output<<"("<<j->ends().first->index()<<", "<<j->ends().second->index()<<") ";
    output<<std::endl;
  }
  output<<"--------------------"<<std::endl;
}

void output_parallel_path(std::ofstream & output, std::tuple<std::map<Graph::Edge, std::pair<Graph::Edge *, double>>, std::map<std::pair<Graph::Node*,Graph::Node*>, double>, double> & seq_par_res){
  output<<"The following flow paths consist of parallel edges:"<<std::endl;
  for (auto & i : std::get<1>(seq_par_res)){
    output<<"Path ("<<i.first.first->index()<<", "<<i.first.second->index()<<")."<<std::endl;
    output<<"The resistance of the parallel edges are:";
    for (auto & j : std::get<0>(seq_par_res))
      if (j.first.ends() == i.first)
	output<<" "<<j.second.second;
    output<<"."<<std::endl; 
    output<<"The total resistance of this path is "<<i.second<<std::endl;
  }
  output<<"--------------------"<<std::endl;
}

void output_flow_rate_distribution(std::ofstream & output, std::pair<std::map<Graph::Edge*, double>, double> & flow_collection, Graph & g){
   for (auto & i : g.nodes())
     for (auto & j : i.second.out()){
       if (flow_collection.first.find(j)==flow_collection.first.end())
         output<<"Edge ("<<j->ends().first->index()<<", "<<j->ends().second->index()<<") inherits "<<100<<"\% of the flow_rate from its predecessor channels."<<std::endl;
       else
         output<<"Edge ("<<j->ends().first->index()<<", "<<j->ends().second->index()<<") inherits "<<flow_collection.first[j]*100<<"\% of the flow_rate from its predecessor channels."<<std::endl;
     }
  output<<"--------------------"<<std::endl; 
}

void output_channel_utilization(std::ofstream & output, Graph & g, std::map<Graph::Edge *, double> & utilization, double time){
  for (auto & i : g.nodes())
    for (auto & j : i.second.out())
      if (utilization[j] != 0)
	output<<"At "<<time<<" seconds, Edge ("<<j->ends().first->index()<<", "<<j->ends().second->index()<<") is "<<utilization[j]*100<<"\% occupied."<<std::endl;
}
