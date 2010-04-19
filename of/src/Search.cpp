/*
 *  Search.cpp
 *  emptyExample
 *
 *  Created by Paul Paradiso on 2/15/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "Search.h"

Node::Node(int _x, int _y){
	pos.set(_x,_y);
	parent = (Node*)NULL;
	vals.set(0.0,0.0,0.0);
}

Astar::Astar(Mind *_mind, int _w, int _h){
	w = _w;
	h = _h;
	grid = (char*)malloc(w*h*sizeof(char));
	//memset((void*)grid,0,_w*_h*sizeof(char);
	minCost = 1.0;
	alpha = 0.5;
	edge = EDGE_SIZE;
	mind = _mind;
	//printf("Current Position = %d,%d\n",(currPos.x,currPos.y);
}

Astar2::Astar2(Mind *_mind, int _w, int _h){
	w = _w;
	h = _h;
	grid = (char*)malloc(w*h*sizeof(char));
	minCost = 1.0;
	edge = EDGE_SIZE;
	mind = _mind;
	currPos = mind->body->pos;
}

int Astar::run(ofxVec2f _goal){
	goal = _goal;
	node_t *curr_node_p;
	list_t opened_list_p;
	list_t closed_list_p;
	listInit(&opened_list_p);
	listInit(&closed_list_p);
	node_t *start_node_p = allocateNode((int)currPos.x,(int)currPos.y);
	printf("our start node = %d,%d\n",start_node_p->x,start_node_p->y);	
	printf("our goal node = %d,%d\n",(int)goal.x,(int)goal.y);
	listAdd(&opened_list_p, start_node_p);
	while(!isListEmpty(&opened_list_p)){
		curr_node_p = findBest(&opened_list_p);
		(void)listGet(&opened_list_p, curr_node_p->x,curr_node_p->y,
					  REMOVE);
		//printf("Curr_node is at %d,%d\n",curr_node_p->x,curr_node_p->y);
		listAdd(&closed_list_p, curr_node_p);
		if((curr_node_p->x == (int)goal.x) && (curr_node_p->y == (int)goal.y)){
			printf("FOUND PATH!!!!!!!!\n");
			buildBestPath(curr_node_p);
			//cleanup(&opened_list_p,&closed_list_p);
			return 1;
		} else {
			node_t *succ_p;
			node_t *tmp;
			//printf("Testing %d, %d\n",curr_node_p->x, curr_node_p->y);
			for(int i = 0; i < 4; i++){
				succ_p = getSuccessorNode(curr_node_p,i);
				if(succ_p != (node_t *)NULL){
					//printf("Successor node is at %d,%d\n",succ_p->x,succ_p->y);
					succ_p->h = calculateH(succ_p);
					succ_p->g = curr_node_p->g + calculateG(succ_p);
					succ_p->f = succ_p->g + succ_p->h;
					if(isListPresent(&opened_list_p, succ_p->x, succ_p->y)){	
						tmp = listGet(&opened_list_p, succ_p->x, succ_p->y,GET);
						if(tmp->f < succ_p->f){
							//free(succ_p);
							continue;
						}
					}
					if(isListPresent(&closed_list_p, succ_p->x, succ_p->y)){
					   tmp = listGet(&closed_list_p, succ_p->x, succ_p->y,GET);
					   if(tmp->f < succ_p->f){
						   //free(succ_p);
						   continue;
					   }
					}
					tmp = listGet(&opened_list_p,succ_p->x, succ_p->y,REMOVE);
					//if(tmp) free(tmp);
					succ_p->parent = (node_t *)curr_node_p;
					listAdd(&opened_list_p, succ_p);
				}
			}
		}
	}
	printf("Solution Not Found.\n");
	//cleanup(&opened_list_p,&closed_list_p);
	return 0;
}

int Astar2::run(ofxVec2f _goal){
	currPos = mind->body->pos;
	goal = _goal;
	Node *currNode;
	nodeList openedList;
	nodeList closedList;
	listInit(&openedList);
	listInit(&closedList);
	Node *startNode = new Node((int)currPos.x,(int)currPos.y);
	listAdd(&openedList,startNode);
	while(!isListEmpty(&openedList)){
		//printf("Have a node.\n");
		currNode = findBest(&openedList);
		//printf("currNode = %d,%d\n",currNode->getX(),currNode->getY());
		(void*)listGet(&openedList,currNode->getX(),currNode->getY(),REMOVE);
		listAdd(&closedList, currNode);
		if((currNode->getX() == goal.x) && (currNode->getY() == goal.y)){
			buildBestPath(currNode);
			//printf("VECTORED ASTAR FOUND SOLUTION\n");
			return 1;
		} else {
			Node *successor;
			Node *tmp;
			for(int i = 0; i < 4; i++){
				successor = getSuccessorNode(currNode, i);
				if(successor != (Node*)NULL){
					//printf("Testing a node\n");
					successor->setH(calculateH(successor));
					successor->setG(currNode->getG() + calculateG(successor));
					successor->setF(successor->getG() + successor->getH());
					if(isListPresent(&openedList, successor->getX(), successor->getY())){
					   tmp = listGet(&openedList, successor->getX(), successor->getY(),GET);
					   if(tmp->getF() < successor->getF()){
						   continue;
					   }
					}
					if(isListPresent(&closedList, successor->getX(), successor->getY())){
						tmp = listGet(&closedList, successor->getX(), successor->getY(), GET);
						if(tmp->getF() < successor->getF()){
							continue;
						}
					}
					tmp = listGet(&openedList, successor->getX(), successor->getY(), REMOVE);
					successor->parent = (Node*)currNode;
					listAdd(&openedList, successor);
				}
			}
		}
	}
	printf("Vectored Astar found no solution\n");
	return 0;
}

void Astar::cleanup(list_t *_o_list_p, list_t *_c_list_p){
	for(int i = 0; i < MAX_LIST; i++){
		if(_o_list_p->elem[i]) free(_o_list_p->elem[i]);
		if(_c_list_p->elem[i]) free(_c_list_p->elem[i]);
	}
}

void Astar::buildBestPath(node_t *_walker){
	int x,y;
	path.clear();
	int g_s = ((int)goal.x + (int)goal.y * w);
	grid[g_s] = 'G';
	_walker = (node_t*)_walker->parent;
	while(_walker->parent != (node_t*)NULL){
		path.push_back(ofxVec2f((float)_walker->x,(float)_walker->y));
		_walker = (node_t *)_walker->parent;
	}
	//grid[start_node_p->x][start_node_p->y] = 'S';
}

void Astar2::buildBestPath(Node *_walker){
	int x,y;
	path.clear();
	int gs = ((int)goal.x + (int)goal.y * w);
	while(_walker->parent != (Node*)NULL){
		path.push_back(ofxVec2f((float)_walker->getX(),(float)_walker->getY()));
		_walker = (Node*)_walker->parent;
	}
	//printf("Have Path\n");
	vector<ofxVec2f>::iterator pi;
	//for(pi = path.begin(); pi != path.end() -1; ++pi){
//		printf("\t%f,%f\n",pi->x,pi->y);
//	}
}
	
node_t * Astar::findBest(list_t *_list_p){
	int i;
	int best = -1;
	for(i = 0; i < MAX_LIST; i++){
		if(_list_p->elem[i] != 0){
			best = i++;
			break;
		}
	}
	for(;i < MAX_LIST; i++){
		if(_list_p->elem[i] != 0){
			if(_list_p->elem[i]->f < _list_p->elem[best]->f){
				best = i;
			}
		}
	}
	return _list_p->elem[best];
}

Node * Astar2::findBest(nodeList *_list){
	nodeList::iterator ni;
	ni = _list->begin();
	Node *best = (*ni++);
	for(; ni != _list->end(); ++ni){
		if((*ni)->getF() < best->getF())
			best = (*ni);
	}
	return best;
}

double Astar::calculateG(node_t *_node_p){
	double g;
	g = 1.0 + ALPHA * (_node_p->g - 1.0);
	return g;
}

double Astar2::calculateG(Node *_node){
	return 1.0 + (ALPHA * (_node->getG() - 1.0));
}

double Astar::calculateH(node_t *_node_p){
	double h;
	h = (double)(MIN_COST *
				 (abs((double)_node_p->x - (double)goal.x) +
				  abs((double)_node_p->y - (double)goal.y)) );
	return h;
}

double Astar2::calculateH(Node *_node){
	return (double)(MIN_COST *
					(abs((double)_node->getX() - (double)goal.x)+
					 abs((double)_node->getY() - (double)goal.y)));
}

void Astar::listInit(list_t *_list_p){
	_list_p->num_elems = 0;
	for(int i = 0; i < MAX_LIST; i++){
		_list_p->elem[i] = (node_t *)NULL;
		
	}
	return;
}


void Astar2::listInit(nodeList *_list){
	_list->clear();
}

int Astar::isListEmpty(list_t *_list_p){
	if(_list_p->num_elems == 0) return 1;
	else return 0;
}

int Astar2::isListEmpty(nodeList *_list){
	return _list->empty();
}

int Astar::isListPresent(list_t *_list_p, int _x, int _y){
	for(int i = 0; i < MAX_LIST; i++){
		if(_list_p->elem[i]){
			if((_list_p->elem[i]->x == _x) && (_list_p->elem[i]->y == _y)){
			//printf("Found node %d,%d\n",_list_p->elem[i]->x,_list_p->elem[i]->y);
			return 1;
			}
		}
	}
	return 0;
}

int Astar2::isListPresent(nodeList *_list, int _x, int _y){
	nodeList::iterator ni;
	for(ni = _list->begin(); ni != _list->begin(); ++ni){
		if (((int)(*ni)->getX() == _x) && ((int)(*ni)->getY() == _y))
			return 1;
	}
	return 0;
}

void Astar::listAdd(list_t *_list_p,  node_t *_elem_p){
	for(int i = 0; i < MAX_LIST; i++){
		if (_list_p->elem[i] == (node_t *)NULL) {
			_list_p->elem[i] = _elem_p;
			_list_p->num_elems++;
			break;
		}
	}
	return;
}

void Astar2::listAdd(nodeList *_list, Node *_node){
	_list->push_back(_node);
}

node_t * Astar::listGet(list_t *_list_p, int _x, int _y, int _remove){
	node_t *node = (node_t *)NULL;
	for(int i = 0; i < MAX_LIST; i++){
		if(_list_p->elem[i]){
			if((_list_p->elem[i]->x == _x) && (_list_p->elem[i]->y == _y)){
				node = _list_p->elem[i];
				//printf("Get Found node at %d,%d and node = %d\n",node->x,node->y,node);
				if(_remove){
					_list_p->elem[i] = (node_t *)NULL;
					_list_p->num_elems--;
				}
				break;
			}
		}
	}
	//printf("LISTGET RETURNING %d.\n",node);
	return node;
}

Node *Astar2::listGet(nodeList *_list, int _x, int _y, int _remove){
	Node *tmpNode;
	nodeList::iterator ni;
	for(ni = _list->begin(); ni != _list->end() - 1; ni++){
		if(((*ni)->getX() == _x) && ((*ni)->getY() == _y)){
			tmpNode = (*ni);
			if(_remove)
				_list->erase(*ni);
		}
	}
	return tmpNode;
}

node_t * Astar::allocateNode(int _x, int _y){
	node_t *node_p;
	node_p = (node_t *)malloc(sizeof(node_t *));
	
	node_p->g = node_p->h = node_p->f = (double)0.0;
	node_p->parent = (node_t *)NULL;
	node_p->x = _x; 
	node_p->y = _y;
	
	return node_p;
}

node_t * Astar::getSuccessorNode(node_t *_curr_node, int _i){
	node_t *succ_p = (node_t *)NULL;
	int x,y;
	
	x = _curr_node->x + succ[_i].x * STEP_SIZE;
	y = _curr_node->y + succ[_i].y * STEP_SIZE;
	
	if(mind->obs->isWalkable(x,y)){
		succ_p = allocateNode(x,y);
		//printf("Made new node at %d,%d\n",succ_p->x,succ_p->y);
	}
	return succ_p;
}

Node * Astar2::getSuccessorNode(Node *_currNode, int _i){
	Node *node = (Node*)NULL;
	int x,y;
								   
	x = (int)_currNode->getX() + succ[_i].x * STEP_SIZE;
	y = (int)_currNode->getY() + succ[_i].y * STEP_SIZE;
	
	if(mind->obs->isWalkable(x, y)){
		node = new Node(x,y);
	}
	return node;
}



