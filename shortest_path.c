//////////////////////////////////////////////////////////
//							//
// Program: kong.c					//
// Creator: Hongxin Kong					//
// Currrent Version: v1.0				//
// Function: Implementation of three routing algorithms	//
// Last Update: Nov 24th, 2016				//
// History: 2016-11-24 initial script		//
//							//
//////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sys/resource.h>
using namespace std;

const int VMAX_NUM = 5000;		// constant integer, vertex number of the graph
const int G1_DEGREE = 6;		// constant integer, degree of each vertex in graph type 1
const double G2_PERCENT = 0.2;		// constant double number, percentage of connectivity in graph type 2
const int MAX_WEIGHT = 20000000;	// constant integer, maximum weight value

// Structure edge: structure to represent edges
struct edge {
	int a;
	int b;
	int w;
};
// END of edge

// Class node: basic structure in linked list recording the data of each edge in graph  
class node {
	private:
		int vertex;	// vertex index in graph
		int weight;	// edge weight
		node *next;	// pointer to another node
	public:
		node();		// default constructor
		node(const int v, const int w, node *p);	// parameterized constructor
		node(const node &other);			// copy constructor
		int GetVertex();		// return vertex value
		int GetWeight();		// return weight value
		node* GetNextNode();		// return pointer value
		void UpdateVertex(int v);	// update vertex value
		void UpdateWeight(int w);	// update weight value
		void UpdateNextNode(node *p);	// update pointer value
};

node::node()	// default constructor
{
	vertex=-1;
	weight=-1;
	next=NULL;
}

node::node(const int v, const int w, node *p)	// parameterized constructor 
{
	vertex=v;
	weight=w;
	next=p;
}

node::node(const node &other)	// copy constructor
{
	vertex=other.vertex;
	weight=other.weight;
	next=other.next;
}

int node::GetVertex()	// return vertex value
{
	return vertex;
}

int node::GetWeight()	// return weight value
{
	return weight;
}

node* node::GetNextNode()	// return pointer value
{
	return next;
}

void node::UpdateVertex(int v)	// update vertex value
{
	vertex=v;
}

void node::UpdateWeight(int w)	// update weight value
{
	weight=w;
}

void node::UpdateNextNode(node *p)	// update pointer value
{
	next=p;
}
// END of class node

// Class nodeList: a linked list to represent a sequence of edges connecting to certain vertex in graph
class nodeList {
	friend class graph;	// declaration of friend class
	private:
		node *head;	// header of linked list
		int length;	// length of linked list
	protected:
		void ResetList();	// Reset all data of the list
		node* GetHead();	// return the header
		int GetLength();	// return the length
	public:
		nodeList();	// default constructor
		~nodeList();	// destructor
		nodeList(const nodeList &other);	// copy constructor
		nodeList& operator=(const nodeList &other);	// copy assignment
		node* SearchNode(int v);	// search specified vertex in the list
		bool AddNode(int v, int w); 	// add specified vertex with weights to the list
		bool DeleteNode(int v);		// delete specified vertex from the list
		int GetWeight(int v);		// return weight of specified edge
		void TraverseNodes();		// traverse all nodes of the list
};

void nodeList::TraverseNodes()	// traverse all nodes of the list
{
	for(node* current=head;current!=NULL;current=current->GetNextNode())	{
		if (current==head) {
			cout << current->GetVertex()<<":"<<current->GetWeight();
		} else {
			cout << " -> "<<current->GetVertex()<<":"<<current->GetWeight();
		} 
	}
	cout << endl;
}

nodeList& nodeList::operator=(const nodeList &other)	// copy assignment
{
	if (this==&other) {return *this;}
	ResetList();
	node *newnode, *current, *othercurrent;
	length=other.length;
	if (other.head==NULL) {
		head=NULL;
	} else {
		othercurrent=other.head;
		head= new node(othercurrent->GetVertex(), othercurrent->GetWeight(), NULL);
		othercurrent=othercurrent->GetNextNode();
		current=head;
		while(othercurrent!=NULL) {
			newnode= new node(othercurrent->GetVertex(), othercurrent->GetWeight(), NULL);
			current->UpdateNextNode(newnode);
			othercurrent=othercurrent->GetNextNode();
			current=current->GetNextNode();
		}
	}
	return *this;
}

nodeList::nodeList(const nodeList &other)	// copy constructor
{
	node *newnode, *current, *othercurrent;
	length=other.length;
	if (other.head==NULL) {
		head=NULL;
	} else {
		othercurrent=other.head;
		head= new node(othercurrent->GetVertex(), othercurrent->GetWeight(), NULL);
		othercurrent=othercurrent->GetNextNode();
		current=head;
		while(othercurrent!=NULL) {
			newnode= new node(othercurrent->GetVertex(), othercurrent->GetWeight(), NULL);
			current->UpdateNextNode(newnode);
			othercurrent=othercurrent->GetNextNode();
			current=current->GetNextNode();
		}
	}
}

node* nodeList::GetHead() 	// return the header
{
	return head;
}

nodeList::nodeList()	// default constructor
{
	head=NULL;
	length=0;
}

nodeList::~nodeList()	// destructor
{
	ResetList();
}

void nodeList::ResetList()	// Reset all data of the list
{
	if (head!=NULL) {
		node *current = head;
		node *tmp;
		head=NULL;
		while(current!= NULL) {
			tmp=current;
			current=current->GetNextNode();
			delete(tmp);
		}
		length=0;
	}
}

int nodeList::GetLength()	// return the length
{
	return length;
}

node* nodeList::SearchNode(int v)	// search specified vertex in the list
{
	node *current=head;
	while(current!=NULL) {
		if(current->GetVertex()==v) return(current);
		current=current->GetNextNode();
	}
	return NULL;
}

bool nodeList::AddNode(int v, int w)	// add specified vertex with weights to the list
{
	if(SearchNode(v)==NULL) {
		node *newnode;
		newnode = new node(v, w, head);
		head=newnode;
		length++;
		return true;
	} else {
		return false;
	}
}

bool nodeList::DeleteNode(int v)	// delete specified vertex from the list
{
	node *current, *prev, *next;
	if(SearchNode(v)!=NULL) {
		if(head->GetVertex()==v) {
			current=head;
			head=current->GetNextNode();
			delete(current);	
		} else {
			prev=head;
			current=prev->GetNextNode();
			next=current->GetNextNode();
			while(current->GetVertex()!=v) {
				prev=current;
				current=next;
				next=next->GetNextNode();
			}
			prev->UpdateNextNode(next);
			delete(current);
		}
		length--;
		return true;
	} else {
		return false;
	}
}

int nodeList::GetWeight(int v)	// return weight of specified edge
{
	node* tmp=SearchNode(v);
	if(tmp!=NULL) {
		return tmp->GetWeight();
	} else {
		return -1;
	}
}
// END of class nodeList

//Class graph: data structure to represent directed graph
class graph {
	protected:
		nodeList* list;	// pointer to array of linked lists
		int size;	// total vertex number
	public:
		graph();	// default constructor
		~graph();	// destructor
		graph(const int i);	// parameterized constructor
		graph(const graph &other);	// copy constructor
		bool SearchEdge(int source, int target);	// search edge from source to target in graph
		bool AddEdge(int source, int target, int weight);	// add edge from source to target with weight in graph
		bool DeleteEdge(int source, int target);	// delete edge from source to target in graph
		int GetWeight(int source, int target);		// return weight of edge from source to target
		int GetVertexNum();		// return total vertex number
		int GetEdgeNum(int source);	// return number of edges starting from source
		node* GetEdges(int source);	// return linked list of edges from source
		void CleanAllEdges();		// reset the whole graph
		void TraverseGraph();		// traverse the whole graph
		int GetComMaxWeight(int i, int j);	// return the maximum weight of edges connecting to two vertices
};

int graph::GetComMaxWeight(int i, int j)	// return the maximum weight of edges connecting to two vertices
{
	int max=-1;
	for(node* ptr=GetEdges(i);ptr!=NULL;ptr=ptr->GetNextNode()) {
		if(ptr->GetWeight()>max) {max=ptr->GetWeight();}
	}
	for(node* ptr=GetEdges(j);ptr!=NULL;ptr=ptr->GetNextNode()) {
		if(ptr->GetWeight()>max) {max=ptr->GetWeight();}
	}
	return max;
}

void graph::TraverseGraph()		// traverse the whole graph
{
	for(int i=0;i<size;i++) {
		cout << "Node " << i<<": ";
		list[i].TraverseNodes();
	}
}

graph::graph(const graph &other)	// copy constructor
{
	size=other.size;
	list= new nodeList[size];
	for (int i=0;i<size;i++) {
		list[i]=other.list[i];
	}
}

graph::~graph()	// destructor
{
	delete[] list;
}

node* graph::GetEdges(int source)	// return linked list of edges from source
{
	return list[source].GetHead();
}

void graph::CleanAllEdges()		// reset the whole graph
{
	for(int i=0; i<size; i++) {
		list[i].ResetList();
	}
}

graph::graph()	// default constructor
{
	list=NULL;
	size=0;
}

graph::graph(const int i)	// parameterized constructor
{
	list=new nodeList[i];
	size=i;
}

int graph::GetEdgeNum(int source)	// return number of edges starting from source
{
	return list[source].GetLength();
}

int graph::GetVertexNum()	// return total vertex number
{
	return size;
}

bool graph::SearchEdge(int source, int target) 	// search edge from source to target in graph
{
	if(list[source].SearchNode(target)!=NULL) {return true;}
	else {return false;}
}

bool graph::AddEdge(int source, int target, int weight)	// add edge from source to target with weight in graph
{
	if (source<size && target<size &&source!=target) {
		return list[source].AddNode(target, weight);
	} else {
		return false;
	}
}

bool graph::DeleteEdge(int source, int target)	// delete edge from source to target in graph
{
	if (source<size && target<size &&source!=target) {
		return list[source].DeleteNode(target);
	} else {
		return false;
	}
}

int graph::GetWeight(int source, int target) 	// return weight of edge from source to target
{
	if (source<size && target<size) {
		return list[source].GetWeight(target);
	} else {
		return -1;
	}
}
// END of class graph

// Class undirGraph: data structure to represent undirected graph, derived from class graph
class undirGraph: public graph {
	private:
		bool SeedGraphType1(int degree, int weight);	// seed to generate graph type 1
	public:
		undirGraph(const int i);	// parameterized constructor 1
		undirGraph(const int i, edge* E, int len);	// parameterized constructor 2
		bool AddEdge(int source, int target, int weight);	// add edge between source and target with weight in graph
		bool DeleteEdge(int source, int target);	// delete edge between source and target in graph
		void BuildGraphType1(int degree, int weight);	// generate graph type 1
		void BuildGraphType2(double percent, int weight);	// generate graph type 2
		void AddExtraEdgesForTest(int s, int t, int weight);	// add extra edges for path from s to t 
		void CleanIthEdges(int i);	// delete all edges connecting to vertex i
		int GetTotalEdgeNum();		// return total edge number in graph
};

int undirGraph::GetTotalEdgeNum()	// return total edge number in graph
{
	int noo=0;
	for(int i=0;i<size;i++) {
		for(node* p=GetEdges(i);p!=NULL;p=p->GetNextNode()) {
			if(p->GetVertex()>i) {noo++;}
		}
	}
	return noo;
}

void undirGraph::CleanIthEdges(int i)	// delete all edges connecting to vertex i
{
	if (i>=0 && i<size) {
		for(int j=0;j<size;j++) {DeleteEdge(i, j);}
	}
}


undirGraph::undirGraph(const int i, edge* E, int len)	// parameterized constructor 2
{
	list=new nodeList[i];
	size=i;
	for(int j=0;j<len;j++) {AddEdge(E[j].a, E[j].b, E[j].w);}
}

undirGraph::undirGraph(const int i)	// parameterized constructor 1
{
	list=new nodeList[i];
	size=i;
}

bool undirGraph::AddEdge(int source, int target, int weight)	// add edge between source and target with weight in graph
{
	if (source<size && target<size && source!=target) {
		return list[source].AddNode(target, weight) && list[target].AddNode(source, weight);
	} else {
		return false;
	}
}

bool undirGraph::DeleteEdge(int source, int target)	// delete edge between source and target in graph
{
	if (source<size && target<size &&source!=target) {
		return list[source].DeleteNode(target) && list[target].DeleteNode(source);
	} else {
		return false;
	}
}

bool undirGraph::SeedGraphType1(int degree, int weight)	// seed to generate graph type 1
{
	srand(time(0));
	int another;
	int count=size;
	for(int i=0; i<size; i++) {
		while(GetEdgeNum(i)<degree) {
			if(count+GetEdgeNum(i)<degree+1) {
				return false;
			}
			another=rand() % size;	
			if (GetEdgeNum(another)<degree) {
				int n=rand()%weight + 1;
				AddEdge(i, another, n);
				if(GetEdgeNum(another)>=degree) {
					count--;
				}
			} 
			if(GetEdgeNum(i)>=degree) {
				count--;	
			}
		}
	}
	return true;
}

void undirGraph::BuildGraphType1(int degree, int weight)	// generate graph type 1
{
	while(!SeedGraphType1(degree, weight)) {CleanAllEdges();}
}

void undirGraph::BuildGraphType2(double percent, int weight)	// generate graph type 2
{
	srand(time(0));
	int another;
	int count=size;
	int total=size;
	for(int i=0; i<size; i++) {
		while(GetEdgeNum(i)*1.0/(total-1)<percent) {
			another=rand() % size;	
			if(((count+GetEdgeNum(i)-1)*1.0/(total-1))<percent) {
				if(GetEdgeNum(another)*1.0/(total-1)<percent) {
					continue;
				} else if (GetEdgeNum(another)<=int(ceil(percent*(total-1)))) {
					int n=rand()%weight + 1;
					AddEdge(i, another, n);
				}
			} else {
				if (GetEdgeNum(another)*1.0/(total-1)<percent) {
					int n=rand()%weight + 1;
					AddEdge(i, another, n);
					if((GetEdgeNum(another)*1.0/(total-1))>=percent) {
						count--;
					}
				}
			}
			if((GetEdgeNum(i)*1.0/(total-1))>=percent) {
				count--;	
			}
		}
	}
}


void undirGraph::AddExtraEdgesForTest(int s, int t, int weight)	// add extra edges for path from s to t
{
	bool A[size];
	node *current;
	int mark;
	for(int i=0;i<size;i++) {A[i]=true;}
	int v=s;
	A[s]=false;
	while(1) {
		current=GetEdges(v);
		while(current!=NULL) {
			if(A[current->GetVertex()]==true && current->GetVertex()!=t) {
				v=current->GetVertex();
				A[v]=false;
				break;
			} else {
				current=current->GetNextNode();
			}
		}
		if (current==NULL) {
			mark=-1;
			for (int i=0;i<size;i++) {
				if(A[i] && i!=t) {mark=i;break;}
			}
			if (mark==-1) {	AddEdge(v, t, rand()%weight + 1); break;}
			else {
				AddEdge(v, mark, rand()%weight + 1);
				A[mark]=false;
				v=mark;
			}
		}
	}
	
}
// END of class undirGraph

//Class maxHeap: data structure of max-heap for routing algorithm
class maxHeap {
	private:
		int *H;	// array of object index
		int *D;	// array of object weight
		int length;	// current size of heap
		int maxsize;	// max size of heap
		bool Heapfy(int i);	// adjust the location of value not in correct max-heap order
	public:
		maxHeap();	// default constructor
		maxHeap(const int i);	// parameterized constructor
		int Max();		// return the index with maximum weight
		bool Insert(int i, int v);	// insert an index with weight
		bool Delete(int i);	// delete an index in ith slot of heap
		int GetValue(int i);	// return the weight of index in ith slot of heap
		int GetLength();	// return the current size of heap
		bool UpdateValue(int i, int v);	// update the weight of ith slot in heap
		void Renew();	// reset the heap
};

bool maxHeap::UpdateValue(int i, int v)	// update the weight of ith slot in heap
{
	if (i>0 && i<=maxsize) {
		int pos=-1;
		for (int k=1;k<=length;k++) {
			if(H[k]==i) {pos=k;break;}
		}
		if (pos!=-1) {
			D[i]=v;
			Heapfy(pos);
		} else {
			return false;
		}	
	} else {
		return false;
	}
}

int maxHeap::GetLength()	// return the current size of heap
{
	return length;
}

int maxHeap::GetValue(int i)	// return the weight of index in ith slot of heap
{
	if (i>0 && i<=maxsize) {
		return D[i];
	} else {
		return -1;
	}
}

maxHeap::maxHeap()	// default constructor
{
	H=NULL;
	D=NULL;
	length=0;
	maxsize=0;
}

maxHeap::maxHeap(const int i)	// parameterized constructor
{
	if(i>0) {
		H=new int[i+1];
		D=new int[i+1];
		maxsize=i;
		length=0;
		for(int k=0;k<i+1;k++) {D[k]=-1;} 
	} else {
		H=NULL;
		D=NULL;
		maxsize=0;
		length=0;
	}
}

void maxHeap::Renew()	// reset the heap
{
	delete H;
	delete D;
	H=new int[maxsize+1];
	D=new int[maxsize+1];
	for(int k=0;k<maxsize+1;k++) {D[k]=-1;} 
	length=0;
}

bool maxHeap::Heapfy(int i)	// adjust the location of value not in correct max-heap order
{
	int h, tmp;
	if (i>0 && i<=length) {
		if(i>1 && D[H[i]]>D[H[i/2]]) {
			h=i;
			while(h>1 && D[H[h]]>D[H[h/2]]) {
				tmp=H[h/2];
				H[h/2]=H[h];
				H[h]=tmp;
				h=h/2;
			}
		} else if (i<=length/2 && D[H[i]]<max(D[H[2*i]],D[H[2*i+1]])) {
			h=i;
			while(h<=length/2 && D[H[h]]<max(D[H[2*h]],D[H[2*h+1]])) {
				if(D[H[2*h]]>=D[H[2*h+1]]) {
					tmp=H[2*h];
					H[2*h]=H[h];
					H[h]=tmp;
					h=2*h;
				} else {
					tmp=H[2*h+1];
					H[2*h+1]=H[h];
					H[h]=tmp;
					h=2*h+1;
				}
			}
		}
		return true;
	} else {
		return false;
	}
}

int maxHeap::Max()	// return the index with maximum weight
{
	return H[1];
}

bool maxHeap::Insert(int i, int v)	// insert an index with weight
{
	if (length==maxsize) {
		return false;
	} else {
		if (i>0 && i<=maxsize) {
			D[i]=v;
			length++;
			H[length]=i;
			return Heapfy(length);
		} else {
			return false;
		}
	}
}

bool maxHeap::Delete(int i)	// delete an index in ith slot of heap
{
	if (i>0 && i<=length) {
		H[i]=H[length];
		length--;
		if (length> 0) {return Heapfy(i);}
		else {return true;}
	} else {
		return false;
	}
}
// END of class maxHeap

// function to check any fringes in the graph
bool ExistFringe(string* A, int len)
{
	for(int i=0;i<len;i++)	{
		if (A[i]=="fringe") {return true;}
	}
	return false;
}

// function to find fringes with largest capacity in Dijkstra's algorithm
int PickLargestFringe(string* A, int* W, int len)
{
	int cap=-999999;
	int v=-1;
	for(int i=0;i<len;i++)	{
		if (A[i]=="fringe") {
			if(W[i]>cap) {
				cap=W[i];
				v=i;
			}
		}
	}
	return v;
}

// Dijkstra's algorithm without using a heap structure
int* Dijkstra(undirGraph &G, int s, int t)
{
	int n=G.GetVertexNum();
	int* Dad=new int[n];
	int Cap[n];
	string Status[n];
	node* ptr;	
	int v, w;
	for (int i=0;i<n; i++) {
		Status[i]="unseen";
		Dad[i]=-1;
	}
	Status[s]="intree";
	ptr=G.GetEdges(s);
	for(node* p=ptr; p!=NULL; p=p->GetNextNode()) {
		w=p->GetVertex();
		Status[w]="fringe";
		Dad[w]=s;
		Cap[w]=p->GetWeight();
	}
	while(ExistFringe(Status, n)) {
		v=PickLargestFringe(Status, Cap, n);
		Status[v]="intree";
		ptr=G.GetEdges(v);
		for(node* p=ptr; p!=NULL; p=p->GetNextNode()) {
			w=p->GetVertex();
			if(Status[w]=="unseen") {
				Status[w]="fringe";
				Dad[w]=v;
				Cap[w]=min(p->GetWeight(),Cap[v]);
			} else if (Status[w]=="fringe" && Cap[w]<min(p->GetWeight(),Cap[v])) {
				Dad[w]=v;
				Cap[w]=min(p->GetWeight(),Cap[v]);
			}
		}
		
	}
	return Dad;
}  

// Dijkstra's algorithm using a max-heap structure
int* ModifiedDijkstra(undirGraph &G, int s, int t)
{
	int n=G.GetVertexNum();
	int* Dad=new int[n];
	int Cap[n];
	string Status[n];
	node* ptr;	
	int v, w;
	maxHeap H(n);
	for (int i=0;i<n; i++) {
		Status[i]="unseen";
		Dad[i]=-1;
	}
	Status[s]="intree";
	ptr=G.GetEdges(s);
	for(node* p=ptr; p!=NULL; p=p->GetNextNode()) {
		w=p->GetVertex();
		Status[w]="fringe";
		Dad[w]=s;
		Cap[w]=p->GetWeight();
		H.Insert(w+1, Cap[w]);
	}
	while(H.GetLength()) {
		v=H.Max()-1;
		Status[v]="intree";
		H.Delete(1);
		ptr=G.GetEdges(v);
		for(node* p=ptr; p!=NULL; p=p->GetNextNode()) {
			w=p->GetVertex();
			if(Status[w]=="unseen") {
				Status[w]="fringe";
				Dad[w]=v;
				Cap[w]=min(p->GetWeight(),Cap[v]);
				H.Insert(w+1, Cap[w]);
			} else if (Status[w]=="fringe" && Cap[w]<min(p->GetWeight(),Cap[v])) {
				Dad[w]=v;
				Cap[w]=min(p->GetWeight(),Cap[v]);
				H.UpdateValue(w+1, Cap[w]);
			}
		}
	}
	return Dad;
}

// function Find in MakeSet-Find-Union operations
int Find(int v, int n, int *Dad)
{
	int w=v;
	int S[n];
	int Slen=0;
	int u;
	while(Dad[w]!=-1) {
		S[Slen++]=w;
		w=Dad[w];
	}
	while(Slen) {
		u=S[--Slen];
		Dad[u]=w;
	}
	return w;
}

// function Union in MakeSet-Find-Union operations
bool Union(int r1, int r2, int *Dad, int *rank)
{
	if(rank[r1]>rank[r2]) {
		Dad[r2]=r1;
	} else if (rank[r1]<rank[r2]) {	
		Dad[r1]=r2;
	} else {
		Dad[r2]=r1;
		rank[r1]++;
	}
}

// Kruskal's algorithm using a max-heap to sort edges
int* Kruskal(undirGraph &G, int s, int t)
{	    
	const rlim_t kStackSize = 512 * 1024 * 1024;   // min stack size = 512 MB
   	struct rlimit rl;
   	int result;
   	result = getrlimit(RLIMIT_STACK, &rl);
   	if (result == 0)
   	{
   	    if (rl.rlim_cur < kStackSize)
   	    {
   	        rl.rlim_cur = kStackSize;
   	        result = setrlimit(RLIMIT_STACK, &rl);
   	        if (result != 0)
   	        {
   	            fprintf(stderr, "setrlimit returned result = %d\n", result);
   	        }
   	    }
   	}

	int n=G.GetVertexNum();
	int V=G.GetTotalEdgeNum();
	edge E[V];
	int Elen=0;
	edge T[n-1];
	int Tlen=0;
	node* ptr;
	for(int i=0;i<n;i++) {
		ptr=G.GetEdges(i);
		for(node* p=ptr;p!=NULL;p=p->GetNextNode()) {
			if(p->GetVertex()>i) {
				E[Elen].a=i;
				E[Elen].b=p->GetVertex();
				E[Elen].w=p->GetWeight();
				Elen++;
			}
		}
	}
	maxHeap H(Elen);
	for(int i=0;i<Elen;i++) {
		H.Insert(i+1, E[i].w);
	}
	int F[Elen];
	int j=0;
	while(H.GetLength()) {
		F[j++]=H.Max()-1;
		H.Delete(1);
	}
	int Dad[n], rank[n];
	for(int i=0;i<n;i++) {
		Dad[i]=-1;
		rank[i]=0;
	}
	for(int i=0;i<Elen;i++) {
		int r1=Find(E[F[i]].a, n, Dad);
		int r2=Find(E[F[i]].b, n, Dad);
		if(r1!=r2) {
			T[Tlen++]=E[F[i]];
			Union(r1, r2, Dad, rank);
		}
	}
	int u;
	string color[n];
	int *P =new int[n];
	for(int i=0;i<n;i++) {
		color[i]="white";
		P[i]=-1;
	}
	int S[n];
	int Slen=0;
	S[Slen++]=s;
	while(Slen) {
		u=S[--Slen];
		color[u]="gray";
		for(int i=0;i<Tlen;i++) {
			if(T[i].a==u && color[T[i].b]=="white") {
				S[Slen++]=T[i].b;
				P[T[i].b]=T[i].a;
			} else if(T[i].b==u && color[T[i].a]=="white") {
				S[Slen++]=T[i].a;
				P[T[i].a]=T[i].b;
			}
		}
		color[u]="black";
	}
	return P;
}

// function to find the median weight from a set of edges
int Median(edge *E, int n, int k)
{
	if(n<=10) {
		maxHeap H(n);
		int S[n];
		for(int i=0;i<n;i++) {
			H.Insert(i+1, E[i].w);
		}
		for(int i=n;i>0;i--) {
			S[i-1]=H.Max()-1;
			H.Delete(1);
		}
		return E[S[k-1]].w;
	}
	int Slen_5=(n%5==0)?(n/5):(n/5+1);
	edge Sn_5[Slen_5];
	maxHeap H(n);
	int S[5];
	int j=1;
	while(j<=n) {
		H.Insert(j, E[j-1].w);
		if(j%5==0) {
			for(int i=5;i>0;i--) {
				S[i-1]=H.Max()-1;
				H.Delete(1);
			}
			Sn_5[j/5-1]=E[S[2]];
			H.Renew();
		}
		j++;
	}
	if(n/5!=0) {
		int h=H.GetLength();
		for(int i=h;i>0;i--) {
			S[i-1]=H.Max()-1;
			H.Delete(1);
		}
		Sn_5[n/5]=E[S[h/2]];
	}
	int m=Median(Sn_5, Slen_5, int(ceil(Slen_5*1.0/2)));
	int Slensmall=0;
	int Slenlarge=0;
	edge Ssmall[n], Slarge[n];
	for(int i=0;i<n;i++) {
		if(E[i].w<m) {Ssmall[Slensmall]=E[i];Slensmall++;}
		else {Slarge[Slenlarge]=E[i];Slenlarge++;}
	}
	if(k<=Slensmall) {return Median(Ssmall, Slensmall, k);}
	else {return Median(Slarge, Slenlarge, k-Slensmall);}
}

// function to build path in a undirected graph for the linear-time algorithm
int* BuildPath(undirGraph &G, int source, int target)
{
	int n=G.GetVertexNum();
	int *Dad=new int[n];
	for(int i=0;i<n;i++) {Dad[i]=-1;}
	int stack[n];
	int stacklen=0;
	stack[stacklen++]=source;
	undirGraph Gdummy=G;
	while(stacklen) {
		int tt=stack[--stacklen];
		for(node* ptr=Gdummy.GetEdges(tt);ptr!=NULL;ptr=ptr->GetNextNode()) {
			Dad[ptr->GetVertex()]=tt;
			stack[stacklen++]=ptr->GetVertex();
		}
		Gdummy.CleanIthEdges(tt);
	}
	return Dad;
}

// Linear-time algorithm to find max-bandwith path based on median of median
int* LinearAlgTrue(undirGraph &G, int s, int t) {
	const rlim_t kStackSize = 512 * 1024 * 1024;   // min stack size = 512 MB
   	struct rlimit rl;
   	int result;
   	result = getrlimit(RLIMIT_STACK, &rl);
   	if (result == 0)
   	{
   	    if (rl.rlim_cur < kStackSize)
   	    {
   	        rl.rlim_cur = kStackSize;
   	        result = setrlimit(RLIMIT_STACK, &rl);
   	        if (result != 0)
   	        {
   	            fprintf(stderr, "setrlimit returned result = %d\n", result);
   	        }
   	    }
   	}
	int n=G.GetVertexNum();
	int V=G.GetTotalEdgeNum();
	edge E[V];
	int Elen=0;
	node* ptr;
	for(int i=0;i<n;i++) {
		ptr=G.GetEdges(i);
		for(node* p=ptr;p!=NULL;p=p->GetNextNode()) {
			if(p->GetVertex()>i) {
				E[Elen].a=i;
				E[Elen].b=p->GetVertex();
				E[Elen].w=p->GetWeight();
				Elen++;
			}
		}
	}
	int m = Median(E, Elen, Elen/2);
	edge Large[V], Small[V];
	int Llen=0; int Slen=0;
	int flag=0;
	for(int i=0;i<Elen;i++) {
		if(E[i].w>m) {
			Large[Llen].a=E[i].a;
			Large[Llen].b=E[i].b;
			Large[Llen].w=E[i].w;
			Llen++;
		} else {
			Small[Slen].a=E[i].a;
			Small[Slen].b=E[i].b;
			Small[Slen].w=E[i].w;
			Slen++;
		}
	}
	undirGraph Glarge(n, Large, Llen);
	undirGraph Gsmall(n, Small, Slen);
	int Com[n];
	for(int i=0;i<n;i++) {Com[i]=-1;}
	int count=0;
	undirGraph Gdummy=Glarge;
	int stack[n];
	int stacklen=0;
	for(int i=0;i<n;i++) {
		if(Com[i]==-1) {
			Com[i]=count; count++;
			stack[stacklen++]=i;
			while(stacklen) {
				int h=stack[--stacklen];
				for(int j=0;j<n;j++) {
					if(Gdummy.SearchEdge(h, j) && Com[j]==-1) {
						Com[j]=Com[h];	Gdummy.DeleteEdge(h, j); stack[stacklen++]=j;
					}
				}
			}
		}
	}
	if(Com[s]==Com[t]) {
		return LinearAlgTrue(Glarge, s, t);
	} else {
		int nNew=Com[0];
		for(int i=0;i<n;i++) { if(Com[i]>nNew) {nNew=Com[i];} }
		nNew++;
		undirGraph Gnew(nNew);
		int k; edge P;
		edge test[nNew*(nNew-1)/2];
		edge NewE[nNew][nNew];
		for(int i=0;i<nNew;i++) {
			for(int j=0;j<nNew;j++) {
				NewE[i][j].a=-1;
				NewE[i][j].b=-1;
				NewE[i][j].w=-1;
			}
		}
		for(int i=0;i<nNew;i++) {
			for(int j=i+1;j<nNew;j++) {
				k=-1;
				for(int v=0;v<n;v++) {
					for(int u=v+1;u<n;u++) {
						if((Com[v]==i && Com[u]==j) || (Com[v]==j && Com[u]==i)) {
							if(Gsmall.GetWeight(v, u)>k) {
							k=Gsmall.GetWeight(v, u);
							P.a=v;
							P.b=u;
							P.w=k;
							}
						}
					}
				}
				if(k!=-1) {
					Gnew.AddEdge(i, j, k);
					NewE[i][j].a=P.a;
					NewE[i][j].b=P.b;
					NewE[i][j].w=P.w;
				}
			}
		}
		int* Dad=new int[n];
		for(int i=0;i<n;i++) {Dad[i]=-1;}
		if(Gnew.SearchEdge(Com[s], Com[t]) && (Gnew.GetComMaxWeight(Com[s], Com[t])==Gnew.GetWeight(Com[s], Com[t]))) {
			int ap=((Com[s]<Com[t])?NewE[Com[s]][Com[t]].a:NewE[Com[t]][Com[s]].a);
			int bp=((Com[s]<Com[t])?NewE[Com[s]][Com[t]].b:NewE[Com[t]][Com[s]].b);
			int *Dads, *Dadt;
			if(Com[s]==Com[ap]) {
				Dads=BuildPath(Glarge, s, ap);
				Dadt=BuildPath(Glarge, bp ,t);
				int k=ap;
				while(Dads[k]!=-1) {
					Dad[k]=Dads[k];
					k=Dads[k];
				}
				k=t;
				while(Dadt[k]!=-1) {
					Dad[k]=Dadt[k];
					k=Dadt[k];
				}
				Dad[bp]=ap;
			} else {
				Dads=BuildPath(Glarge, s, bp);
				Dadt=BuildPath(Glarge, ap ,t);
				int k=bp;
				while(Dads[k]!=-1) {
					Dad[k]=Dads[k];
					k=Dads[k];
				}
				k=t;
				while(Dadt[k]!=-1) {
					Dad[k]=Dadt[k];
					k=Dadt[k];
				}
				Dad[ap]=bp;			
			}
			delete(Dads);
			delete(Dadt);
		} else {
			int *Dadnew = LinearAlgTrue(Gnew, Com[s], Com[t]);
			int tp=t;
			int comtp=Com[tp];
			int comsp=Dadnew[comtp];
			int *Dadt;
			while(comtp!=Com[s]) {
				int ap=((comsp<comtp) ? NewE[comsp][comtp].a : NewE[comtp][comsp].a);
				int bp=((comsp<comtp) ? NewE[comsp][comtp].b : NewE[comtp][comsp].b);
				if(comtp==Com[ap]) {
					Dadt=BuildPath(Glarge, ap, tp);
					int k=tp;
					while(Dadt[k]!=-1) {
						Dad[k]=Dadt[k];
						k=Dadt[k];
					}
					Dad[ap]=bp;
					tp=bp;
					delete(Dadt);
				} else {
					Dadt=BuildPath(Glarge, bp, tp);
					int k=tp;
					while(Dadt[k]!=-1) {
						Dad[k]=Dadt[k];
						k=Dadt[k];
					}
					Dad[bp]=ap;
					tp=ap;
					delete(Dadt);
				}
				comtp=Com[tp];
				comsp=Dadnew[comtp];
			}
			Dadt=BuildPath(Glarge, s, tp);
			int k=tp;
			while(Dadt[k]!=-1) {
				Dad[k]=Dadt[k];
				k=Dadt[k];
			}
			delete(Dadnew);
		}
		return Dad;
	}
}

// function to display the max-bandwidth path for a given path and its max bandwidth
void Display(int *Dad, undirGraph &G, int source, int target)
{
	int stack[G.GetVertexNum()];
	int stacklen=0;
	int h=target;
	while(h!=-1) {
		stack[stacklen++]=h;
		h=Dad[h];
	}
	int a=stack[--stacklen];
	int b=stack[--stacklen];
	int maxband=G.GetWeight(a, b);
	cout << a<<"-";
	while(stacklen) {
		cout <<b<<"-";
		if(G.GetWeight(a,b)<maxband) {maxband=G.GetWeight(a,b);}
		a=b;
		b=stack[--stacklen];
	}
	if(G.GetWeight(a,b)<maxband) {maxband=G.GetWeight(a,b);} cout<<b;
	cout <<endl<<"Max Bandwidth: "<<maxband<<endl;
}

// function to display the max bandwidth of a given path
void Display2(int *Dad, undirGraph &G, int source, int target)
{
	int stack[G.GetVertexNum()];
	int stacklen=0;
	int h=target;
	while(h!=-1) {
		stack[stacklen++]=h;
		h=Dad[h];
	}
	int a=stack[--stacklen];
	int b=stack[--stacklen];
	int maxband=G.GetWeight(a, b);
	while(stacklen) {
		if(G.GetWeight(a,b)<maxband) {maxband=G.GetWeight(a,b);}
		a=b;
		b=stack[--stacklen];
	}
	if(G.GetWeight(a,b)<maxband) {maxband=G.GetWeight(a,b);} //cout<<b;
	cout <<"Max Bandwidth: "<<maxband<<endl<<endl;
}

// function to count runtime
static double diffclock(clock_t clock1,clock_t clock2)
{
    double diffticks=clock1-clock2;
    double diffms=(diffticks)/(CLOCKS_PER_SEC/1000);
    return diffms;
}

// function to test 3 routing algorithms for max-bandwidth problem
void TestRoutingAlg()
{	
	srand(time(0));
	clock_t begin, end;
	double seconds;
	int source, target;
	int *Dad, *Dad2, *Dad3;	
	undirGraph G1(VMAX_NUM), G2(VMAX_NUM);
	cout << "------Start to test routing algorithms------"<<endl<<endl;
	for(int q=0;q<5;q++) {
		cout << "Round "<<q+1<<endl<<endl;
		G1.BuildGraphType1(G1_DEGREE, MAX_WEIGHT);
		cout << "Build graph type1"<<endl;
		G2.BuildGraphType2(G2_PERCENT, MAX_WEIGHT);
		cout << "Build graph tupe2"<<endl<<endl;
		for(int c=0;c<5;c++) {
			undirGraph G3=G1;
			undirGraph G4=G2;
			do {
				source=rand() % VMAX_NUM;
				target=rand() % VMAX_NUM;
			} while(source==target)	;
			cout << "Randomly pick source-destination pair : "<<source<<", "<<target<<endl<<endl;
			G3.AddExtraEdgesForTest(source, target, MAX_WEIGHT);
			cout << "Testing algorithms on graph type 1 from "<<source<<" to " <<target<<endl<<endl;

			begin = clock();
			Dad=Dijkstra(G3, source, target);
			end = clock();
			seconds = diffclock(end, begin);
			cout<< "Runtim of Dijkstra Algorithm: "<<seconds<<" ms"<< endl;
			cout << "Result of Dijkstra Algorithm: "<<endl;
			Display2(Dad, G3, source, target);
			delete Dad;

			begin = clock();
			Dad2=ModifiedDijkstra(G3, source, target);
			end = clock();
			seconds = diffclock(end, begin);
			cout<< "Runtim of Modified Dijkstra Algorithm with Max Heap: "<<seconds<<" ms"<< endl;
			cout << "Result of Modified Dijkstra Algorithm with Max Heap: "<<endl;
			Display2(Dad2, G3, source, target);
			delete Dad2;

			begin = clock();
			Dad3=Kruskal(G3, source, target);
			end = clock();
			seconds = diffclock(end, begin);
			cout << "Runtim of Kruskal Algorithm: "<<seconds<<" ms"<< endl;
			cout << "Result of Kruskal Algorithm: "<<endl;
			Display2(Dad3, G3, source, target);
			delete Dad3;		

			G3.CleanAllEdges();

			G4.AddExtraEdgesForTest(source, target, MAX_WEIGHT);
			cout << "Testing algorithms on graph type 2 from "<<source<<" to " <<target<<endl<<endl;

			begin = clock();
			Dad=Dijkstra(G4, source, target);
			end = clock();
			seconds = diffclock(end, begin);
			cout<< "Runtim of Dijkstra Algorithm: "<<seconds<<" ms"<< endl;
			cout << "Result of Dijkstra Algorithm: "<<endl;
			Display2(Dad, G4, source, target);
			delete Dad;

			begin = clock();
			Dad2=ModifiedDijkstra(G4, source, target);
			end = clock();
			seconds = diffclock(end, begin);
			cout<< "Runtim of Modified Dijkstra Algorithm with Max Heap: "<<seconds<<" ms"<< endl;
			cout << "Result of Modified Dijkstra Algorithm with Max Heap: "<<endl;
			Display2(Dad2, G4, source, target);
			delete Dad2;
		
			begin = clock();
			Dad3=Kruskal(G4, source, target);
			end = clock();
			seconds = diffclock(end, begin);
			cout << "Runtim of Kruskal Algorithm: "<<seconds<<" ms"<< endl;
			cout << "Result of Kruskal Algorithm: "<<endl;
			Display2(Dad3, G4, source, target);
			delete Dad3;

			G4.CleanAllEdges();
		}
		G1.CleanAllEdges();G2.CleanAllEdges();
	}
}

// function to test 4 algorithms for max-bandwidth problem
// 3 algorithms + linear-time algorithm
void TestRoutingAlgExt()
{	
	srand(time(0));
	clock_t begin, end;
	double seconds;
	int source, target;
	int *Dad, *Dad2, *Dad3, *Dad4;	
	undirGraph G1(VMAX_NUM), G2(VMAX_NUM);
	cout << "------Start to test routing algorithms------"<<endl<<endl;
	for(int q=0;q<5;q++) {
		cout << "Round "<<q+1<<endl<<endl;
		G1.BuildGraphType1(G1_DEGREE, MAX_WEIGHT);
		cout << "Build graph type1"<<endl;
		G2.BuildGraphType2(G2_PERCENT, MAX_WEIGHT);
		cout << "Build graph tupe2"<<endl<<endl;
		for(int c=0;c<5;c++) {
			undirGraph G3=G1;
			undirGraph G4=G2;
			do {
				source=rand() % VMAX_NUM;
				target=rand() % VMAX_NUM;
			} while(source==target)	;
			cout << "Randomly pick source-destination pair : "<<source<<", "<<target<<endl<<endl;
			G3.AddExtraEdgesForTest(source, target, MAX_WEIGHT);
			cout << "Testing algorithms on graph type 1 from "<<source<<" to " <<target<<endl<<endl;

			begin = clock();
			Dad=Dijkstra(G3, source, target);
			end = clock();
			seconds = diffclock(end, begin);
			cout<< "Runtim of Dijkstra Algorithm: "<<seconds<<" ms"<< endl;
			cout << "Result of Dijkstra Algorithm: "<<endl;
			Display2(Dad, G3, source, target);
			delete Dad;

			begin = clock();
			Dad2=ModifiedDijkstra(G3, source, target);
			end = clock();
			seconds = diffclock(end, begin);
			cout<< "Runtim of Modified Dijkstra Algorithm with Max Heap: "<<seconds<<" ms"<< endl;
			cout << "Result of Modified Dijkstra Algorithm with Max Heap: "<<endl;
			Display2(Dad2, G3, source, target);
			delete Dad2;

			begin = clock();
			Dad3=Kruskal(G3, source, target);
			end = clock();
			seconds = diffclock(end, begin);
			cout << "Runtim of Kruskal Algorithm: "<<seconds<<" ms"<< endl;
			cout << "Result of Kruskal Algorithm: "<<endl;
			Display2(Dad3, G3, source, target);
			delete Dad3;		

			begin = clock();
			Dad4=LinearAlgTrue(G3, source, target);
			end = clock();
			seconds = diffclock(end, begin);
			cout << "Runtim of Linear Algorithm: "<<seconds<<" ms"<< endl;
			cout << "Result of Linear Algorithm: "<<endl;
			Display2(Dad4, G3, source, target);
			delete Dad4;		

			G3.CleanAllEdges();

			G4.AddExtraEdgesForTest(source, target, MAX_WEIGHT);
			cout << "Testing algorithms on graph type 2 from "<<source<<" to " <<target<<endl<<endl;

			begin = clock();
			Dad=Dijkstra(G4, source, target);
			end = clock();
			seconds = diffclock(end, begin);
			cout<< "Runtim of Dijkstra Algorithm: "<<seconds<<" ms"<< endl;
			cout << "Result of Dijkstra Algorithm: "<<endl;
			Display2(Dad, G4, source, target);
			delete Dad;

			begin = clock();
			Dad2=ModifiedDijkstra(G4, source, target);
			end = clock();
			seconds = diffclock(end, begin);
			cout<< "Runtim of Modified Dijkstra Algorithm with Max Heap: "<<seconds<<" ms"<< endl;
			cout << "Result of Modified Dijkstra Algorithm with Max Heap: "<<endl;
			Display2(Dad2, G4, source, target);
			delete Dad2;
		
			begin = clock();
			Dad3=Kruskal(G4, source, target);
			end = clock();
			seconds = diffclock(end, begin);
			cout << "Runtim of Kruskal Algorithm: "<<seconds<<" ms"<< endl;
			cout << "Result of Kruskal Algorithm: "<<endl;
			Display2(Dad3, G4, source, target);
			delete Dad3;

			begin = clock();
			Dad4=LinearAlgTrue(G4, source, target);
			end = clock();
			seconds = diffclock(end, begin);
			cout << "Runtim of Linear Algorithm: "<<seconds<<" ms"<< endl;
			cout << "Result of Linear Algorithm: "<<endl;
			Display2(Dad4, G4, source, target);
			delete Dad4;

			G4.CleanAllEdges();
		}
		G1.CleanAllEdges();G2.CleanAllEdges();
	}
}

// Main function
int main (int argc, char *argv[]) {
	TestRoutingAlg();
	//TestRoutingAlgExt();
	// before using TestRoutingAlgExt(), please make sure the value of VMAX_NUM is suitable for testing
	// the runtime of linear-time algorithm in graph with large vertex number is unaffordable 
	return 1;
} 
