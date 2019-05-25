#include "ENode.h"
#include "Attraction.h"
#include<queue>
#include <iostream>
#include<string>
#include<stack>
#include<fstream>
#define INFTY 10000;

enum ResultCode { Success, Duplicate, Failure, NotPresent };                                     //枚举数据类型,返回操作的状态
class ExtLGraph {
public:
	ExtLGraph();
	~ExtLGraph();
	ResultCode Insert(int u, int v, int & w);                                                                         //插入边
	ResultCode Remove(int u, int v);                                                                                  //删除边
	ResultCode Modify(int u, int v, int & w);																		//修改边
	ResultCode InsertAttraction(int no, string attractionName, string attractionDesc);    //插入顶点
	ResultCode InsertAttraction(string attractionName, string attractionDesc);              
	ResultCode RemoveAttraction(int no);                                                                     //删除顶点
	ResultCode ModifyAttraction(int no, string attractionName, string attractionDesc);//修改顶点
	bool Exist(int u, int v)const;                                                                                     //判断边是否存在
	void Prim(int k, int* nearest, int * lowcost);															  //普里姆算法,计算最小生成树
	void showAttractions(int no);																				//展示特点编号的景点信息
	int choose(int *d, bool * s);																					//迪杰斯特拉算法辅助函数
	void Dijkstra(int v, int* d, int* path);																	    //迪杰斯特拉算
	void ExtDFS(int v);																								//深度遍历算法
	void writeToVexTxt();																							//将顶点信息写入文件
	void writeToVexTxt(string attractionName, string attractionDesc);						
	void writeToEdgeTxt();																							//将边信息写入文件
	void readFromVexTxt();																						//读取顶点信息
	void readFromEdgeTxt();																						//读取边信息
	void showAllAttractions();																						//展示所有的景点信息
	void showAllAttractionMap();                                                                               //展示邻接表信息
	int getNum();																									    //返回顶点编号
private:
	int n;
	int e;
	ENode ** a;
	Attraction * attraction;
	void ExtDFS(int v, bool *visited);
	stack<int> st;
	ofstream out;
};

ExtLGraph::ExtLGraph() {
	readFromVexTxt();
	readFromEdgeTxt();
}

ExtLGraph::~ExtLGraph() {
	ENode * p, *q;
	for (int i = 0; i < n; i++) {
		p = a[i];
		q = p;
		while (p) {
			p = p->nextArc;
			delete q;
			q = p;
		}
	}
	delete[] a;
}

bool ExtLGraph::Exist(int u, int v)const {
	if (u<0 || v<0 || u>n - 1 || v>n - 1 || u == v)
		return false;
	ENode * p = a[u];
	while (p&& p->adjVex != v)
		p = p->nextArc;
	if (!p) return false;
	else return true;
}

ResultCode ExtLGraph::InsertAttraction(int no, string attractionName, string attractionDesc) {
	if (no < 0 || no > n) {
		return NotPresent;
	}
	attraction[no].attractionDesc = attractionDesc;
	attraction[no].attractionName = attractionName;
	attraction[no].no = no;
	return Success;
}

ResultCode ExtLGraph::InsertAttraction(string attractionName, string attractionDesc) {
	for (int i = 0; i < n; i++) {
		if (attraction[i].attractionName == "empty") {
			attraction[i].attractionName = attractionName;
			attraction[i].attractionDesc = attractionDesc;
			writeToVexTxt();
			cout << "Insert Success, the Attraction No is " << i<<endl;
			return Success;
		}
	}

	n++;
	writeToVexTxt(attractionName, attractionDesc);
	readFromVexTxt();
	readFromEdgeTxt();
	cout << "Insert Success, the Attraction No is " << n - 1 << endl;
	return Success;
}

ResultCode ExtLGraph::RemoveAttraction(int no) {
	if (no < 0 || no > n || attraction[no].attractionName == "empty") {
		return NotPresent;
	}
	attraction[no].attractionDesc = "empty";
	attraction[no].attractionName = "empty";

	for (int i = 0; i < n; i++) {
		Remove(i, no);
		Remove(no, i);
	}
	return Success;
}

ResultCode ExtLGraph::ModifyAttraction(int no, string attractionName, string attractionDesc) {
	if (no < 0 || no > n || attraction[no].attractionName == "empty") {
		return NotPresent;
	}
	attraction[no].attractionDesc = attractionDesc;
	attraction[no].attractionName = attractionName;
	return Success;
}

ResultCode ExtLGraph::Insert(int u, int v, int & w) {
	if (u<0 || v<0 || u>n - 1 || v>n - 1 || u == v || attraction[u].attractionName=="empty" || attraction[v].attractionName=="empty")
		return Failure;
	if (Exist(u, v))
		return Duplicate;
	ENode* p = new ENode(v, w, a[u]);
	a[u] = p;
	e++;
	return Success;
}

ResultCode ExtLGraph::Modify(int u, int v, int & w) {
	if (u<0 || v<0 || u>n - 1 || v>n - 1 || u == v)
		return Failure;
	if (!Exist(u, v))
		return NotPresent;

	for (ENode * p = a[u]; p; p = p->nextArc) {
		if (p->adjVex == v) {
			p->w = w;
			return Success;
		}
	}
}

ResultCode ExtLGraph::Remove(int u, int v) {
	if (u<0 || v<0 || u>n - 1 || v>n - 1 || u == v) return Failure;
	ENode* p = a[u], *q = NULL;
	while (p && p->adjVex != v) {
		q = p; p = p->nextArc;
	}
	if (!p) return NotPresent;
	if (q) q->nextArc = p->nextArc;
	else a[u] = p->nextArc;
	delete p; e--;
	return Success;
}

void ExtLGraph::ExtDFS(int v) {
	bool * visited = new bool[n];
	for (int i = 0; i < n; i++) visited[i] = false;
	ExtDFS(v, visited);
}

void ExtLGraph::ExtDFS(int v, bool *visited) {
	if (!visited[v]) {
		st.push(v);                                           //景点没访问过压入栈中,并设置为true
		visited[v] = true;                                  
	}
	for (ENode *p = a[v]; p; p = p->nextArc) {        //遍历邻接表，递归调用该方法
		if (!visited[p->adjVex]) {
			ExtDFS(p->adjVex, visited);
		}
	}
	int size = st.size();
	if (size != n && size != 0) {                               //判断栈的大小，数量不足则代表没能访问所有顶点，弹出顶点元素
		int m = st.top();
		st.pop();
		visited[m] = false;                                        //弹出后设置为false
	}
	else {                                                              //数量等于顶点数符合，输出
		stack<int> s2;
		int *road = new int[n];
		int count = 0;
		s2 = st;
		while (!s2.empty()) {
			road[count] = s2.top();
			s2.pop();
			count++;
		}
		for (int i = n-1; i >=0; i--) {
			cout << road[i] << " ";
		}
		delete[] road;
		int m = st.top();
		st.pop();
		visited[m] = false;
		cout << endl;
	}
}

void ExtLGraph::Prim(int k, int* nearest, int* lowcost) {
	int i, j;
	bool* mark = new bool[n];
	ENode *p;
	if (k<0 || k>n - 1) {
		cout << "OutofBounds";  return;
	}
	for (int i = 0; i<n; i++) {
		mark[i] = false;
		lowcost[i] = INFTY;  nearest[i] = -1;
	}
	mark[k] = true;  lowcost[k] = 0;  nearest[k] = k;
	for (i = 1; i<n; i++) {
		for (p = a[k]; p; p = p->nextArc) {
			j = p->adjVex;  //修改lowcost和nearest的值
			if ((!mark[j]) && (lowcost[j]>p->w)) {
				lowcost[j] = p->w;  nearest[j] = k;
			}
		}
		int min = INFTY;    //求下一条最小权值的边
		for (j = 0; j<n; j++)
			if ((!mark[j]) && (lowcost[j]<min)) {
				min = lowcost[j]; k = j;
			}
		mark[k] = true;       //将顶点k加到生成树上
	}
}

void ExtLGraph::showAttractions(int no) {
	cout << attraction[no].attractionName << " " << attraction[no].attractionDesc << endl;
	ENode *p = a[no];
	for (ENode *p = a[no]; p; p = p->nextArc) {
		cout << p->adjVex <<" "<< p->w << "m"<< " " << attraction[p->adjVex].attractionName << " " << attraction[p->adjVex].attractionDesc << endl;
	}
}

int ExtLGraph::choose(int *d, bool * s) {
	int i, minpos; int min;
	min = INFTY;
	minpos = -1;
	for (i = 1; i<n; i++)                             //遍历找出当前最短路径的终点序号
		if (d[i]<min && !s[i]) {
			min = d[i];
			minpos = i;
		}
	return minpos;
}

void ExtLGraph::Dijkstra(int v, int* d, int* path) {                   //d[i] 记录从源点vx到顶点vi的“当前最短的”路径长度值,
																						    //path[i] 记录从源点vx到顶点vi的“当前最短的”路径上倒数第二个顶点的序号
	int i, k, w;
	if (v < 0 || v > n - 1) {
		cout << "Out of Bounds";
		return;
	}


	//初始化
	bool * s = new bool[n];
	for (int i = 0; i < n; i++) {
		s[i] = false;
		d[i] = INFTY;
		path[i] = -1;
	}

	for (ENode *p = a[v]; p; p = p->nextArc) {
		d[p->adjVex] = p->w;
		path[p->adjVex] = v;
	}

	s[v] = true;

	for (int i = 0; i < n; i++) {
		//确定第i条最短路径的终点序号k
		k = choose(d, s);
		if (k == -1)
			return;
		s[k] = true;

		//修正源点到其余各顶点的最短路径      
		for (ENode *p = a[k]; p; p = p->nextArc) {
			if (!s[p->adjVex] && (d[k] + p->w) < d[p->adjVex]) {
				d[p->adjVex] = d[k] + (p->w);
				path[p->adjVex] = k;
			}
		}
	}

}

void ExtLGraph::writeToVexTxt() {
	out.open("Vex.txt", ios::out);
	if (!out.is_open()) {
		return;
	}

	out << n << endl;
	for (int i = 0; i < n; i++) {
		out << attraction[i].no << endl;
		out << attraction[i].attractionName << endl;
		out << attraction[i].attractionDesc << endl;
	}
	out.close();
}

void ExtLGraph::writeToVexTxt(string attractionName, string attractionDesc) {
	out.open("Vex.txt", ios::out);
	if (!out.is_open()) {
		return;
	}

	out << n << endl;
	for (int i = 0; i < (n-1); i++) {
		out << attraction[i].no << endl;
		out << attraction[i].attractionName << endl;
		out << attraction[i].attractionDesc << endl;
	}

	out << n - 1 << endl;
	out << attractionName << endl;
	out << attractionDesc << endl;
	out.close();
}

void ExtLGraph::writeToEdgeTxt() {
	out.open("Edge.txt", ios::out);
	if (!out.is_open()) {
		return;
	}

	out << e << endl;
	for (int i = 0; i < n; i++) {
		for (ENode*p = a[i]; p; p = p->nextArc) {
			out << i << " " << p->adjVex << " " << p->w << endl;
		}
	}
	out.close();
}

void ExtLGraph::readFromVexTxt() {
	ifstream in;
	in.open("Vex.txt", ios::in);
	if (!in.is_open()) {
		return;
	}

	int attractionNum;
	in >> attractionNum;
	n = attractionNum;
	e = 0;
	a = new ENode*[n];
	attraction = new Attraction[n];
	for (int i = 0; i < n; i++) {
		a[i] = NULL;
	}

	for (int i = 0; i < attractionNum; i++) {
		int no;
		char attractionName[100];
		char attractionDesc[100];
		in >> no;  in >> attractionName; in >> attractionDesc;
		InsertAttraction(no, attractionName, attractionDesc);
	}
	in.close();
}

void ExtLGraph::readFromEdgeTxt() {
	ifstream in;
	in.open("Edge.txt", ios::in);
	if (!in.is_open()) {
		return;
	}

	int edgeNum;
	in >> edgeNum;
	for (int i = 0; i < edgeNum; i++) {
		int a1; int a2; int distance;
		in >> a1; in >> a2; in >> distance;
		Insert(a1, a2, distance);
	}
	in.close();
}

int ExtLGraph::getNum() {
	return n;
}

void ExtLGraph::showAllAttractions() {
	for (int i = 0; i < n; i++) {
		cout <<"\t\t\t"<<attraction[i].no << "----------------" << attraction[i].attractionName << endl;
	}
}

void ExtLGraph::showAllAttractionMap() {
	for (int i = 0; i < n; i++) {
		cout << "\t\t\t" << i;
		for (ENode *p = a[i]; p; p = p->nextArc) {
			cout << "->"<<p->adjVex;
		}
		cout << endl;
	}
}

