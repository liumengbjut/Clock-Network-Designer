
//#include "global_var.h"
using namespace std;
double Manhattan_arc (const Point& p1, const Point& p2);
void Get_def_data(const string& data, const string& cell_data_file);
vector<Point> Get_cell_data (const string& data);
void Get_weight2_data(vector<Point>& cell, const string& mnweightfile);
vector<vector<int>> Get_weight3_data(vector<vector<int> > graph, vector<Point>& cell);
vector<vector<int>> Get_weight5_data(vector<vector<int> > graph, vector<Point>& cell);
vector<vector<int>> Get_weight7_data(vector<vector<int> > graph, vector<Point>& cell);
