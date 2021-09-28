//#include <iostream>
//#include <fstream>
//#include <utility>
//#include <cmath>
//#include <vector>
//#include <regex>
//#include "EdmondsMatching.hpp"
//#include <unistd.h>
//#include "obstacle.h"
//#include "parse.h"

using namespace std;

//string path=""
//int sink_num;

//double Eu_dist (const Point& p1, const Point& p2) {
//    return sqrt(pow((p1.x - p2.x), 2)+pow((p1.y - p2.y), 2));
//}
//pair <int,int> Merge2Node (Point cell1, Point cell2, double r);

double Get_2p_r(vector<Point> cell,const string& result) {
	ifstream fin(result);
	double m, n, r_max = 0, r;
    while (!fin.eof()) {
        fin >> m >> n ;
        r = Manhattan_arc(cell[m], cell[n])/2;
        if (r > r_max)
            r_max = r;
    }
    cout << "2P_R ======= " << r_max << endl;
    return r_max+20;
}



class TrrType {
//    Point p;
	public:
        Point center;
        Point top;
        Point bottom;
        Point left;
        Point right;

    public:
	  void Build_TRR(const Point& p, double r) {
        center.x = p.x;
        center.y = p.y;
        top.x = p.x;
        top.y = p.y + r;
        bottom.x = p.x;
        bottom.y = p.y - r;
        left.x = p.x - r;
        left.y = p.y;
        right.x = p.x + r;
        right.y = p.y;
    }

    TrrType MakeIntersect(TrrType& trr1, TrrType& trr2) {
        TrrType TRR;
        if((trr1.top.y < trr2.top.y) && (trr1.bottom.y > trr2.bottom.y) && (trr1.left.x > trr2.left.x) && (trr1.right.x < trr2.right.x))
            return trr1;
        if((trr1.top.y > trr2.top.y) && (trr1.bottom.y < trr2.bottom.y) && (trr1.left.x < trr2.left.x) && (trr1.right.x > trr2.right.x))
            return trr2;
        if ((trr1.center.x < trr2.center.x) && (trr1.bottom.y < trr2.center.y) && (trr2.center.y < trr1.top.y ))
            {
                int b = abs((trr2.left.y - trr2.left.x) - (trr1.right.y - trr1.right.x))/2;
                TRR.left = trr2.left;
                TRR.right = trr1.right;
                TRR.top.x = TRR.right.x - b;
                TRR.top.y = TRR.right.y + b;
                TRR.bottom.x = TRR.left.x + b;
                TRR.bottom.y = TRR.left.y - b;
                TRR.center.x = (TRR.left.x + TRR.right.x)/2;
                TRR.center.y = (TRR.left.y + TRR.right.y)/2;
                return TRR;
            }
        else if ((trr1.center.x > trr2.center.x) && (trr1.bottom.y < trr2.center.y) && (trr2.center.y < trr1.top.y))
            {
                int b = abs((trr2.right.y - trr2.right.x) - (trr1.left.y - trr2.left.x))/2;
                TRR.left = trr1.left;
                TRR.right = trr2.right;
                TRR.top.x = TRR.right.x - b;
                TRR.top.y = TRR.right.y + b;
                TRR.bottom.x = TRR.left.x + b;
                TRR.bottom.y = TRR.left.y - b;
                TRR.center.x = (TRR.left.x + TRR.right.x)/2;
                TRR.center.y = (TRR.left.y + TRR.right.y)/2;
                return TRR;
            }
        else if ((trr1.center.y > trr2.center.y) && (trr1.left.x < trr2.center.x) && (trr2.center.x < trr1.right.x))
            {
                int b = abs((trr2.right.y + trr2.right.x) - (trr1.left.y + trr1.left.x))/2;
                TRR.top = trr2.top;
                TRR.bottom = trr1.bottom;
                TRR.left.x = TRR.top.x - b;
                TRR.left.y = TRR.top.y - b;
                TRR.right.x = TRR.bottom.x + b;
                TRR.right.y = TRR.bottom.y + b;
                TRR.center.x = (TRR.top.x + TRR.bottom.x)/2;
                TRR.center.y = (TRR.top.y + TRR.bottom.y)/2;
                return TRR;
            }
        else if ((trr1.center.y < trr2.center.y) && (trr1.left.x < trr2.center.x) && (trr2.center.x < trr1.right.x))
            {
                int b = abs((trr2.right.y + trr2.right.x) - (trr1.left.y + trr1.left.x))/2;
                TRR.top = trr1.top;
                TRR.bottom = trr2.bottom;
                TRR.left.x = TRR.top.x - b;
                TRR.left.y = TRR.top.y - b;
                TRR.right.x = TRR.bottom.x + b;
                TRR.right.y = TRR.bottom.y + b;
                TRR.center.x = (TRR.top.x + TRR.bottom.x)/2;
                TRR.center.y = (TRR.top.y + TRR.bottom.y)/2;
                return TRR;
            }
    }

//    pair <double,double> mid_value_2point(pair<Point,Point> P) {
//        return make_pair (int((P.first.x + P.second.x)/2), int((P.first.y + P.second.y)/2));
//    }
};




pair <int,int> Merge2Node (Point cell1, Point cell2, double r)
{
    TrrType Trr1, Trr2, Trr3, Trr;
    Point parent_cell;
//    pair <Point,Point> current_point_pair;
    pair <double,double> current_point_value;


        Trr1.Build_TRR(cell1,r);
        Trr2.Build_TRR(cell2,r);
        Trr = Trr3.MakeIntersect(Trr1,Trr2);




        if(1)
        {
//            current_point_value=Trr.mid_value_2point(current_point_pair);
              current_point_value.first = Trr.center.x;
              current_point_value.second = Trr.center.y;
//              cout<<"xxxxxxxxxxxx= "<<Trr.center.x<<endl;
//              cout<<"yyyyyyyyyyyy= "<<Trr.center.y<<endl;
        }

        if(overlap_check(obs,current_point_value) == 0)
        {cout<< "fail!!!" << "value check==================="<<current_point_value.first  <<"   "<<current_point_value.second<<endl;}

        parent_cell.x=current_point_value.first;
        parent_cell.y=current_point_value.second;

//        cout<<"testin22222222222222"<<endl;
        return make_pair(parent_cell.x,parent_cell.y);
}


//vector<Point> Merge2Node (Point cell1, Point cell2, double r, vector<pair<int,int>> ed_method)
//{
//    TrrType Trr1, Trr2, Trr;
//    int counter;
//    pair <int,int> merge_pair;
//    Point parent_cell;
//    pair <Point,Point> current_point_pair;
//    pair <int,int> current_point_value;
//    vector<Point> store_cell;
//    for(int i=0; i<ed_method.size();i++)
//    {
//        merge_pair.first=ed_method[i].first;
//        merge_pair.second=ed_method[i].second;
//        Trr1.Build_TRR(cell[merge_pair.first],r);
//        Trr2.Build_TRR(cell[merge_pair.second],r);
//        current_point_pair=Trr.MakeIntersect(Trr1,Trr2);
//        current_point_value=Trr.mid_value_2point(current_point_pair);
//
//        parent_cell.index=i+counter;
//        parent_cell.x=current_point_value.first;
//        parent_cell.y=current_point_value.second;
//        store_cell.push_back(parent_cell);
//    }
//    return store_cell;
//}



