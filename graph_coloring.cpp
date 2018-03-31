#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<string.h>
#include<stdlib.h>
#include<map>
#include<set>

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
    	if(flag == 1 && n_vertices != -1){
    		while(getline(inFile,line)){
    			arr = split(line, " ");
    			if(arr[0] == "e"){
    				int v1 = atoi(arr[1].c_str());
    				int v2 = atoi(arr[2].c_str());
    				graph[v1].insert(v2);
					graph[v2].insert(v1);
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
		
		//----------------------------------------------------------------------
		return 0;
	}
	else{
		cerr << "Unable to open file" << endl;
	}
}
