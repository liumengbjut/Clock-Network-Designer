
#include <iostream>
#include <fstream>
#include <utility>
#include <cmath>
#include <vector>
#include <regex>
#include <unistd.h>
#include "global_var.h"
using namespace std;
int sink_num;


double Manhattan_arc (const Point& p1, const Point& p2)
{
    double X, Y;
    X = abs (p1.x - p2.x);
    Y = abs (p1.y - p2.y);
    return abs (X+Y);
}

void Get_def_data(const string& data, const string& cell_data_file)
{
    ifstream fin(data);
    ofstream fout(cell_data_file);
    string line;

    smatch results;
    regex num_pattern("Sink : ([[:digit:].]+)");
    regex xy_pattern("^[[:space:]]*Coordinate : ([[:digit:].]+) ([[:digit:].]+)");
    while(getline(fin,line))
     {
        if (regex_search(line,results,num_pattern))
            fout << results.str(1).c_str() << " ";
        if (regex_search(line,results,xy_pattern))
         {
            fout << results.str(1).c_str() << " ";
            fout << results.str(2).c_str() << endl;
         }
    }
}

vector<Point> Get_cell_data (const string& data)
{
    std::ifstream fin;
    fin.open (data);      //0 x0 y0\n 1 x1 y1\n ... n-1 xn-1 yn-1
    vector<Point> cell;
    Point cell_;
    while (fin >> cell_.index >> cell_.x >> cell_.y)
    {
        cell.push_back(cell_);
    }
    fin.close();
    sink_num = cell.size();
    cout<<"sink_num = "<<sink_num<<endl;
    return cell;
}

void Get_weight2_data(vector<Point>& cell, const string& mnweightfile)
{		//generate edmond's input file
    ofstream fout(mnweightfile);
    fout << cell.size() << " " << (cell.size())*(cell.size() - 1)/2 << endl;
    for (int i = 0;i < cell.size(); i++)
    {
        for (int j = i + 1; j < cell.size(); j++)
        {
            fout<< i << " " << j << " " << Manhattan_arc(cell[i], cell[j]) << endl;
        }

    }
//    cout<<"weight2 data got!"<<endl;
}

vector<vector<int>> Get_weight3_data(vector<vector<int> > graph, vector<Point>& cell)
{		//generate edmond's input file
    for (int i=0; i<cell.size(); i++)
    {
        for (int j=0; j<cell.size(); j++)
        {
            graph[i][j] = Manhattan_arc (cell[i], cell[j]);
        }
    }
    return graph;
//    cout<<"weight3 data-----got!"<<endl;
}

vector<vector<int>> Get_weight5_data(vector<vector<int> > graph, vector<Point>& cell)
{		//generate edmond's input file
    for (int i=0; i<cell.size(); i++)
    {
        for (int j=0; j<cell.size(); j++)
        {
            graph[i][j] = Manhattan_arc (cell[i], cell[j]);
        }
    }
    return graph;
//    cout<<"weight5 data-----got!"<<endl;
}

vector<vector<int>> Get_weight7_data(vector<vector<int> > graph, vector<Point>& cell)
{		//generate edmond's input file
    for (int i=0; i<cell.size(); i++)
    {
        for (int j=0; j<cell.size(); j++)
        {
            graph[i][j] = Manhattan_arc (cell[i], cell[j]);
        }
    }
    return graph;
//    cout<<"weight7 data-----got!"<<endl;
}







