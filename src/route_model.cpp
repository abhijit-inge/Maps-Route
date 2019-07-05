#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
int i=0;
for(Model::Node mNode:this->Nodes()){
  m_Nodes.push_back(Node(i, this, mNode));
}
 CreateNodeToRoadHashmap();
}
// float RouteModel::Node::distance (RouteModel::Node node) const{
// int x=this->x;
// int y=this->y;
// int nodex=node.x;
// int nodey=node.y;
// return sqrt((x-nodex)^2+(y-nodey^2));

// }
  void RouteModel::CreateNodeToRoadHashmap(){
    for(const Model::Road &road:Roads()){
      if(road.type!=Model::Road::Type::Footway){
        for(int wayNodes:Ways()[road.way].nodes){
          if(node_to_road.find(wayNodes)==node_to_road.end()){
            node_to_road[wayNodes]=std::vector<const Model::Road *>();

          }
          node_to_road[wayNodes].push_back(&road);
        }
      }
    }

  }
RouteModel::Node *RouteModel::Node::FindNeighbor(std::vector<int> node_indices){
Node *closest_node=nullptr;
Node node;
for(int node_index:node_indices){
  node=parent_model->snodes()[node_index];
  if(this->distance(node)!=0 &&!node.visited){
    if(closest_node==nullptr||this->distance(node)<this->distance(*closest_node)){
      closest_node=&parent_model->snodes()[node_index];

    }

  }
  return closest_node;
}
}
void RouteModel::Node::FindNeighbors(){
  for(auto & road:parent_model->node_to_road[this->index]){
    RouteModel::Node *new_neighbor=this->FindNeighbor(parent_model->Ways()[road->way].nodes);
    if(new_neighbor){
      this->neighbors.push_back(*new_neighbor);

    }
  }
}

RouteModel::Node &RouteModel::FindClosestNode(float x, float y){
  Node input;
  input.x=x;
  input.y=y;
  float min_dist=std::numeric_limits<float>::max();
  float dist;
  int closest_idx;
  for(const Model::Road &road:Roads()){
    if(road.type!=Model::Road::Type::Footway){
      for(int node_idx:Ways()[road.way].nodes){
        dist=input.distance(snodes()[node_idx]);
        if(dist<min_dist)
{
  closest_idx=node_idx;
  min_dist=dist;

}      }
    }
  }
return snodes()[closest_idx];
}