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

Astar2::Astar2(Mind *_mind,int _w, int _h){
	w = _w;
	h = _h;
	grid = (char*)malloc(w*h*sizeof(char));
	minCost = 1.0;
	edge = STEP_SIZE;
	mind = _mind;
	currPos = mind->body->pos;
	spaceInited = 0;
	initSpace();
}

int Astar2::run(ofxVec2f _goal){
	initSpace();
	currPos = mind->body->pos;
	goal = _goal;
	listInit(&openedList);
	listInit(&closedList);
	Node * currNode;
	Node * successor;
	Node * startNode = spaceGet(currPos.x,currPos.y);
	listAdd(&openedList,startNode,1);
	printf("start node = %d,%d\n",startNode->getX(),startNode->getY());
	while(!isListEmpty(&openedList)){
		currNode = findBest(&openedList);
		if(currNode->getX() == goal.x && currNode->getY() == goal.y){
			buildBestPath(currNode);
			return 1;
		} else {
			//listSwitch(&openedList,&closedList,currNode);
			listAdd(&closedList,currNode,0);
			listRemove(&openedList,currNode->getX(),currNode->getY());
			for(int i = 0; i < 4; i++){
				successor = getSuccessorNode(currNode, i);
				if(successor != (Node*)NULL){
					successor->setH(calculateH(successor));
					successor->setG(currNode->getG() + calculateG(successor));
					successor->setF(successor->getG() + successor->getH());
					if(isListPresent(&openedList, successor->getX(), successor->getY())){
						Node * tmp = listGet(&openedList,successor->getX(),successor->getY(),GET);
						if(tmp->getG() < successor->getG())
							continue;
						else
							listRemove(&closedList, successor->getX(), successor->getY());
					if(isListPresent(&closedList,successor->getX(), successor->getY()))
						Node * tmp = listGet(&openedList,successor->getX(),successor->getY(),GET);
						if(tmp->getG() < successor->getG())
							continue;
						else
							listRemove(&closedList, successor->getX(), successor->getY());
					}
					successor->parent = currNode;
					listAdd(&openedList, successor, 1);
				}	
			}
		}
	}
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
		}
	}
}


Node * Astar2::spaceGet(int _x, int _y){
	Node *tmp = &space.at((_x/STEP_SIZE) + (_y/STEP_SIZE) * (w/STEP_SIZE));
	return tmp;
}

void Astar2::listSwitch(nodeList *_outList, nodeList *_inList, Node *_node){
	_inList->push_back(_node);
	nodeList::iterator ni;
	//printf("Closed List:\n");
//	for(ni = _inList->begin(); ni != _inList->end(); ++ni){
//		printf("\t%d,%d\n",(*ni)->getX(),(*ni)->getY());
//	}
	for(ni = _outList->begin(); ni != _outList->end(); ++ni){
		if((*ni)->getX() == _node->getX() && (*ni)->getX() == _node->getY()){
			printf("About to delete %d,%d\n",(*ni)->getX(),(*ni)->getY());
			_outList->erase(ni);
		}
	}
}

void Astar2::listRemove(nodeList *_list,int _x, int _y){
	nodeList::iterator ni;
	for(ni = _list->begin(); ni != _list->end(); ++ni){
		printf("testing - %d,%d - for removal. looking for - %d,%d\n",(*ni)->getX(),(*ni)->getY(),_x,_y);
		if((*ni)->getX() == _x && (*ni)->getY() == _y){
			printf("About to delete %d,%d\n",(*ni)->getX(),(*ni)->getY());
			_list->erase(ni);
		}
	}
	
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

Node * Astar2::findBest(nodeList *_list){
	nodeList::iterator ni;
	ni = _list->begin();
	Node *best = (*ni++);
	for(; ni != _list->end(); ++ni){
		if((*ni)->getF() < best->getF())
			best = (*ni);
	}
	printf("Found Best - %d,%d\n",best->getX(),best->getY());
	return best;
}

double Astar2::calculateG(Node *_node){
	return 1.0 + (ALPHA * (_node->getG() - 1.0));
}

double Astar2::calculateH(Node *_node){
	return (double)(MIN_COST *
					(abs((double)_node->getX() - (double)goal.x)+
					 abs((double)_node->getY() - (double)goal.y)));
}

void Astar2::listInit(nodeList *_list){
	_list->clear();
}

int Astar2::isListEmpty(nodeList *_list){
	return _list->empty();
}

int Astar2::isListPresent(nodeList *_list, int _x, int _y){
	nodeList::iterator ni;
	for(ni = _list->begin(); ni != _list->begin(); ++ni){
		if (((*ni)->getX() == _x) && ((*ni)->getY() == _y))
			return 1;
	}
	return 0;
}

Node * Astar2::listAdd(nodeList *_list, Node *_node,int _l){
	if(_l)
		printf("About to add item to opened list at %d,%d\n",_node->getX(),_node->getY());
	else
		printf("About to add item to closed list at %d,%d\n",_node->getX(),_node->getY());
	_list->push_back(_node);
	
}

Node * Astar2::listGet(nodeList *_list, int _x, int _y, int _remove){
	Node *tmpNode;
	nodeList::iterator ni;
	for(ni = _list->begin(); ni != _list->end() - 1; ni++){
		//printf("NI = %d,%d\n",(*ni).getX(),(*ni).getY());
		if(((*ni)->getX() == _x) && ((*ni)->getY() == _y)){
			tmpNode = *ni;
			if(_remove){
				printf("About to remove item from list of size = %d at %d,%d\n",_list->size(),(*ni)->getX(),(*ni)->getY());
				_list->erase(ni);
			}
		}
	}
	return tmpNode;
}

Node * Astar2::getSuccessorNode(Node *_currNode, int _i){
	Node *node = (Node*)NULL;
	int x,y;
								   
	x = (int)_currNode->getX() + succ[_i].x * STEP_SIZE;
	y = (int)_currNode->getY() + succ[_i].y * STEP_SIZE;
	
	if(isListPresent(&closedList, x, y))
		return node;
	
	if(mind->obs->isWalkable(x, y)){
		node = spaceGet(x,y);
	}
	return node;
}


//currPos = mind->body->pos;
//goal = _goal;
//Node *currNode;
//listInit(&openedList);
//listInit(&closedList);
//listInit(&allNodes);
//Node startNode(currPos.x,currPos.y);
//listAdd(&openedList,startNode,1);
//while(!isListEmpty(&openedList)){
//	//printf("Have a node.\n");
//	currNode = findBest(&openedList);
//	//printf("currNode = %d,%d\n",currNode->getX(),currNode->getY());
//	listAdd(&closedList, *currNode,0);
//	if(isListPresent(&openedList,currNode->getX(),currNode->getY()));	
//	(void)listGet(&openedList,currNode->getX(),currNode->getY(),REMOVE);
//	if((currNode->getX() == goal.x) && (currNode->getY() == goal.y)){
//		buildBestPath(currNode);
//		//printf("VECTORED ASTAR FOUND SOLUTION\n");
//		return 1;
//	} else {
//		Node successor(-1.0,-1.0);
//		Node tmp(0.0,0.0);
//		for(int i = 0; i < 4; i++){
//			printf("currNode = %d,%d\n",currNode->getX(),currNode->getY());
//			successor = getSuccessorNode(currNode, i);
//			if((successor.getX() > 1.0) && (successor.getY() > 1.0)){
//				printf("successor = %d,%d\n",successor.getX(),successor.getY());
//				//printf("Testing a node\n");
//				successor.setH(calculateH(&successor));
//				successor.setG(currNode->getG() + calculateG(&successor));
//				successor.setF(successor.getG() + successor.getH());
//				if(isListPresent(&openedList, successor.getX(), successor.getY())){
//					tmp = listGet(&openedList, successor.getX(), successor.getY(),GET);
//					if(tmp.getF() < successor.getF()){
//						continue;
//					}
//				}
//				if(isListPresent(&closedList, successor.getX(), successor.getY())){
//					tmp = listGet(&closedList, successor.getX(), successor.getY(), GET);
//					if(tmp.getF() < successor.getF()){
//						continue;
//					}
//				}
//				listAdd(&allNodes, *currNode, 2);
//				Node * nTmp = listGet2(&allNodes,currNode->getX(),currNode->getY());
//				if(isListPresent(&closedList, successor.getX(), successor.getY()))
//					tmp = listGet(&openedList, successor.getX(), successor.getY(), REMOVE);
//					if(isListPresent(&closedList, successor.getX(), successor.getY()))
//						tmp = listGet(&closedList, successor.getX(), successor.getY(), REMOVE);
//						successor.parent = (Node*)nTmp;
//						listAdd(&openedList, successor,1);
//						}
//		}
//	}
//}
//printf("Vectored Astar found no solution\n");
//return 0;
