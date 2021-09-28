using namespace std;

double Get_5p_r(vector<Point> cell, vector<vector<int>> matching)
{
    vector<double> t_dis;
    double x_t,y_t,d_t,d_max=0;

    for(int i=0; i<matching.size(); i++)
    {

        x_t = (cell[matching[i][0]].x + cell[matching[i][1]].x + cell[matching[i][2]].x + cell[matching[i][3]].x + cell[matching[i][4]].x)/5;
        y_t = (cell[matching[i][0]].y + cell[matching[i][1]].y + cell[matching[i][2]].y + cell[matching[i][3]].y + cell[matching[i][4]].y)/5;

        for(int j=0; j<5; j++)
        {
            d_t = abs(abs(x_t-cell[matching[i][j]].x) + abs(y_t-cell[matching[i][j]].y));
            if(d_t > d_max)
            {
                d_max = d_t;
            }
        }

    }
    cout<<"5P_R ======== "<<d_max<<endl;
    return d_max;
}
