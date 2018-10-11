#ifndef GRAPH_H
#define GRAPH_H

#include<set>
#include<map>
#include<vector>
#include<utility>
#include<tuple>
#include<iostream>

#define WIDTH 0.0001
#define HEIGHT 0.0001
#define VISCOSITY 0.00089
#define PRESSURE 100

class Graph;


class Graph{

  public:
    class Node;
    class Edge;

    class Edge{
  
      private:
        int _index;
        std::pair<Node *, Node *> _ends;
        double _length = 0;

      public:

        Edge() = default;

        Edge(int index, Node * a, Node * b, double length) 
          : _index(index), _ends(std::make_pair(a,b)), _length(length){}
        
        Edge(int index, std::pair<Node *, Node *> ends, double length) 
          : _index(index), _ends(ends), _length(length){}

        void swap_ends(){
          Node * temp = _ends.first;
          _ends.first = _ends.second;
          _ends.second = temp;
        }

        const int & index() const{
          return _index;
        }

        const std::pair<Node *, Node *> & ends() const{
          return _ends;
        }

        const double & length() const{
          return _length;
        }

        bool operator ==(const Edge & other) const{
          if (other._index == _index && other._ends == _ends && other._length == _length)
            return true;
          return false;
        }
 
        bool operator <(const Edge & other) const{
          if (other._index < _index)
            return true;
          return false;
        }
    };


    class Node{

      private:
        int _index;
        char _type;
        bool _status;
        std::set<Edge *> _connections;
        std::set<Edge *> _in;
        std::set<Edge *> _out;

      public:
        Node() = default;

        Node(int index, char type) 
          : _index(index), _type(type) {
          switch (_type){
	    case 'i':
	      _status = false;
	      break;
            case 'o':
	      _status = true;
	      break;
	    case 'v':
	      _status = true;
	      break;
	    case 'b':
	      _status = true;
	      break;
          }    
        }

        const int & index() const{
          return _index;
        }

        const char & type() const{
          return _type;
        }

        const bool & status() const{
          return _status;
        }

        std::set<Node *> neighbors(){
          std::set<Node *> output;
          for (auto i:_connections){
	    if (i->ends().first->index() != _index)
	      output.insert(i->ends().first);
	    else
	      output.insert(i->ends().second);
          }
          return output;
        }

        std::set<Edge *> & connections(){
          return _connections;
        }

        std::set<Edge *> & in(){
          return _in;
        }

        std::set<Edge *> & out(){
          return _out;
        }
  
        void connect(Edge * given_edge){
          if (_connections.find(given_edge) == _connections.end())
            _connections.insert(given_edge);
        }

        void disconnect(Edge * given_edge){
          if (_connections.find(given_edge) != _connections.end())
    	    _connections.erase(_connections.find(given_edge)); 
        }

        void connect_in(Edge * given_edge){
          _in.insert(given_edge);
        }

        void connect_out(Edge * given_edge){
          _out.insert(given_edge);
        }

        void open(){
          switch (_type){
	    case 'b': case 'o':
	      std::cerr<<"The status of this node cannot be changed!"<<std::endl;
	      break;
	    case 'i': case 'v':
              _status = true;
              break;
          }
        }

        void close(){
          switch (_type){
	    case 'b': case 'o':
	      std::cerr<<"The status of this node cannot be changed!"<<std::endl;
	      break;
	    case 'i': case 'v':
              _status = false;
              break;
          }
        }
    };

    class CChannel{

      private:
        int _index;
        std::vector<Node * > _cValves;

      public:

        CChannel() = default;

        CChannel(int index) : _index(index){}

        void set_cValves(Node * cValve){
          if (cValve->type()!='v'){
            std::cerr<<"The given node is not a valve!"<<std::endl;
            return;
          }
          _cValves.push_back(cValve);
        }

        void pressurize(){
          for (auto i: _cValves)
	    i->close();
        }

        void depressurize(){
          for (auto i: _cValves)
            i->open();
        }
    };

  private:
    std::map<int, Node> _nodes;
    std::map<int, Edge> _edges;
    std::map<int, CChannel> _cchannels;

  public:
    std::map<int, Node> & nodes(){
      return _nodes;
    }

    std::map<int, Edge> & edges(){
      return _edges;
    }

    std::map<int, CChannel> & cchannels(){
      return _cchannels;
    }

};

#endif
