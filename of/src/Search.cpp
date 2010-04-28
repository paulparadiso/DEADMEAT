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
	onClosedList = 0;
	onOpenedList = 1;
}

Astar2::Astar2(Mind *_mind,int _w, int _h){
	w = _w;
	h = _h;
	grid = (char*)malloc(w*h*sizeof(char));
	float costs[8] = {1.0,1.0,1.0,1.0,1.4,1.4,1.4,1.4};
	for(int i = 0; i < 8; i++){
		cost[i] = costs[i];
	}
	pathGenerated = 0;
	minCost = 1.0;
	path.resize(100000);
	edge = STEP_SIZE;
	mind = _mind;
	currPos = mind->body->pos;
	spaceInited = 0;
	atStart = 1;
	initSpace();
}

int Astar2::run(ofxVec2f _goal){
	Node * startNode;
	if(atStart){
		initSpace();
		currPos = nearestPoint(mind->body->pos, STEP_SIZE);
		startNode = spaceGet(currPos.x,currPos.y);
		goal = _goal;
		openedList.clear();
		startNode->onOpenedList=1;
		openedList.push_back(startNode);
		atStart = 0;
	} else {
		currPos = lastPos;
		startNode = spaceGet(currPos.x,currPos.y);
	}
	isEmpty=0;
	numSearched = 0;
	//printf("start node = %d,%d\n",startNode->getX(),startNode->getY());
	while(!openedList.empty() && numSearched < MAX_SEARCH){
		Node * successor;
		//Node * currNode = spaceFindBest();
		Node * currNode = openedList.back();
		lastPos.set(currNode->getX(),currNode->getY());
		numSearched++;
		//printf("currNode = %d,%d\n",currNode->getX(),currNode->getY());
		if(isGoalNode(currNode)){
			printf("Found best path\n");
			buildBestPath(currNode);
			atStart = 1;
			pathGenerated = 1;
			return 1;
		} else {
			currNode->onClosedList = 1;
			currNode->onOpenedList = 0;
			openedList.pop_back();
			for(int i = 0; i < 8; i++){
				successor = getSuccessorNode(currNode, i);
				if(successor != (Node*)NULL){
					Node tmp = *successor;
					tmp.setH(calculateH(&tmp));
					tmp.setG(currNode->getG() + calculateG(&tmp,i));
					tmp.setF(tmp.getG() + tmp.getH());
					if(successor->onOpenedList || successor->onClosedList){
						if(successor->getF() < tmp.getF())
							continue;
					}
					successor->setF(tmp.getF());
					successor->setG(tmp.getG());
					successor->setH(tmp.getH());
					successor->parent = currNode;
					if(!successor->onOpenedList)
						listInsert(&openedList,successor);
					successor->onOpenedList = 1;					
				}
			}
		}
	}
	return 0;
}

void Astar2::initSpace(){
	if(!spaceInited){
		for(int i = 0; i < h; i += STEP_SIZE){
			for(int j = 0; j < w; j += STEP_SIZE){
				//printf("Making node at %d,%d\n",j,i);
				space.push_back(Node(j,i));
			}
		}
		spaceInited = 1;
	} else {
		vector<Node>::iterator ni;
		for(ni = space.begin(); ni != space.end(); ++ni){
			ni->setF(10000.0);
			ni->setG(10000.0);
			ni->setH(10000.0);
			ni->onOpenedList = 0;
			ni->onClosedList = 0;
			ni->parent = (Node*)NULL;
		}
	}
}


Node * Astar2::spaceGet(int _x, int _y){
	Node *tmp = &space.at((_x/STEP_SIZE) + (_y/STEP_SIZE) * (w/STEP_SIZE));
	return tmp;
}
		
void Astar2::buildBestPath(Node *_walker){
	int x,y;
	int c = 0;
	Node *tester = _walker;
	while(tester->parent != (Node*)NULL){
		++c;
		tester = tester->parent;
	}
	path.resize(c);
	path.clear();
	printf("Making Path.\n");
	int gs = ((int)goal.x + (int)goal.y * w);
	while(_walker->parent != (Node*)NULL){
		path.push_back(ofxVec2f((float)_walker->getX(),(float)_walker->getY()));
		//printf("\tAdding %d,%d\n",_walker->getX(),_walker->getY());
		_walker = (Node*)_walker->parent;
	}
}

int Astar2::isGoalNode(Node *_node){
	int xDist = abs(goal.x - _node->getX());
	int yDist = abs(goal.y - _node->getY());
	if(xDist < STEP_SIZE * 2 && yDist < STEP_SIZE)
		return 1;
	else
		return 0;
}

Node * Astar2::spaceFindBest(){
	vector<Node>::iterator ni;
	Node *node;
	node = 0;
	for(ni = space.begin(); ni != space.end(); ++ni){
		if(ni->onOpenedList){
			if(node == (Node*)NULL)
				node = &(*ni);
			else
				if(ni->getF() < node->getF())
					node = &(*ni);
		}
	}
	if(node == 0)
		isEmpty = 1;
	//printf("Sending back a node at = %d,%d\n",node->getX(),node->getY());
	return node;
}

void Astar2::listInsert(nodeList *_list, Node *_node){
//	printf("Inserting: %d,%d --> %f\n",_node->getX(),_node->getY(),_node->getF());
	if(_list->empty()){
		//printf("It's empty\n");
		_list->push_back(_node);
		return;
	}
	nodeList::iterator ni;
	//for(ni = _list->begin(); ni != _list->end(); ++ni){
//		printf("list item %d,%d --> %f\n",(*ni)->getX(),(*ni)->getY(),(*ni)->getF());
//	}
	if(_list->size()==1){
		Node *tmp = _list->back();
		if(tmp->getF() > _node->getF()){
			_list->push_back(_node);
			return;
		} else {
			_list->pop_back();
			_list->push_back(_node);
			_list->push_back(tmp);
			return;
		}
	}
	Node * tmp = _list->front();
	if(tmp->getF() < _node->getF()){
		_list->insert(_list->begin(),_node);
		return;
	}
	for(ni = _list->begin(); ni != _list->end() -1; ++ni){
		//printf("\t%f\n",(*ni)->getF());
		if((_node->getF() < (*ni)->getF()) && (_node->getF() > (*(ni+1))->getF())){
			_list->insert(ni,_node);
			return;
		}
	}
	_list->push_back(_node);
	return;
}

double Astar2::calculateG(Node *_node,int _i){
	return cost[_i] + (ALPHA * (_node->getG() - 1.0));
}

double Astar2::calculateH(Node *_node){
	return (double)(MIN_COST *
					(abs((double)_node->getX() - (double)goal.x) +
					 abs((double)_node->getY() - (double)goal.y)));
}

Node * Astar2::getSuccessorNode(Node *_currNode, int _i){
	Node *node = (Node*)NULL;
	int x,y;
								   
	x = (int)_currNode->getX() + succ[_i].x * STEP_SIZE;
	y = (int)_currNode->getY() + succ[_i].y * STEP_SIZE;
	
	if(mind->obs->isWalkable(x, y, _currNode->getX(), _currNode->getY(), STEP_SIZE)){
		node = spaceGet(x,y);
	}
	return node;
}

ofxVec2f Astar2::nearestPoint(ofxVec2f _cPos, int _div){
	/*
	Find the nearest point the players current position that is 
	 on the main grid.
	 */
	int xDiv = _cPos.x / _div;
	int xMod = (int)_cPos.x % _div;
	int yDiv = _cPos.y / _div;
	int yMod = (int)_cPos.y % _div;
	int xPlus, yPlus;
	if (xMod < _div / 2){ xPlus = 0; }
	else xPlus = _div;
	if (yMod < _div / 2){ yPlus = 0; }
	else yPlus = _div;
	int newX = xDiv * _div + xPlus; 
	int newY = yDiv * _div + yPlus;
	if (newX <= 0) newX = _div;
	if (newX >= w) newX = w - _div;
	if (newY <= 0) newY = _div;
	if (newY >= h) newY = h - _div;
	return ofxVec2f(newX,newY);
}

