//int N=6;
//vector<vector<double> > obs(N,vector<int>(4));

//for(int i=0;i<N;i++)
//    {
//        for(int j=0;j<4;j++)
//            {
//                obs[i][j] =
//            }
//    }

bool overlap_check(vector<vector<double> > &obs,pair<double,double> merge_point)
{
    int flag = 1;
    for(int i=0;i<obs.size();i++)
    {
        if((obs[i][0]<=merge_point.first) && (merge_point.first<=obs[i][2]) && (obs[i][1]<=merge_point.second) && (merge_point.second<=obs[i][3]))
        flag=0;
//        cout<<"obs   "<<obs[i][0]<<"   "<<obs[i][3]<<endl;
//          cout<<"flag ===== "<<flag<<endl;
    }
    return flag;
}
