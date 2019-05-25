#include "ExtLGraph.h"
#include<fstream>

void shortestPath(int *path, int n) {
	if (path[n] > -1) {
		shortestPath(path, path[n]);
	}
	cout << n << ",";
}


int main() {
	/*ifstream in;
	in.open("Vex.txt", ios::in);
	if (!in.is_open()) {
		system("pause");
		return 0;
	}

	int attractionNum;  
	in >> attractionNum;

	ExtLGraph extl(attractionNum);

	for (int i = 0; i < attractionNum; i++) {
		int no;
		char attractionName[100];
		char attractionDesc[100];
		in >> no;  in >> attractionName; in >> attractionDesc;
		extl.InsertAttraction(no, attractionName, attractionDesc);
	}
	in.close();

	in.open("Edge.txt", ios::in);
	if (!in.is_open()) {
		system("pause");
		return 0;
	}

	int edgeNum;
	in >> edgeNum;
	for (int i = 0; i < edgeNum; i++) {
		int a1; int a2; int distance;
		in >> a1; in >> a2; in >> distance;
		extl.Insert(a1, a2, distance);
	}
	in.close();*/

	ExtLGraph extl;
	
	int attractionNum = extl.getNum();

	int start, end, distance;
	int* d = new int[attractionNum];
	int* path = new int[attractionNum];

	int totalLength = 0;

	int* nearest = new int[attractionNum];
	int* lowcost = new int[attractionNum];

	cout << "\t\tWelcome to the Attractions System" << endl << endl;
	cout << "\t\tThe fllowing is the No of the Attractio: " << endl;
	extl.showAllAttractions();


	cout << "\t -----------------------Table---------------------" << endl;
	cout << "\t|\t1.Create the Attractions Map              |" << endl;
	cout << "\t|\t2.Search Attraction information           |" << endl;
	cout << "\t|\t3.The road to view all the Attractions    |" << endl;
	cout << "\t|\t4.Searcg the shortest path                |" << endl;
	cout << "\t|\t5.Laying circuit planning                 |" << endl;
	cout << "\t|\t6.Modify attraction information           |" << endl;
	cout << "\t|\t0.Exit the System                         |" << endl;
	cout << "\t ------------------------------------------------" << endl;
	int choose;
	cout << "choose the experiemt: (1~6)    Exit: 0" << endl;
	cin >> choose;
	while (choose) {
			int num;
			if(choose==1){
				extl.showAllAttractionMap();
			}
			else if (choose == 2) {
				cout << "please input the Attraction you want to konw: (0 ~ "<<extl.getNum() - 1<<")";
				cin >> num;
				extl.showAttractions(num);
			}
			else if (choose == 3) {
				cout << "please input the Attraction you want to konw: (0, 1, 2 ,3...)";
				cin >> num;
				extl.ExtDFS(num);
			}
			else if (choose == 4) {
				cout << "please input the start point and the end point: " << endl;
				cout << "start point: ";
				cin >> start;
				cout << "end point: ";
				cin >> end;
				extl.Dijkstra(start, d, path);
				shortestPath(path, end);
				cout << d[end] << "m" << endl;
			}
			else if (choose == 5) {
				extl.Prim(0, nearest, lowcost);
				cout << "the road need to build: " << endl;
				for (int i = 1; i < attractionNum; i++) {
					cout << i << "-" << nearest[i] << " " << lowcost[i] << "m" << endl;
					totalLength += lowcost[i];
				}
				cout << "totalLength: " << totalLength << endl;
				totalLength = 0;
			}
			else if (choose == 6) {
				while (choose != 0) {
					cout << "Edge: 1    " << "Vex: 2  " << "Exit: 0 " << endl;
					cin >> choose;
					while (choose != 0) {
						if (choose == 1) {
							cout << "Insert:1" << endl;
							cout << "Remove:2" << endl;
							cout << "Modify:3" << endl;
							cout << "Exist:0" << endl;
							cin >> choose;
							if (choose == 1) {
								cout << "Please input the start point: ";
								cin >> start;
								cout << "Please input the end point: ";
								cin >> end;
								cout << "Please input the distance: ";
								cin >> distance;
								if (Success == extl.Insert(start, end, distance)) {
									cout << "Insert Success" << endl;
									extl.writeToEdgeTxt();
								}
								else {
									cout << "Insert Failed" << endl;
								}
							}
							else if (choose == 2) {
								cout << "Please input the start point: ";
								cin >> start;
								cout << "Please input the end point: ";
								cin >> end;
								if (Success == extl.Remove(start, end)) {
									cout << "Remove Success" << endl;
									extl.writeToEdgeTxt();
								}
								else {
									cout << "Remove Failed" << endl;
								}
							}
							else if (choose == 3) {
								cout << "Please input the start point: ";
								cin >> start;
								cout << "Please input the end point: ";
								cin >> end;
								cout << "Please input the distance: ";
								cin >> distance;
								if (Success == extl.Modify(start, end, distance)) {
									cout << "Modify Success" << endl;
									extl.writeToEdgeTxt();
								}
							}
							else if (choose == 0) {
								break;
							}
						}
						else if (choose == 2) {
							cout << "Insert:1" << endl;
							cout << "Remove:2" << endl;
							cout << "Modify:3" << endl;
							cout << "Exist:0" << endl;
							cin >> choose;
							if (choose == 1) {
								string name, desc;
								cout << "Please input AttractionName: ";
								cin >> name;
								cout << "Please input AttractionDesc: ";
								cin >> desc;
								if (Success == extl.InsertAttraction(name, desc)) {

								}
							}
							if (choose == 2) {
								cout << "Please input the Attraction No you want to remove: ";
								cin >> num;
								if (Success == extl.RemoveAttraction(num)) {
									cout << "Remove Success" << endl;
									extl.writeToEdgeTxt();
									extl.writeToVexTxt();
								}
								else {
									cout << "Remove Failed" << endl;
								}
							}
							if (choose == 3) {
								string attractionName, attractionDesc;
								cout << "Please input the Attraction No you want to Modify: ";
								cin >> num;
								cout << "Please inpu the Modify Attraction Name: ";
								cin >> attractionName;
								cin >> attractionDesc;
								if (Success == extl.ModifyAttraction(num, attractionName, attractionDesc)) {
									cout << "Modify Success" << endl;
									extl.writeToVexTxt();
								}
							}
							else {
								break;
							}
						}
					}
				}
			}
			else {
			}
		cout << "chose the experient: (1~6)    Exit: 0" << endl;
		cin >> choose;
	}

	delete[] d;
	delete[] path;
	delete[] nearest;
	delete[] lowcost;
	return 0;
}