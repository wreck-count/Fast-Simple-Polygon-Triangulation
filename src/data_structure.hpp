/** @file */

#ifndef DATA_STRUCTURE_HPP
#define DATA_STRUCTURE_HPP

#include "utils.hpp"

enum vertex_types{START,END,REGULAR,SPLIT,MERGE};



/** @brief Vertex struct:
 * has a data member type that identifies the vertex type among start, end, regular, split, merge
 * has two data members x and y corresponding to the x and y co-ordinates of the vertex respectively
 * overloads comparison operator
 * */
struct Vertex
{
  int type;
  double x,y;
  struct Edge *incident_edge;
  /** Constructor to initialize the co-ordinates of the vertex */
  Vertex(double x,double y):x(x),y(y){incident_edge = nullptr;}
  bool operator<(const Vertex &rhs){
    if(this->y == rhs.y)
    {
      return this->x>rhs.x;
    }
    return this->y<rhs.y;
  }
};

/** @brief Edge struct:
 * has Edge data members that store pointers to twin, next and prev
 * has Vertex data members that store pointers to origin and helper
 * has a Face data member that points to the incident face
 * */
struct Edge
{
  struct Edge *twin,*next,*prev;
  struct Vertex *origin,*helper,*target;
  struct Face *incident_face;

  /** Function that returns the helper vertex */  
  Vertex *get_helper()
  {
    return helper;
  }

  void set_helper(Vertex* v)
  {
    helper = v;
    return;
  }

  bool operator < (const Edge &rhs)
  {
    Vertex &v1 = *(this->origin);
    Vertex &v2 = *(this->next->origin);
    Vertex &v3 = *(rhs.origin);
    Vertex &v4 = *(rhs.next->origin);
    if(v2.y>=v4.y)
    {
      double x = (v4.y-v2.y)*(v1.x-v2.x)/(v1.y-v2.y)+v2.x;
      return x<v4.x;
    }
    else{
      double x = (v2.y-v4.y)*(v3.x-v4.x)/(v3.y-v4.y)+v4.x;
      return v2.x<x; 
    }
  } 
};

/** @brief Face struct:
 * Contains a pointer to the corresponding Edge
 * */
struct Face
{
  struct Edge *ptr;
  vector<Vertex*>vertices;
};

/** @brief DCEL struct:
 * The main DCEL data structure that is used to represent the regions. Implements handling for different kinds 
 * of vertices through its member functions; Also implements the make-montone function that decomposes the polygon
 * into monotone subcomponents
 * */
struct DCEL
{
  int n;
  vector<Vertex*>vertices;
  vector<Edge*>edges;
  vector<Face*>faces,ymonotones;
  int monotone_edges;
  ofstream fout1,fout2;
  
  DCEL(){
    fout1.open("mne.dat");
    fout2.open("tgn.dat");
  }
  /** @brief EdgePtrComp Struct:
 * Helper struct to compare two edges using the Tau structure
 * */
  struct EdgePtrComp
  {
    bool operator()(const Edge *lhs,const Edge *rhs)const 
    {
      Vertex *v1 = lhs->origin;
      Vertex *v2 = lhs->next->origin;
      Vertex *v3 = rhs->origin;
      Vertex *v4 = rhs->next->origin;
      if(v2->y>=v4->y){
        double x = (v4->y-v2->y)*(v1->x-v2->x)/(v1->y-v2->y)+v2->x;
        return x<v4->x;
      }
      else{
        double x = (v2->y-v4->y)*(v3->x-v4->x)/(v3->y-v4->y)+v4->x;
        return v2->x<x; 
      }
    }
  };

  /** @brief VertexPtrComp Struct:
 * Helper struct to compare two vertex pointers
 * */
  struct VertexPtrComp
  {
    bool operator()(const Vertex *lhs,const Vertex *rhs)const {
      if(lhs->y == rhs->y){
        return lhs->x>rhs->x;
      }
      return lhs->y<rhs->y;
    }
  };

  virtual Edge* get_support(Vertex *v,set<Edge*,EdgePtrComp> &Tau);

  virtual void handle_start(Vertex *v,set<Edge*,EdgePtrComp> &Tau);

  virtual void handle_end(Vertex *v,set<Edge*,EdgePtrComp> &Tau);

  virtual void add_diagnol(Vertex *helper, Vertex *v,bool TRI = 0);

  virtual void handle_split(Vertex *v,set<Edge*,EdgePtrComp> &Tau);
  
  virtual void handle_merge(Vertex *v,set<Edge*,EdgePtrComp> &Tau);

  virtual void handle_regular(Vertex* v,set<Edge*,EdgePtrComp> &Tau);

  virtual void make_monotone();
  
  virtual void triangulate();

};

double get_cross(Vertex &piv,Vertex &u,Vertex &v);

#endif
