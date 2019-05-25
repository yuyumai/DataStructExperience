#pragma once
#include<iostream>
using namespace std;

struct ENode
{
	int adjVex;
	int w;
	ENode* nextArc;
	ENode() { nextArc = NULL; adjVex = 0;  }
	ENode(int vertex, int weight, ENode* next) {
		adjVex = vertex; w = weight; nextArc = next;
	}
};