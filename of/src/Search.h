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
#define STEP_SIZE 10

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
	Node(ofxVec2f *_pos);
	Node* getParent();
	void setParent(Node *_p);
	Node* getChild();
	void setChild(Node *_p);
	int getF();
	int getG();
	int getH();
	void setF(int _f);
	void setG(int _g);
	void setH(int _h);
	void setPos(ofxVec2f _pos);
	ofxVec2f getPos();
	float getX();
	float getY();
	Node *parent;
	Node *child;
	ofxVec2f pos;
	int f,g,h;
};



class Astar{
public:
	Astar(Mind *_mind,int _w, int _h);	
	int run(ofxVec2f _goal);
	node_t * findBest(list_t *_list_p);
	void listInit(list_t *_list_p);
	double calculateG(node_t *_node_p);
	double calculateH(node_t *_node_p);
	int isListEmpty(list_t *_list_p);
	void cleanup(list_t *_o_list_p,list_t *_c_list_p); 
	int isListPresent(list_t *_list_p, int _x, int _y);
	void listAdd(list_t *_list_p,  node_t *_elem_p);
	node_t * listGet(list_t *_list_p, int _x, int _y, int _remove);
	node_t * allocateNode(int _x, int _y);
	node_t * getSuccessorNode(node_t *_curr_node, int _i);
	void buildBestPath(node_t *_walker);
	ofxVec2f currPos;
	ofxVec2f goal;
	Mind *mind;
	vector <ofxVec2f> path;
	char *grid;
	int w,h;
	int edge;
	double minCost;
	double alpha;
};

#endif
