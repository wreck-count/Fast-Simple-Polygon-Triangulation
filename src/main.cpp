/** @file */

#include "utils.hpp"
#include "data_structure.hpp"


int main(){//FAST;
ifstream fin("pgn.dat");

DCEL D;
double x,y;
Face* outer_face = new Face;
Face* inner_face = new Face;

while(fin>>x){
  fin>>y;
  Vertex* v = new Vertex(x,y);
  D.vertices.push_back(v);
  inner_face->vertices.push_back(v);
}
D.n = D.vertices.size();
// split - next and prev below this and interior angle greater than pi
// merge - next and prev above this and interior angle greater than pi
// regular - next/prev above and prev/next below
// start - next and prev below and int_ang less than pi
// end - next and prev above and int_ang less than pi
// type determination below
int n = D.n;
for(int i=0;i<n;++i){
  Vertex *vp = D.vertices[(i-1+n)%n],*vn = D.vertices[(i+1)%n],*v = D.vertices[i];
  double ang = get_cross(*v,*vp,*vn);
  if(*vp < *v and *vn < *v){
    if(ang>=0){
      // split
      v->type = SPLIT;
      // cout<<"SPLIT"<<'\n';
    }
    else{
      // start
      v->type = START;
      // cout<<"START"<<'\n';
    }
  }
  else if(*v < *vp and *v < *vn){
    if(ang>=0){
      // merge
      v->type = MERGE;
      // cout<<"MERGE"<<'\n';
    }
    else{
      // end
      v->type = END;
      // cout<<"END"<<'\n';
    }
  }
  else{
    // regular
    v->type = REGULAR;
    // cout<<"REGULAR"<<'\n';
  }
}

cout<<"\nDUPES O CLOCK\n";



for(int i=0;i<n;++i){
  Edge *e = new Edge;
  Edge *e_twin = new Edge;
  e->twin = e_twin;
  e_twin->twin = e;
  e->next = e->prev = nullptr;
  e_twin->next = e_twin->prev = nullptr;
  e->origin = D.vertices[i];
  e->target = D.vertices[(i+1)%n];
  e_twin->origin = D.vertices[(i+1)%n];
  e_twin->target = D.vertices[i];
  e->incident_face = outer_face;
  e_twin->incident_face = inner_face;
  D.vertices[i]->incident_edge = e;
  D.edges.push_back(e);
  // D.edges.push_back(e_twin);
}

outer_face->ptr = D.edges[0];
inner_face->ptr = D.edges[0]->twin;
D.faces.push_back(outer_face);
D.faces.push_back(inner_face);
for(int i=0;i<n;++i){
  D.edges[i]->next = D.edges[(i+1)%n];
  D.edges[i]->prev = D.edges[(i-1+n)%n];
  D.edges[i]->twin->next = D.edges[(i-1+n)%n]->twin;
  D.edges[i]->twin->prev = D.edges[(i+1)%n]->twin;
}

// cout<<"BIM BUM\n";
std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
cout<<"MONOTONE EDGES \n";
D.make_monotone();
cout<<"TRIANGULATION EDGES \n";
D.triangulate();
std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
// cout<<D.ymonotones.size()<<'\n';
// for(auto it:D.faces){
//  cout<<"Face  \n";
//  // for(auto is:it->vertices){
//  //  cout<<is->x<<' '<<is->y<<'\n';
//  // }
//  Edge *es = it->ptr;
//  Edge *tra = es;
//  while(1){
//    cout<<tra->origin->x<<' '<<tra->origin->y<<'\n';
//    tra = tra->next;
//    if(tra == es)break;
//  }
// }
cout<<"TIME TAKEN : "<<std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "[ns]" << std::endl;
fin.close();
return 0;
}
