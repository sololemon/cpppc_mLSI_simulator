#ifndef PRESSURIZE_H
#define PRESSURIZE_H

#include<map>

std::map<int, Graph::Node *> pressurize(std::string, Graph &);
void read_protocols(std::string, Graph &);
void block_nodes(std::map<int, Graph::Node *> &, Graph &);



#endif

