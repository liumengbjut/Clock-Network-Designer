
using namespace std;
//double Get_3p_r(const vector<Point>& cell, vector<vector<double>> matching);
double Get_3p_r(vector<Point> cell, vector<vector<int>> matching)
{
    vector<double> t_dis;
    double max_dis = 0,tmp_max;
    for(int i=0; i<matching.size(); i++)
    {
        t_dis.push_back(Manhattan_arc(cell[matching[i][0]], cell[matching[i][1]]));
        t_dis.push_back(Manhattan_arc(cell[matching[i][0]], cell[matching[i][2]]));
        t_dis.push_back(Manhattan_arc(cell[matching[i][1]], cell[matching[i][2]]));
        tmp_max = *max_element(t_dis.begin(), t_dis.end());
        t_dis.clear();
        if(max_dis < tmp_max)
        {
            max_dis = tmp_max;
        }
    }
    cout<<"3PR ======== "<<max_dis<<endl;
    return max_dis+20;
}


//double Get_3p_r(vector<Point> cell, vector<vector<int>> matching)
//{
//    vector<double> t_dis;
//    double x_t,y_t,d_t,d_max=0;
//
//    for(int i=0; i<matching.size(); i++)
//    {
//
//        x_t = (cell[matching[i][0]].x + cell[matching[i][1]].x + cell[matching[i][2]].x )/3;
//        y_t = (cell[matching[i][0]].y + cell[matching[i][1]].y + cell[matching[i][2]].y )/3;
//
//        for(int j=0; j<3; j++)
//        {
//            d_t = abs(abs(x_t-cell[matching[i][j]].x) + abs(y_t-cell[matching[i][j]].y));
//            if(d_t > d_max)
//            {
//                d_max = d_t;
//            }
//        }
//
//    }
//    cout<<"3P_R ======== "<<d_max<<endl;
//    return d_max;
//}



pair <int,int> Merge3Node (Point cell1, Point cell2, Point cell3, double r)

{
    TrrType Trr1, Trr2, Trr3, Trr_tmp, Trr;
    Point parent_cell;
//    pair <Point,Point> current_point_pair;
    pair <double,double> current_point_value;



    Trr1.Build_TRR(cell1,r);
    Trr2.Build_TRR(cell2,r);
    Trr3.Build_TRR(cell3,r);
    Trr_tmp = Trr_tmp.MakeIntersect(Trr1,Trr2);
    Trr = Trr.MakeIntersect(Trr3,Trr_tmp);



    if(1)
        {
//            current_point_value=Trr.mid_value_2point(current_point_pair);
              current_point_value.first = Trr.center.x;
              current_point_value.second = Trr.center.y;
//              cout<<"x= "<<Trr.center.x<<endl;
//              cout<<"y= "<<Trr.center.y<<endl;
        }

        if(overlap_check(obs,current_point_value) == 0)
        {cout<< "fail!!!" << "value check==================="<<current_point_value.first  <<"   "<<current_point_value.second<<endl;}

        parent_cell.x=current_point_value.first;
        parent_cell.y=current_point_value.second;

//        cout<<"testin3333333333333"<<endl;
        return make_pair(parent_cell.x,parent_cell.y);
}








//pair <int,int> Merge3Node (Point cell1, Point cell2, Point cell3, double r)
//{
//    vector<double> t_dis;
//    double r3;
//    t_dis.push_back(Manhattan_arc(cell1, cell2));
//    t_dis.push_back(Manhattan_arc(cell1, cell3));
//    t_dis.push_back(Manhattan_arc(cell2, cell3));
//    r3 = *max_element(t_dis.begin(), t_dis.end());
//
//
//
//
//
//
//
//
//
//}

//	ifstream fin(result);
//	double m, n, l, r_max = 0, r;
//    while (!fin.eof()) {
//        fin >> m >> n >> l;
//        if (Manhattan_arc(cell[m],cell[n]) > Manhattan_arc(cell[m], cell[l]))
//            r = Manhattan_arc(cell[m], cell[n]);
//        else
//            r = Manhattan_arc(cell[m], cell[l]);
//        if (r < Manhattan_arc(cell[n], cell[l]))
//            r = Manhattan_arc(cell[n], cell[l]);
//        if (r > r_max)
//            r_max = r;
//    }
//    return r_max;
//}
