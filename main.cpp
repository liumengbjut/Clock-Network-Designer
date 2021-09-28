
#include <iostream>
#include <fstream>
#include <utility>
#include <cmath>
#include <vector>
#include <regex>
#include <string>
#include <iomanip>
#include <map>
#include <set>
#include <chrono>
#include <thread>
#include <unistd.h>
#include <algorithm>
#include <math.h>
#include "global_var.h"
#include "layout_info.h"
#include "factor1201.h"
#include "parse.h"
#include "obstacle.h"
#include "row.h"
#include "buffer.h"
#include "merge_2points.h"
#include "EdmondsMatching.hpp"
#include "edmondfunc.hpp"
#include "cluster_3points.h"
#include "merge_3points.h"
#include "merge_5points.h"
#include "merge_7points.h"
#include "cluster_5points.h"
#include "cluster_7points.h"
#include "report_tree.h"

using namespace std;

typedef struct TreeNode
{
	int num;
	struct TreeNode *child1;
	struct TreeNode *child2;
	struct TreeNode *child3;
	struct TreeNode *child4;
	struct TreeNode *child5;
	struct TreeNode *child6;
	struct TreeNode *child7;
	struct TreeNode *parent;
	int index;
}TreeNode;

TreeNode *MergeNodes(vector<Point>& cell, vector<int> level_method) {
	vector<vector<int> > matching_2p = { { 0, 1 }, { 2, 3 }, { 4, 5 } };
	vector<vector<int> > matching_3p = { { 6, 7, 8 } };
	vector<vector<int> > matching_5p;
	vector<vector<int> > matching_7p;


	vector<TreeNode*> tmp_node;
	vector<Point> tmp_cell;
	string mnweightfile = "/home/lm/work/symmetrical_clock_tree_20161129/mnweightfile";
	string result = "/home/lm/work/symmetrical_clock_tree_20161129/result";

	for (int i = 0; i < cell.size(); i++) {
		TreeNode *tn = new TreeNode();
		tn->index = cell[i].index;
		tn->num = 0;
		tn->child1 = NULL;
		tn->child2 = NULL;
		tn->child3 = NULL;


		tn->child4 = NULL;
		tn->child5 = NULL;

		tn->child6 = NULL;
		tn->child7 = NULL;

		tn->parent = NULL;
		tmp_node.push_back(tn);

		tmp_cell.push_back(cell[i]);
	}



	for (int i = 0; i < level_method.size(); i++) {
		vector<TreeNode*> save_node;





		if (level_method[i] == 2) {
        vector<Point> save_cell;
        Get_weight2_data(tmp_cell, mnweightfile);
        matching_2p = edmondfunc(mnweightfile,result);
        double r = Get_2p_r(tmp_cell, result);
//        R.push_back(r);
        int buf_num = ceil(r/select_driver_fanout1().dist);
        cout<<"Each path of level two need to insert "<<buf_num<< " buffers" <<endl;


        save_cell = tmp_cell;
        tmp_cell.clear();

//        cout<<"matching1_size = "<<matching1.size()<<endl;
//        cout<<"R = "<<r<<endl;
			for (int j = 0; j < matching_2p.size(); j++) {
				Point *p = new Point();
				p->index = cell.size();
				//get matching1 & matching2
//                cout<<"p->index"<<cell.size()<<endl;

				//how to merge two cell to one
				pair <int,int> par;
				par = Merge2Node(save_cell[matching_2p[j][0]],save_cell[matching_2p[j][1]],r);
				p->x = par.first;
				p->y = par.second;
                p->buf_name = select_driver_fanout2().driver;

                for(int n=0;n<2;n++)
                {
                    report_buf_connection(*p,save_cell[matching_2p[j][n]],r,buf_num);
                }


//				cout<<"get_point_info x="<<p->x<<"y="<<p->y<<endl;
//				cout<<"merge2"<<endl;

				TreeNode *tn = new TreeNode();
				tn->index = cell.size();
				tn->num = 2;
				tn->child1 = tmp_node[j * 2];
				tn->child2 = tmp_node[j * 2 + 1];
				tmp_node[j * 2]->parent = tn;
				tmp_node[j * 2 + 1]->parent = tn;

				cell.push_back(*p);
				save_node.push_back(tn);
                tmp_cell.push_back(*p);
//                cout<<"finish_merge two points"<<endl;
			}
//			save_cell.clear();
		}


		else if (level_method[i] == 3) {
        vector<Point> save_cell;
        vector<vector<int> > graph(500,vector<int>(500));
        graph = Get_weight3_data(graph, tmp_cell);
        Greedy_3P Greedy;
        Greedy.init(tmp_cell);
        matching_3p = Greedy.strategy(graph, tmp_cell);
        double r = Get_3p_r(tmp_cell, matching_3p);
//        R.push_back(r);

        int buf_num = ceil(r/select_driver_fanout1().dist);
        cout<<"Each path of level three need to insert "<<buf_num<< " buffers" <<endl;
//        cout<<"R ======= "<<r<<endl;
        save_cell = tmp_cell;
        tmp_cell.clear();

			for (int j = 0; j < matching_3p.size(); j++) {
				Point *p = new Point();
				p->index = cell.size();
//				cout << "1: " << matching_3p[j][0] << " 2: " << matching_3p[j][1] << " 3: " << matching_3p[j][2] << endl;
//				cout << "1: " << tmp_cell[matching_3p[j][0]].x << " " << tmp_cell[matching_3p[j][0]].y << endl;
//				cout << "2: " << tmp_cell[matching_3p[j][1]].x << " " << tmp_cell[matching_3p[j][1]].y << endl;
//				cout << "3: " << tmp_cell[matching_3p[j][2]].x << " " << tmp_cell[matching_3p[j][2]].y << endl;
				pair <int,int> par;
				par = Merge3Node(save_cell[matching_3p[j][0]],save_cell[matching_3p[j][1]],save_cell[matching_3p[j][2]],r);
//                p->x = (save_cell[matching_3p[j][0]].x + save_cell[matching_3p[j][1]].x + save_cell[matching_3p[j][2]].x) / 3;
//				p->y = (save_cell[matching_3p[j][0]].y + save_cell[matching_3p[j][1]].y + save_cell[matching_3p[j][2]].y) / 3;
//                save_cell.push_back(*p);

				p->x = par.first;
				p->y = par.second;
				p->buf_name = select_driver_fanout3().driver;

                for(int n=0;n<3;n++)
                {
                    report_buf_connection(*p,save_cell[matching_3p[j][n]],r,buf_num);
                }

				TreeNode *tn = new TreeNode();
				tn->index = cell.size();
				tn->num = 3;
				tn->child1 = tmp_node[j * 3];
				tn->child2 = tmp_node[j * 3 + 1];
				tn->child3 = tmp_node[j * 3 + 2];
				tmp_node[j * 3]->parent = tn;
				tmp_node[j * 3 + 1]->parent = tn;
				tmp_node[j * 3 + 2]->parent = tn;

				cell.push_back(*p);
				save_node.push_back(tn);
                tmp_cell.push_back(*p);
			}
		}

		else if (level_method[i] == 5) {
        vector<Point> save_cell;
        vector<vector<int> > graph(500,vector<int>(500));
        graph = Get_weight5_data(graph, tmp_cell);
        Greedy_5P Greedy;
        Greedy.init(tmp_cell);
        matching_5p = Greedy.strategy(graph, tmp_cell);
        double r = Get_5p_r(tmp_cell, matching_5p);
//        R.push_back(r);
//        cout<<"R ======= "<<r<<endl;
        save_cell = tmp_cell;
        tmp_cell.clear();

			for (int j = 0; j < matching_5p.size(); j++) {
				Point *p = new Point();
				p->index = cell.size();
				p->x = (save_cell[matching_5p[j][0]].x + save_cell[matching_5p[j][1]].x + save_cell[matching_5p[j][2]].x + save_cell[matching_5p[j][3]].x + save_cell[matching_5p[j][4]].x) / 5;
				p->y = (save_cell[matching_5p[j][0]].y + save_cell[matching_5p[j][1]].y + save_cell[matching_5p[j][2]].y + save_cell[matching_5p[j][3]].y + save_cell[matching_5p[j][4]].y) / 5;
//                save_cell.push_back(*p);


				TreeNode *tn = new TreeNode();
				tn->index = cell.size();
				tn->num = 5;
				tn->child1 = tmp_node[j * 5];
				tn->child2 = tmp_node[j * 5 + 1];
				tn->child3 = tmp_node[j * 5 + 2];
				tn->child4 = tmp_node[j * 5 + 3];
				tn->child5 = tmp_node[j * 5 + 4];

				tmp_node[j * 5]->parent = tn;
				tmp_node[j * 5 + 1]->parent = tn;
				tmp_node[j * 5 + 2]->parent = tn;
				tmp_node[j * 5 + 3]->parent = tn;
				tmp_node[j * 5 + 4]->parent = tn;

				cell.push_back(*p);
				save_node.push_back(tn);
                tmp_cell.push_back(*p);
			}
		}

        else if (level_method[i] == 7) {
        vector<Point> save_cell;
        vector<vector<int> > graph(500,vector<int>(500));
        graph = Get_weight7_data(graph, tmp_cell);
        Greedy_7P Greedy;
        Greedy.init(tmp_cell);
        matching_7p = Greedy.strategy(graph, tmp_cell);
        double r = Get_7p_r(tmp_cell, matching_7p);
//        R.push_back(r);
//        cout<<"R ======= "<<r<<endl;
        save_cell = tmp_cell;
        tmp_cell.clear();

			for (int j = 0; j < matching_7p.size(); j++) {
				Point *p = new Point();
				p->index = cell.size();
				p->x = (save_cell[matching_7p[j][0]].x + save_cell[matching_7p[j][1]].x + save_cell[matching_7p[j][2]].x + save_cell[matching_7p[j][3]].x
                        + save_cell[matching_7p[j][4]].x + save_cell[matching_7p[j][5]].x + save_cell[matching_7p[j][6]].x) / 7;
				p->y = (save_cell[matching_7p[j][0]].y + save_cell[matching_7p[j][1]].y + save_cell[matching_7p[j][2]].y + save_cell[matching_7p[j][3]].y
                        + save_cell[matching_7p[j][4]].y + save_cell[matching_7p[j][5]].y + save_cell[matching_7p[j][6]].y) / 7;
//                save_cell.push_back(*p);


				TreeNode *tn = new TreeNode();
				tn->index = cell.size();
				tn->num = 7;
				tn->child1 = tmp_node[j * 7];
				tn->child2 = tmp_node[j * 7 + 1];
				tn->child3 = tmp_node[j * 7 + 2];
				tn->child4 = tmp_node[j * 7 + 3];
				tn->child5 = tmp_node[j * 7 + 4];
                tn->child6 = tmp_node[j * 7 + 5];
				tn->child7 = tmp_node[j * 7 + 6];

				tmp_node[j * 7]->parent = tn;
				tmp_node[j * 7 + 1]->parent = tn;
				tmp_node[j * 7 + 2]->parent = tn;
				tmp_node[j * 7 + 3]->parent = tn;
				tmp_node[j * 7 + 4]->parent = tn;
                tmp_node[j * 7 + 5]->parent = tn;
				tmp_node[j * 7 + 6]->parent = tn;

				cell.push_back(*p);
				save_node.push_back(tn);
                tmp_cell.push_back(*p);
			}
		}
		tmp_node = save_node;
	}
//	cout << "End, tmp_node size:" << tmp_node.size() << endl;
	return tmp_node[0];
}


int main()
{   vector<Point> cell;

//    vector<vector<Point>> pcell;
//parse benchmark file
    string cell_benchmark_file = "/mnt/hgfs/work/ed/textfile/r6";
    string cell_data_file = "/mnt/hgfs/work/ed/textfile/data6";
    string test = "/home/lm/work/symmetrical_clock_tree_20161129/Symmetrical_clock_tree/testfile";


    Get_def_data(cell_benchmark_file, cell_data_file);
    cell=Get_cell_data(cell_data_file);

    read_buf_file(buf, filename);
    report_buf(buf);
//tree plan of 2 or 3 nodes
    int level_num;
    vector<int> level_method;
    vector<int> node_number;
    Factor QFactor;
//  cout << "sink num = "<<sink_num<<endl;
	pair<int, int> result = QFactor.NQF(sink_num);
//get level number
	level_num = result.second;
    cout << "all_sink_plan_number: " << result.first << endl;
    cout << "level number: " << result.second << endl;
    level_method = QFactor.element(node_number);
    cout << "level method ===== "<<endl;
    for (auto & c : level_method)
        cout << c <<endl;

    cout << "node number ===== "<<endl;
    for (auto & c : node_number)
        cout << c <<endl;



    TreeNode *root;
	root = MergeNodes(cell, level_method);
	cout << "cell size: " << cell.size() - result.first << endl;

	ofstream testfile(test);
	for(int i = result.first; i < cell.size(); i++) {
		cout << "create_cell[" << i  << "], set_location x: " << cell[i].x << " y: " << cell[i].y << ", buf_info: " << cell[i].buf_name << endl;
//		testfile << "cell[" << i << "], x: " << cell[i].x << " y: " << cell[i].y << endl;
	}




    return 0;
}
