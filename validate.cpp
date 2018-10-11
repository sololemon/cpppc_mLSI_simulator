#include "graph.h"
#include "validate.h"
#include<map>

Graph validate(std::map<int, Graph::Node *> & blocked_nodes, Graph & g){
  Graph valid_graph = initialize(blocked_nodes, g);
  delete_invalid_branch(valid_graph);
  delete_intermediate_node(valid_graph);
  refine_edges(valid_graph);
  guide_direction(valid_graph);
  return valid_graph;
}

Graph initialize(std::map<int, Graph::Node *> & blocked_nodes, Graph & g){
  Graph valid_g;
  for (auto i : g.nodes()){
    if (blocked_nodes.find(i.first) == blocked_nodes.end()){
      valid_g.nodes()[i.first] = Graph::Node(i.first, i.second.type());
      for (auto j : i.second.connections())
	if (blocked_nodes.find(j->ends().first->index()) == blocked_nodes.end()
	 && blocked_nodes.find(j->ends().second->index()) == blocked_nodes.end())
	  valid_g.nodes()[i.first].connect(j);
    }
  }
  return valid_g;
}

void delete_invalid_branch(Graph & g){
  std::set<int> to_be_erased_branch_index;
  for (auto & i : g.nodes())
    if (i.second.type() == 'b' && i.second.connections().size() == 1)
      to_be_erased_branch_index.insert(i.first);
  for (auto & i : to_be_erased_branch_index){
    for (auto & j: g.nodes()[i].connections()){
      g.nodes()[j->ends().first->index()].disconnect(j);
      g.nodes()[j->ends().second->index()].disconnect(j);
    }
    g.nodes().erase(i);
  }
}

void delete_intermediate_node(Graph & g){
  int valid = 0;
  while (valid != g.nodes().size()){
    valid = g.nodes().size();
    std::set<int> to_be_erased_node_index;
    for (auto & i : g.nodes()){
      if (i.second.connections().size() == 2){
	to_be_erased_node_index.insert(i.first);
	// save the connections of the to-be-erased node as edge1 and edge2
	int count_temp = 1;
	Graph::Edge * edge1, * edge2;
	for (auto & k : i.second.connections()){
	  if (count_temp == 1){
	    count_temp++;
	    edge1 = k;
	    continue;
	  }
	  edge2 = k;
	}
	// merge edge1 and edge2 as a new edge and store the new edge in edges[edges.size()+1];
	int temp_index = g.edges().size() + 1;
	g.edges()[temp_index] = merge(temp_index, *edge1, *edge2);
	// denote the indices of the ends of the new edge as index1 and index 2
	int index1 = g.edges()[temp_index].ends().first->index();
	int index2 = g.edges()[temp_index].ends().second->index();
	// connect valid_nodes[index1] and valid_nodes[index2] to the new edge
	g.nodes()[index1].connect(&g.edges()[temp_index]);
	g.nodes()[index2].connect(&g.edges()[temp_index]);
	// disconnect the merged edges from their ends
	g.nodes()[edge1->ends().first->index()].disconnect(edge1);
	g.nodes()[edge1->ends().second->index()].disconnect(edge1);
	g.nodes()[edge2->ends().first->index()].disconnect(edge2);
	g.nodes()[edge2->ends().second->index()].disconnect(edge2);
      }
    }
    for (auto i : to_be_erased_node_index)
      g.nodes().erase(i);
  }
}

void refine_edges(Graph & g){
  g.edges().clear();
  std::set<Graph::Edge *> related_edges = get_related_edges(g.nodes().begin(),g.nodes().end());
  int count = 1;
  for (auto i : related_edges){
    g.edges()[count] = *i;
    count++;
  }
}

void guide_direction(Graph & g){
  int departure = 0;
  for (auto i : g.nodes())
    if (i.second.type() == 'i'){
      departure = i.second.index();
      break;
    }
  std::map<int, int> node_level;
  node_level[departure] = 0;
  while (node_level.size() != g.nodes().size())
    for (auto i : g.nodes())
      for (auto j : i.second.neighbors())
        if (node_level.find(i.first)==node_level.end() && node_level.find(j->index())!=node_level.end())
          node_level[i.first] = node_level[j->index()] + 1;
 // If the level of the second node in an edge is higher than the level of the first node, exchange the nodes
  std::set<Graph::Edge *> related_edges = get_related_edges(g.nodes().begin(), g.nodes().end());
  for (auto & i : related_edges)
    if (node_level[i->ends().first->index()] > node_level[i->ends().second->index()])
      i->swap_ends();
  for (auto & i : g.edges())
    if (node_level[i.second.ends().first->index()] > node_level[i.second.ends().second->index()])
      i.second.swap_ends();
  // Add the edges ending at node i to i.from, add the edges starting from node i to i.to
  for (auto & i : g.edges()){
    g.nodes()[i.second.ends().first->index()].connect_out(&(i.second));
    g.nodes()[i.second.ends().second->index()].connect_in(&(i.second));
  }
}


Graph::Edge merge(int index, Graph::Edge & edge1, Graph::Edge & edge2){
  if (edge1.ends().first == edge2.ends().first){
    Graph::Node * a = edge1.ends().second;
    Graph::Node * c = edge2.ends().second;
    return Graph::Edge(index, a, c, edge1.length() + edge2.length());  
  }
  if (edge1.ends().second == edge2.ends().first){
    Graph::Node * a = edge1.ends().first;
    Graph::Node * c = edge2.ends().second;
    return Graph::Edge(index, a, c, edge1.length() + edge2.length());  
  }
  if (edge1.ends().first == edge2.ends().second){
    Graph::Node * a = edge1.ends().second;
    Graph::Node * c = edge2.ends().first;
    return Graph::Edge(index, a, c, edge1.length() + edge2.length());  
  }
  if (edge1.ends().second == edge2.ends().second){
    Graph::Node * a = edge1.ends().first;
    Graph::Node * c = edge2.ends().first;
    return Graph::Edge(index, a, c, edge1.length() + edge2.length());  
  }
}

template<typename Iterator>
std::set<Graph::Edge *> get_related_edges(Iterator begin, Iterator end){
  std::set<Graph::Edge *> related_edges;
  for (auto i = begin; i != end; i++)
    for (auto & j : i->second.connections())
      related_edges.insert(j);
  return related_edges;
} 
