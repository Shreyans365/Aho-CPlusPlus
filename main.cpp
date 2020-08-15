//
//  main.cpp
//  Aho-Corasick
//
//  Created by Shreyans Jain on 7/25/20.
//  Copyright © 2020 Shreyans Jain. All rights reserved.
//

#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

struct node {
    int edge[258];   // edges originating from this node
    bool leaf;      // true if this node represents a string in the given set
    int p;          // parent of this node
    char pch;       // character marked on edge from parent to this node
    int link;       // suffix-link originating from this node
    int transition[258];   // node to transition to on getting some character at this node
    int depth;
    int id;
    
    node(int d, char c, int par) {
        memset(edge, -1, sizeof(edge));
        memset(transition, -1, sizeof(transition));
        id = -1;
        depth = d;
        link = -1;
        pch = c;
        p = par;
        leaf = false;
    }
};

node root(0, 0, -1);
vector<node> aho = {root};

void add_string(string& str, int x = -1) {
    int curr_node = 0;      // start from root node
    for (char& c: str) {
        int ch = c;
        if (aho[curr_node].edge[ch] == -1) {      // make new node if current node does not have required edge
            aho[curr_node].edge[ch] = (int)(aho.size());
            node newNode(aho[curr_node].depth+1, c, curr_node);
            aho.push_back(newNode);
        }
        curr_node = aho[curr_node].edge[ch];     // travel to next node
    }
    aho[curr_node].id = x;
    aho[curr_node].leaf = true;     // string represented by this node is present in set
}

int makeTransition(int curr_node, char c);

int getLink(int curr_node) {
    if (aho[curr_node].link == -1) {
        if (curr_node == 0 || aho[curr_node].p == 0)
            aho[curr_node].link = 0;
        else {
            aho[curr_node].link = makeTransition(getLink(aho[curr_node].p), aho[curr_node].pch);
        }
    }
    return aho[curr_node].link;
}


int makeTransition(int curr_node, char c) {
    int ch = c;
    if (aho[curr_node].transition[ch] == -1) {
        if (aho[curr_node].edge[ch] == -1)
            aho[curr_node].transition[ch] = (curr_node == 0)?0:makeTransition(getLink(curr_node), c);
        else
            aho[curr_node].transition[ch] = aho[curr_node].edge[ch];
    }
    return aho[curr_node].transition[ch];
}


void find(string& text, vector<string>& patterns) {
    if (patterns.size() == 0) {
        cout<<text<<endl;
        return;
    }

    int curr_node = 0;
    int i = 0;
    unordered_map<int, bool> map;
    for (char& c: text) {
        curr_node = makeTransition(curr_node, c);
        if (aho[curr_node].leaf == true) {
            map[i-aho[curr_node].depth+1] = true;
        }
        i++;
    }
    
    int len = text.length();
    string output = "";
    
    for (int i=0;i<len;i++) {
        if (map.count(i)) {
            output.push_back('$');
            output.push_back('$');
        }
        output.push_back(text[i]);
    }
    cout<<output<<endl;
}

void replace(string& text, vector<string>& patterns) {
    if (patterns.size() == 0) {
        cout<<text<<endl;
        return;
    }
    int curr_node = 0;
    int i = 0;
    unordered_map<int, pair<int,int>> map;
    for (char& c: text) {
        curr_node = makeTransition(curr_node, c);
        if (aho[curr_node].leaf == true) {
            map[i-aho[curr_node].depth+1] = {aho[curr_node].id, aho[curr_node].depth};
        }
        i++;
    }
    int len = text.length();
    string output = "";
    i = 0;
    while (i<len) {
        if (map.count(i)) {
            for (char c: patterns[map[i].first]) {
                output.push_back(c);
            }
            i += map[i].second;
        }
        else {
            output.push_back(text[i]);
            i++;
        }
    }
    cout<<output<<endl;
}

int main(int argc, const char * argv[]) {
    vector<string> patterns;
    string option1 = "f";
    string option2 = "r";
    if (argv[1] != option1 && argv[1] != option2) {
        cerr<<argv[1]<<endl;
        cerr<<"Incorrect syntax!"<<endl;
        return 0;
    }
    for (int i=2;i<argc;i++) {
        string s(argv[i]);
        patterns.push_back(s);
    }
    if (argv[1] == option1) {
        for (string& pattern: patterns)
            add_string(pattern);
    }
    else {
        int numPatterns = patterns.size();
        if (numPatterns%2 == 1) {
            std::cerr<<"Replace functionality requires a replace word for every keyword !!"<<endl;
            return 0;
        }
        for (int i=0;i<numPatterns-1;i+=2) {
            add_string(patterns[i], i+1);
        }
    }
    string text;
    while(getline(cin,text)) {
        if (argv[1] == option1)
            find(text, patterns);
        else
            replace(text, patterns);
    };
    return 0;
}
