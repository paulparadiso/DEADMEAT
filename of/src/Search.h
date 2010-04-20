/*
 *  Search.h
 *  emptyExample
 *
 *  Created by Paul Paradiso on 2/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _SEARCH_H_
#define _SEARCH_H_

#include "ofxVectorMath.h"
#include "Obstacles.h"
#include "Mind.h"
#include <vector>

#define MAX_LIST 1000
#define Y_MAX 15
#define X_MAX 15
#define MIN_COST (double)1.0
#define ALPHA (double)0.5
#define GET 0
#define REMOVE 1

class Mind;

struct node_s;

typedef struct node_s{
	struct node_s *parent;
	double g;
	double h;
	double f;
	int x;
	int y;
} node_t;
	
typedef struct {
	int num_elems;
	node_t *elem[MAX_LIST];
} list_t;

const struct {
	int x;
	int y;
} succ[4] = {{0,-1},{0,1},{1,0},{-1,0}};

class Node{
public:
	Node(int _x, int _y);
	
	ofxVec2f pos;
	ofxVec3f vals;
	
	int getX(){return (int)pos.x;}
	int getY(){return (int)pos.y;}
	float getF(){return vals.x;}
	float getG(){return vals.y;}
	float getH(){return vals.z;}
	
	void setX(int _x){pos.x = _x;}
	void setY(int _y){pos.y = _y;}
	void setF(float _f){vals.x = _f;}
	void setG(float _g){vals.y = _g;}	
	void setH(float _h){vals.z = _h;}
	
	Node *parent;
};

typedef vector<Node*> nodeList;

class Astar2{
public:
	Astar2(Mind *_mind, int _w, int _h);
	int run(ofxVec2f _goal);
	Node * findBest(nodeList *_list);
	void listInit(nodeList *_list);
	double calculateG(Node* _node);
	double calculateH(Node* _node);
	int isListEmpty(nodeList *_list);
	int isListPresent(nodeList *_list, int _x, int _y);
	Node * listAdd(nodeList *_list, Node *_node,int _l);
	Node * listGet(nodeList *_list, int _x, int _y, int _remove);
	void listRemove(nodeList, int _x, int _y);
	Node allocateNode(int _x, int _y);
	Node * getSuccessorNode(Node *_curr_node, int _i);
	void buildBestPath(Node *_walker);
	void initSpace();
	void listSwitch(nodeList *_outList, nodeList *_inList, Node * _node);
	void listRemove(nodeList *_list, int _x, int _y);
	Node * spaceGet(int _x, int _y);
	void spaceSet(int _x, int _y, Node _node);
	ofxVec2f currPos;
	ofxVec2f goal;
	Mind *mind;
	nodeList openedList;
	nodeList closedList;
	vector <ofxVec2f> path;
	vector<Node>space;
	char *grid;
	int w,h;
	int edge;
	double minCost;
	double alpha;
	int spaceInited;
};


#endif
