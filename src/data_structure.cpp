#include "data_structure.hpp"



double get_cross(Vertex &piv,Vertex &u,Vertex &v)
{
  // double mag1 = hypot(u.x-piv.x,u.y-piv.y);
  // double mag2 = hypot(v.x-piv.x,v.y-piv.y);
  // double cosval = ((u.x-piv.x)*(v.x-piv.x)+(u.y-piv.y)*(v.y-piv.y))/(mag1*mag2);
  double cp = (piv.x-u.x)*(v.y-piv.y) - (piv.y-u.y)*(v.x-piv.x);
  return cp;
}

void DCEL :: make_monotone()
{
    priority_queue<Vertex*,vector<Vertex*>,VertexPtrComp>Q;
    set<Edge*,EdgePtrComp> Tau;
    for(int i=0;i<n;++i){
      Q.push(vertices[i]);
    }
    while(!Q.empty()){
      Vertex *v = Q.top();Q.pop();
      int type = v->type;
      if(type == START){
        // start
        handle_start(v,Tau);
      }
      else if(type == END){
        // end
        handle_end(v,Tau);
      }
      else if(type == REGULAR){
        //regular
        handle_regular(v,Tau);
      }
      else if(type == SPLIT){
        //split
        handle_split(v,Tau);
      }
      else if(type == MERGE){
        //merge
        handle_merge(v,Tau);
      }
    }
     for(auto it:faces){
       ymonotones.push_back(it);
     }
}

void DCEL :: handle_regular(Vertex* v,set<Edge*,EdgePtrComp> &Tau)
{
    Edge *ei = v->incident_edge;
    Vertex *w = ei->next->origin;
    if(*v < *w){
      // to its right
      Vertex *helper = ei->get_helper();
      if(helper->type == MERGE){
        add_diagnol(helper,v);
      }
      Tau.erase(ei);
      Tau.insert(ei->prev);
      ei->prev->set_helper(v);
    }
    else{
      // to its left
      Edge* ej = get_support(v,Tau);
      Vertex *helper = ej->get_helper();
      if(helper->type == MERGE){
        add_diagnol(helper,v);
      }
      ej->set_helper(v);
    }
    return;
}

void DCEL :: handle_merge(Vertex *v,set<Edge*,EdgePtrComp> &Tau)
{
    Edge *ei = v->incident_edge;
    Vertex *helper = ei->get_helper();
    if(helper->type == MERGE){
      add_diagnol(helper,v);
    }
    Tau.erase(ei);
    Edge *ej = get_support(v,Tau);
    helper = ej->get_helper();
    if(helper->type == MERGE){
      add_diagnol(helper,v);
    }
    ej->set_helper(v);
    return;
  }

void DCEL :: handle_split(Vertex *v,set<Edge*,EdgePtrComp> &Tau)
{
    Edge *ej = get_support(v,Tau);
    // get helper of this edge
    Vertex *helper = ej->get_helper();
    // add diagnol between helper and v
    add_diagnol(helper,v);
    // set helper of ej to be v now
    ej->set_helper(v);
    Edge *ep = v->incident_edge->prev;
    Tau.insert(ep);
    ep->set_helper(v);
    return;
}

void DCEL :: add_diagnol(Vertex *helper, Vertex *v,bool TRI = 0)
  {
    if(TRI){
      fout2<<setprecision(16)<<helper->x<<' '<<helper->y<<' '<<v->x<<' '<<v->y<<'\n'; 
      return;
    }
    else{
      fout1<<setprecision(16)<<helper->x<<' '<<helper->y<<' '<<v->x<<' '<<v->y<<'\n'; 
      // return;
    }
      Face *nf1 = new Face;
      Face *nf2 = new Face;

      Edge *edg = new Edge;
      Edge *tw = new Edge;

      // if(helper->x < v->x){
          edg->origin = helper; 
          tw->origin = v;
      // }else{
      //     edg->origin = v; 
      //     tw->origin = helper;
      // }

      tw->twin = edg;edg->incident_face = nf1;
      tw->incident_face = nf2;

      edg->twin = tw;

      nf1->ptr = edg; nf2->ptr = tw;
      

      vector<Vertex*>::iterator it1,it2;
      Edge *lala,*lala2;
      Edge *lite,*lite2;
      for(int i=1;i<(int)faces.size();++i){
          
          it1 = find ((faces[i]->vertices).begin(), (faces[i]->vertices).end(), helper);
          it2 = find ((faces[i]->vertices).begin(), (faces[i]->vertices).end(), v);
          
          if (it1 != (faces[i]->vertices).end() && it2 != (faces[i]->vertices).end()){
              lala = helper->incident_edge;
              lala2 = v->incident_edge;
              
              while(lala->incident_face != faces[i]){
                  lala = lala->twin->next;
              }
              // cout<<lala->origin->x<<' '<<lala->origin->y<<'\n';
              while(lala2->incident_face != faces[i]){
                  lala2 = lala2->twin->next;
              }
              // cout<<lala2->origin->x<<' '<<lala2->origin->y<<'\n';
              lite = lala->prev;lite2 = lala2->prev;
              
              lala2->prev = edg; edg->next = lala2; lite->next = edg; edg->prev = lite;
              lala->prev = tw; tw->next = lala; lite2->next = tw; tw->prev = lite2;

              lite = lala;
              
              do{
                  (nf2->vertices).push_back(lite->origin);
                  lite->incident_face = nf2;
                  lite = lite->next;
              }while(lite != lala);

              lite = lala2;
              do{
                  (nf1->vertices).push_back(lite->origin);
                  lite->incident_face = nf1;
                  lite = lite->next;
              }while(lite != lala2);

              faces.erase(faces.begin() + i);
              faces.push_back(nf1);
              faces.push_back(nf2);

              edges.push_back(edg);
              edges.push_back(tw);
              break;
          }
      }
      return;
}

void DCEL :: handle_end(Vertex *v,set<Edge*,EdgePtrComp> &Tau)
{
    Edge *ei = v->incident_edge;
    Edge *ep = v->incident_edge->prev;
    Vertex *helper = ei->get_helper();
    if(helper->type == MERGE){
      // create a diagnol which means creating a new face
      add_diagnol(helper,v);
    }
    Tau.erase(ei);
    return;
}

void DCEL :: handle_start(Vertex *v,set<Edge*,EdgePtrComp> &Tau)
{
    Edge *ep = v->incident_edge->prev;
    Tau.insert(ep);
    ep->set_helper(v);
    return;
}

Edge* DCEL :: get_support(Vertex *v,set<Edge*,EdgePtrComp> &Tau)
  {
    //make a "point" edge
    Edge *temp = new Edge;
    temp->next = temp->prev = temp;
    temp->origin = v;
    // search for the nearest edge to the left of this point edge
    set<Edge*,EdgePtrComp>::iterator it = Tau.upper_bound(temp);
    --it;
    delete temp;
    return *it;
  }

void DCEL :: triangulate(){

  // triangulate each monotone piece
  for(int i=1;i<(int)ymonotones.size();++i){
    
    Face *f = ymonotones[i];
    Edge *es = f->ptr;
    Edge *tra = es;
    vector<Edge*>chain_left,chain_right;
    Edge *top_edge = es;
    Edge *bottom_edge = es;
    Vertex *mxv = es->origin,*miv = es->origin;
    
    // bottom most and top most vertices
    while(1){
      if(*tra->origin < *miv){
        miv = tra->origin;
        bottom_edge = tra;
      }
      if(*mxv < *tra->origin){
        mxv = tra->origin;
        top_edge = tra;
      }
      tra = tra->next;
      if(tra == es){
        break;
      }
    }

    // seperate into 2 chains, as edges
    tra = top_edge;
    while(1){
      chain_left.push_back(tra);
      if(tra == bottom_edge)break;
      tra = tra->next;
    }
    tra = bottom_edge;
    while(tra != top_edge){
      if(tra != bottom_edge){
        chain_right.push_back(tra);
      }
      tra = tra->next;
    }
    reverse(chain_right.begin(),chain_right.end());

    // merge start
    vector<pair<Edge*,int> >merged;
    int l = 0;
    int r = 0;
    while(l<chain_left.size() and r<chain_right.size()){
      if(*chain_left[l]->origin < *chain_right[r]->origin){
        merged.push_back(make_pair(chain_right[r++],1));
      }
      else{
        merged.push_back(make_pair(chain_left[l++],0)); 
      }
    }
    while(l<chain_left.size()){
      merged.push_back(make_pair(chain_left[l++],0));
    }
    while(r<chain_right.size()){
      merged.push_back(make_pair(chain_right[r++],1)); 
    }

    //start triangulation
    stack<pair<Edge*,int> >S;
    S.push(merged[0]);
    S.push(merged[1]);
    int sz = merged.size();
    for(int j=2;j<=sz-2;++j){
      pair<Edge*,int>tp = S.top();
      pair<Edge*,int>uj = merged[j];
      if(tp.second != merged[j].second){
        while(!S.empty()){
          pair<Edge*,int>cur = S.top();S.pop();
          if(S.empty())continue;
          add_diagnol(cur.first->origin,merged[j].first->origin,1);
        }
        S.push(merged[j-1]);
        S.push(merged[j]);
      }
      else{
        pair<Edge*,int>lpop = S.top();S.pop();
        while(!S.empty()){
          pair<Edge*,int>tp = S.top();
          if(uj.second != tp.second){
            if(!uj.second){
              if(get_cross(*lpop.first->origin,*uj.first->origin,*tp.first->origin)<0){
                add_diagnol(tp.first->origin,uj.first->origin,1);
                lpop = S.top();S.pop();      
              }
              else break;
            }
            else{
              if(get_cross(*lpop.first->origin,*uj.first->origin,*tp.first->origin)>0){
                add_diagnol(tp.first->origin,uj.first->origin,1);      
                lpop = S.top();S.pop();
              }
              else break;
            }
          }
          else{
            if(!uj.second){
              if(get_cross(*lpop.first->origin,*uj.first->origin,*tp.first->origin)<0){
                add_diagnol(tp.first->origin,uj.first->origin,1);
                lpop = S.top();S.pop();
              }
              else break;
            }
            else{
              if(get_cross(*lpop.first->origin,*uj.first->origin,*tp.first->origin)>0){
                add_diagnol(tp.first->origin,uj.first->origin,1);      
                lpop = S.top();S.pop();
              }
              else break;
            }
          }
        }
        S.push(lpop);
        S.push(merged[j]);
      }
    }
    bool temp = 0;
    while(!S.empty()){
      pair<Edge*,int>tp = S.top();S.pop();
      if(!temp){
        temp = 1;
        continue;
      }
      if(S.empty())continue;
      add_diagnol(tp.first->origin,merged[sz-1].first->origin,1);
    }
  }
  return;
}




