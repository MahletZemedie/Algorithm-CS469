

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <set>
#include <stack>
#include <queue>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std;

int clock1=1;
int clock2=1;
size_t c=0;

class Node {
  
private:
  string m_name; //a string that labels the node
  int    m_id; //a unique integer from 0 to n-1, where n is the total number of nodes.
  int m_pre;
  int m_post;
  bool m_visited=false;
  
public:
  
  Node() {};
  Node(const string & name, int id) {m_name = name, m_id = id;};
  int id() const {return m_id;};
  const string & name() const {return m_name;};
  int pre() const {return m_pre;};
  int post() const {return m_post;};
  int visited() const {return m_visited;};
  void assignPre(int pre){ m_pre=pre;}
  void assignPost(int post){ m_post=post;}
  void assignVisited(bool visited) { m_visited=visited;};
};


class Graph {
  
private:
  vector< Node > m_nodes;
  vector< list<Node> > m_adjList;
  vector< bool > m_visit;
  vector< int > m_pre;
  vector< int > m_post;
  vector<size_t> m_ccnum;
  vector<size_t> m_dist;
public:
  
  //Construct the graph from a file of edges
  Graph(const string & file,const string & file1){
    scan(file);
    save(file1);
  };
  void setSize(){
    m_visit.resize(m_nodes.size());
    m_pre.resize(m_nodes.size());
    m_post.resize(m_nodes.size());
     m_dist.resize(m_nodes.size());
    m_ccnum.resize(m_nodes.size());
  }
  //Insert an edge (a,b) to m_adjList
  void addEdge(const Node & a, const Node & b){
    m_adjList.resize(m_nodes.size());
    m_adjList[a.id()].push_back(b);
  }
  
  //Insert a node a to m_nodes
  void addNode(const Node & a) {
    m_nodes.push_back(a);
    
    
  };
  
  void setvisit(int id,bool visit){
    
    m_visit[id]=visit;
  }
  
  void  setpre(int id,int pre){
    
    m_pre[id]=pre;
    clock1=pre+1;
  }
  
  void  setpost(int id,int post){
    
    m_post[id]=post;
    clock1=post+1;
  }
   void setDist(int id,int dist){
    
    m_dist[id]=dist;
  }
  void setCCnum(int id,size_t s){
    
    m_ccnum[id]=s;
  }
  const int  getDist(int id) const { return m_dist[id]; }
  const size_t  getCCnum(int id) const { return m_ccnum[id]; }
  
  const vector<size_t>  getCCnumV() const { return m_ccnum; }
  
  const bool  getVisited(int id) const { return m_visit[id]; }
  
  const int  getPre(int id) const { return m_pre[id]; }
  
  const int  getPost(int id) const { return m_post[id]; }
  
  const vector<int>  getPostV() const { return m_post; }
  
  const vector<Node>  getNodes() const { return m_nodes; }
  
  //Return node with id equal to i
  const Node & getNode(size_t i) const { return m_nodes[i]; }
  
  //Return reference of the adjacency list of node a.
  list <Node> & getAdjNodes(const Node & a) {return m_adjList[a.id()];}
  
  //Return constant reference to adjacency list of node a
  const list<Node> & getAdjNodes(const Node & a) const {return m_adjList[a.id()];}
  
  //Return the total number of nodes in the graph
  size_t num_nodes() const {return m_nodes.size();}
  
  //Create a graph from a tab-separated text edge list file to adjacency lists
  void scan(const string & file){
    
    string line,token;
    set<string> setnodes;
    unordered_map<string,int> hash_nodes;
    
    ifstream myfile;
    int id=0;
    vector<vector<string> > vec;
    
    myfile.open(file.c_str());
    if (myfile.is_open())
    {
      while (getline(myfile,line))
      {  stringstream ss(line);
        string left,right;
        vector<string> rel;
        
        ss>>left;
        ss>>right;
        
        
        if(!left.empty() && !right.empty()){
          rel.push_back(left);
          rel.push_back(right);
          
          vec.push_back(rel);
          setnodes.insert(left);
          setnodes.insert(right);
          
        }
      }
    }
    myfile.close();
    
    for (std::set<string>::iterator it=setnodes.begin(); it!=setnodes.end(); ++it){
      Node    n(*it,id);
      addNode(n);
      hash_nodes.insert({*it,id});
      id++;
    }
    
    for(int i=0;i<vec.size();i++){
      
      vector<string> v=vec[i];
      
      Node n1(v[0],hash_nodes[v[0]]);
      Node n2(v[1],hash_nodes[v[1]]);
      addEdge(n1,n2);
    }
  }
  
  
  //Save a graph from adjacency lists to a tab-separated text edge list file
  void save(const string & file) const{
    ofstream myfile (file);
    if (myfile.is_open())
    {
      for(int i=0;i<m_adjList.size();i++){
        list<Node> adjL=m_adjList[i];
        for (std::list<Node>::iterator it=adjL.begin(); it!=adjL.end(); ++it){
          Node n=*it;
          myfile << m_nodes[i].name();
          myfile << '\t';
          myfile << n.name();
          myfile << '\n';
        }
      }
      myfile.close();
    }
  }
};

void explore(Graph & g, int id){
  
  g.setvisit(id,true);
  g.setpre(id,clock1);
  g.setCCnum(id,c);
  
  list<Node> adj=g.getAdjNodes(g.getNode(id));
  
  for (std::list<Node>::iterator it=adj.begin(); it!=adj.end(); ++it){
    Node nn=*it;

    int id=nn.id();
    bool b=g.getVisited(id);
    if(b!=true){
      explore(g,nn.id());
    }
  }
  g.setpost(id,clock1);
}
void DFS_recursive(Graph & g){
  
  g.setSize();
  int no=g.num_nodes();
  for(int i=0;i<no;i++){
    bool b=g.getVisited(i);
    if(b!=true){
      explore(g,g.getNode(i).id());
      
    }
  }
}

/////////////////////////////////REVERSE

void exploreR(Graph & g, int id,vector<list<Node> > radj){
  g.setvisit(id,true);
  g.setpre(id,clock1);
  g.setCCnum(id,c);
 
  list<Node> adj=radj[id];
  
  for (std::list<Node>::iterator it=adj.begin(); it!=adj.end(); ++it){
    Node nn=*it;
    
    int id=nn.id();
    bool b=g.getVisited(id);
    if(b!=true){
      exploreR(g,nn.id(),radj);
    }
  }
  g.setpost(id,clock1);
}
void DFS_recursiveR(Graph & g,vector<list<Node> > radj){
  g.setSize();
  int no=g.num_nodes();
  for(int i=0;i<no;i++){
    bool b=g.getVisited(i);
    if(b!=true){
      exploreR(g,g.getNode(i).id(),radj);
      
    }
  }
}


void DFS_iterative(Graph & g){
  
  g.setSize();
  for(int i=0;i<g.num_nodes();i++){
    g.setpost(i,0);
    g.setpre(i,0);
  }
  
  stack<Node> nstack;
  for(int i=0;i<g.num_nodes();i++){
    Node s= g.getNode(i);
    
    nstack.push(s);
    
    while(!nstack.empty()){
      
      Node n=nstack.top();
      nstack.pop();
      if(g.getPost(n.id())!=0){
        
      }
      else if(g.getPre(n.id())!=0){
        
        g.setpost(n.id(),clock1); 
      }
      else if(g.getPre(n.id())==0){
        g.setvisit(n.id(),true);
        g.setpre(n.id(),clock1);
        nstack.push(n);
      }
      list<Node> l=g.getAdjNodes(n);
      for(list<Node>::reverse_iterator itr = l.rbegin(); itr != l.rend(); itr++){
        
        Node nn=*itr;
        
        if(g.getVisited(nn.id())!=true){
          nstack.push(nn);
          
        }
        
      }
      
    }
  }
 
}

void BFS(Graph & g,Node s){
  
  g.setSize();
  for(int i=0;i<g.num_nodes();i++){
    g.setDist(i,-1);
  }
  g.setDist(s.id(),0);
  queue<Node> nQueue;
nQueue.push(s);
 
    
    while(!nQueue.empty()){
      
      Node n=nQueue.front();
      nQueue.pop();
     
      list<Node> l=g.getAdjNodes(n);
      for(list<Node>::iterator itr = l.begin(); itr != l.end(); itr++){
        
        Node nn=*itr;
       
        if(g.getDist(nn.id())==-1){
          nQueue.push(nn);
	  g.setDist(nn.id(),g.getDist(n.id())+1);
          
        }
        
      }
      
    }
  
}
//UNDIRECTED GRAPH
vector<size_t> find_connected_components ( Graph & g)
{  
  //  vector<size_t> ccnum;
  c=0;
  clock1=1;
  //  ccnum.resize(g.num_nodes());
  g.setSize();
  int no=g.num_nodes();
  for(int i=0;i<no;i++){
    bool b=g.getVisited(i);
    if(b!=true){
      
      explore(g,g.getNode(i).id());
      c++;
    }
    
  }
  return g.getCCnumV();
}
//undirected connected components algorithm for the directed
vector<size_t> find_connected_components_D( Graph & g,vector<Node> nodes)
{  c=0;
  clock1=1;
 
  for(int i=0;i<g.num_nodes();i++){
    g.setvisit(i,false);
  }
  
  g.setSize();
  int no=nodes.size();
  for(int i=0;i<nodes.size();i++){
    bool b=g.getVisited(nodes[i].id());
    if(b!=true){
      
      explore(g,nodes[i].id());
      c++;
    }
    
  }
  return g.getCCnumV();
}



vector <size_t> find_strongly_connected_components(Graph & g)
{  c=0;
  clock1=1;
  vector<list<Node> > radj;
  radj.resize(g.num_nodes());
  for(int i=0; i<g.num_nodes();i++){
    list<Node> adj = g.getAdjNodes(g.getNode(i));
    for(list<Node>::const_iterator itr = adj.begin(); itr != adj.end(); itr++){
      Node n=*itr;
      radj[n.id()].push_back(g.getNode(i));
    }
  }
  
  DFS_recursiveR(g,radj);
  

  vector<int> postV=g.getPostV();
  int temp1;
  Node temp2;
  vector<Node> nodes=g.getNodes();
  for(int i=0;i<postV.size();i++){
    for(int j=i;j<postV.size();j++){
      if(postV[i] < postV[j]){
        temp1 = postV[i];
        temp2 = nodes[i];
        postV[i] = postV[j];
        nodes[i] = nodes[j];
        postV[j] = temp1;
        nodes[j] = temp2;
       
      }
    }
  }

  find_connected_components_D(g,nodes);
  

  return g.getCCnumV();
  }

ostream & operator <<(ostream & out, const Graph & g) {
  
  
  out << "Nodes in the graph:" << endl;
  for(unsigned i = 0; i< g.num_nodes(); i++){
    out << "pre,post:" << endl;
    out << "clock" <<g.getVisited(i)<< endl;
    out << g.getPre(i)<< ",,,"<< g.getPost(i) << endl;
    out << g.getNode(i).name() << " , "<<g.getNode(i).id();
    out << "CCnum" <<g.getCCnum(i)<< endl;
    out << endl;
  }
  out << "Adjacency list of the graph:" << endl;
  for(unsigned i = 0; i < g.num_nodes(); i++) {
    out << "Node " << g.getNode(i).name() << ":";
    const list<Node> neighbors = g.getAdjNodes(g.getNode(i));
    for(list<Node>::const_iterator itr = neighbors.begin(); itr != neighbors.end(); itr++)
      out << itr->name() << " ,";
    out << endl;
  }
  return out;
  
}

bool test_small(){
  
  //test 1	
  
  bool correct1=true;
  Graph g1("test1.txt","test1out.txt");
  
  BFS(g1,g1.getNode(2));
  vector<int> dist_r1={2,1,0,1,1};
  for(int i=0;i<g1.num_nodes();i++){
    if(g1.getDist(i)!=dist_r1[i]){
      correct1=false;
    }
  }
  if(correct1){
    cout<<"First test passed\n";
  }
  else{
    cout<<"First test failed\n";
  }
  
  //test 2
  bool correct2=true;
  Graph g2("test2.txt","test2out.txt");
  
 BFS(g2,g2.getNode(0));
  vector<int> dist_r2={0,1,2,3,-1,-1,-1};
  for(int i=0;i<g2.num_nodes();i++){
    if(g2.getDist(i)!=dist_r2[i]){
      correct2=false;
    }
  }
  if(correct2){
    cout<<"Second test passed\n";
  }
  else{
    cout<<"Second test failed\n";
  }
  //cout<<g2;
 
 //test 3
  bool correct3=true;
  Graph g3("test3.txt","test3out.txt");
  
  BFS(g3,g3.getNode(0));
  vector<int> dist_r3={0,1,2,1,2,3,-1,-1,-1};
  for(int i=0;i<g3.num_nodes();i++){
    if(g3.getDist(i)!=dist_r3[i]){
      correct3=false;
    }
  }
  if(correct3){
    cout<<"Third test passed\n";
  }
  else{
    cout<<"Third test failed\n";
  }
  
 
  //test 4
  bool correct4=true;
  Graph g4("test4.txt","test4out.txt");
  
  BFS(g4,g4.getNode(0));
  vector<int> dist_r4={0,1,-1,2,3};
  for(int i=0;i<g4.num_nodes();i++){
    if(g4.getDist(i)!=dist_r4[i]){
      correct4=false;
    }
  }
  if(correct4){
    cout<<"Fourth test passed\n";
  }
  else{
    cout<<"Fourth test failed\n";
  }
  
  
  //test 5
  bool correct5=true;
  Graph g5("test5.txt","test5out.txt");
  
 BFS(g5,g5.getNode(1));
  vector<int> dist_r5={5,0,1,2,3,4};
  for(int i=0;i<g5.num_nodes();i++){
    if(g5.getDist(i)!=dist_r5[i]){
      correct5=false;
    }
  }
  if(correct5){
    cout<<"Fifth test passed\n";
  }
  else{
    cout<<"Fifth test failed\n";
  }

  
  
}

int test_big_unDirected(){
  Graph g_big("test_big.txt","test_bigout.txt");
  BFS(g_big,g_big.getNode(10));
  return 0;
  
}

int test_big_Directed(){
  Graph g_big("test_big.txt","test_bigout.txt");
  BFS(g_big,g_big.getNode(10));
  return 0;
  
}

int main() {
 
  test_small();
  
  
  
}

