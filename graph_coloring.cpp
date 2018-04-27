#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<string.h>
#include<stdlib.h>
#include<map>
#include<set>
#include <cstdio>
#include<algorithm>

using namespace std;

//function to split a string
vector<string> split(string str, string dlm){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    vector<string> arr;
    current=strtok(cstr,dlm.c_str());
    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,dlm.c_str());
	}
    return arr;
}

//priority queue with change key 
//- Reference:https://github.com/kartikkukreja/blog-codes/blob/master/src/Indexed%20Min%20Priority%20Queue.cpp
class MinIndexedPQ {
    int NMAX, N, *heap, *index, *keys;

    void swap(int i, int j) {
        int t = heap[i]; heap[i] = heap[j]; heap[j] = t;
        index[heap[i]] = i; index[heap[j]] = j;
    }

    void bubbleUp(int k)    {
        while(k > 1 && keys[heap[k/2]] > keys[heap[k]])   {
            swap(k, k/2);
            k = k/2;
        }
    }

    void bubbleDown(int k)  {
        int j;
        while(2*k <= N) {
            j = 2*k;
            if(j < N && keys[heap[j]] > keys[heap[j+1]])
                j++;
            if(keys[heap[k]] <= keys[heap[j]])
                break;
            swap(k, j);
            k = j;
        }
    }

public:
    // Create an empty MinIndexedPQ which can contain atmost NMAX elements
    MinIndexedPQ(int NMAX)  {
        this->NMAX = NMAX;
        N = 0;
        keys = new int[NMAX + 1];
        heap = new int[NMAX + 1];
        index = new int[NMAX + 1];
        for(int i = 0; i <= NMAX; i++)
            index[i] = -1;
    }
	
    ~MinIndexedPQ() {
        delete [] keys;
        delete [] heap;
        delete [] index;
    }

    // check if the PQ is empty
    bool isEmpty()  {
        return N == 0;
    }

    // associate key with index i; 0 < i < NMAX
    void insert(int i, int key) {
        N++;
        index[i] = N;
        heap[N] = i;
        keys[i] = key;
        bubbleUp(N);
    }

    // delete the minimal key and return its associated index
    int deleteMin() {
        int min = heap[1];
        swap(1, N--);
        bubbleDown(1);
        index[min] = -1;
        heap[N+1] = -1;
        return min;
    }

    // decrease the key associated with index i to the specified value
    void decreaseKey(int i, int key)    {
    	int ind;
    	for(int j = 0; j<= N; j++)
    	{
    		if(heap[j] == i)
    		{
    			ind = j;
    			break;
			}
		}
        keys[ind] = key;
        bubbleUp(index[ind]);
    }
};

int main()
{
	//input graph-------------------------------------------------------------
	//input filename
    string filename;
    cout << "Enter the filename: ";
    getline(cin, filename);
    
    //read the file and make graph
    ifstream inFile(filename.c_str());   
    if(inFile.is_open()){
    	vector<string> arr;
    	map<int, set<int> > graph; //some files mention the edges twice
        string line;
        int flag = 0;
        int n_vertices = -1;
        int m_edges = -1;
        while(getline(inFile, line)){
        	//find the parameters 
	        arr = split(line," ");
	        if(arr[0] == "p"){
	        	flag = 1;
	        	n_vertices = atoi(arr[2].c_str());
	        	m_edges = atoi(arr[3].c_str());
	        	break;
			}
    	}
    	
    	//add edges to the graph
    	int degree[n_vertices+1] = {0};
    	if(flag == 1 && n_vertices != -1){
    		while(getline(inFile,line)){
    			arr = split(line, " ");
    			if(arr[0] == "e"){
    				int v1 = atoi(arr[1].c_str());
    				int v2 = atoi(arr[2].c_str());
    				graph[v1].insert(v2);
					graph[v2].insert(v1);
					degree[v1] += 1;
					degree[v2] += 1;
				}
			}
		}
		else{
			cerr<< "Missing parameters" << endl;
		}
		
		//check whether the graph is empty
		if(graph.size() == 0){
			cout << "Empty instance" << endl;
			return -1;
		}
		//----------------------------------------------------------------------
		
		
		//start dsatur algorithm to color the graph
		//----------------------------------------------------------------------
		
		int saturation[n_vertices + 1] = {0};
		int color[n_vertices + 1] = {0};
		MinIndexedPQ pq(n_vertices);
		
		//add vertices to the priority queue
		for(int i = 1; i <= n_vertices+1; i++)
		{
			pq.insert(i, -1*degree[i]);
		}
		
		//color vertices
		int max = 0;
		while(!pq.isEmpty())
		{
			int current_node = pq.deleteMin();
			int color_ind[n_vertices + 1] = {0};
			int ind;
			
			//find minimum color
			for (set<int>::iterator it=graph[current_node].begin(); it!=graph[current_node].end(); ++it)
			{
				 int node = *it;
				 color_ind[color[node]] = 1; 
				 saturation[node] += 1;   //increase saturation degree of neighbour nodes
				 int node_key = n_vertices*saturation[node] + degree[node];
				 node_key = -1*node_key;
				 pq.decreaseKey(node, node_key); //change key of neighbours
			}
			
			for(int k=1; k<=n_vertices+1; k++)
			{
				if(color_ind[k] == 0)
				{
					ind = k;
					if(ind>max)
					{
						max = ind;
					}
					break;
				}
			}
			
			color[current_node] = ind;
			color_ind[n_vertices + 1] = {0};
		}
		cout<<max;
		//----------------------------------------------------------------------
		return 0;
	}
	else{
		cerr << "Unable to open file" << endl;
	}
}
