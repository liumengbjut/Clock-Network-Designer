

using namespace std;


class Greedy_3P
{

vector<vector<int> > cluster_index;
vector<Point> temp_cell;
//vector<vector<int> > graph(100,vector<int>(100));
//vector<vector<int> > grah;
public:
    void init(vector <Point>& t)
    {
        temp_cell = t;
    }

    int left_cell (vector<Point>& temp_cell)
    {
        double min_x = 10000000;
        int min_id = 0;
        for(int i=0;i<temp_cell.size();i++)
        {
            if(temp_cell[i].index == -1)
                continue;
            else if(min_x > temp_cell[i].x)
            {
                min_x = temp_cell[i].x;
                min_id = i;
            }
        }
//       cout<<"min_Id = "<<min_id<<"   min_x = "<<min_x<<endl;
        return min_id;
    }

    int bottom_cell(vector<Point>& temp_cell)
    {
        double min_y = 10000000;
        int min_id = 0;
        for(int i=0;i<temp_cell.size();i++)
        {
            if(temp_cell[i].index == -1)
                continue;
            else if(min_y > temp_cell[i].y)
            {
                min_y = temp_cell[i].y;
                min_id = i;
            }
        }
        return min_id;
    }


    int right_cell (vector<Point>& temp_cell)
    {
        double max_x = -1;
        int max_id = 0;
        for(int i=0;i<temp_cell.size();i++)
        {
            if(temp_cell[i].index == -1)
                continue;
            else if(max_x < temp_cell[i].x)
            {
                max_x = temp_cell[i].x;
                max_id = i;
            }
        }
//        cout<<"max_Id = "<<max_id<<"   max_x = "<<max_x<<endl;
        return max_id;
    }

    int top_cell (vector<Point>& temp_cell)
    {
        double max_y = -1;
        int max_id = 0;
        for(int i=0;i<temp_cell.size();i++)
        {
            if(temp_cell[i].index == -1)
                continue;
            else if(max_y < temp_cell[i].y)
            {
                max_y = temp_cell[i].y;
                max_id = i;
            }
        }
        return max_id;
    }

    int boundary_cell(vector<Point>& temp_cell)
    {
        double average_loc_x,average_loc_y;
        double sum_x = 0,sum_y=0;
        int dis_id = 0;
        int t = 0;
        for(int i=0;i<temp_cell.size();i++)
        {
            if(temp_cell[i].index != -1)
            {
            sum_x = temp_cell[i].x+sum_x;
            sum_y = temp_cell[i].y+sum_y;
            t = t+1;
            }
        }
        average_loc_x = sum_x/t;
        average_loc_y = sum_y/t;
        double max_distance = 0;
        for(int i=0;i<temp_cell.size();i++)
        {
            if(temp_cell[i].index != -1)
            {
            double dist_temp = sqrt(pow((average_loc_x-temp_cell[i].x),2)+pow((average_loc_y-temp_cell[i].y),2));
            if(max_distance < dist_temp )
            {
                max_distance = dist_temp;
                dis_id = i;
            }
            }
        }
        return dis_id;
    }

 /*   int boundary_cell_avr(vector<Point>& temp_cell)
    {
        double average_loc_x,average_loc_y;
        double sum_x = 0,sum_y=0;
        int dis_id = 0;
        int t = 0;
        for(int i=0;i<temp_cell.size();i++)
        {

            sum_x = temp_cell[i].x+sum_x;
            sum_y = temp_cell[i].y+sum_y;

        }
        average_loc_x = sum_x/temp_cell.size();
        average_loc_y = sum_y/temp_cell.size();
        double max_distance = 0;
        for(int i=0;i<temp_cell.size();i++)
        {
            if(temp_cell[i].index != -1)
            {
            double dist_temp = sqrt(pow((average_loc_x-temp_cell[i].x),2)+pow((average_loc_y-temp_cell[i].y),2));
            if(max_distance < dist_temp )
            {
                max_distance = dist_temp;
                dis_id = i;
            }
            }
        }
        return dis_id;
    } */



    vector<int> min_cluster (vector<vector<int> > &graph, int given_id, const vector<Point>& cell_list)           //Given id and current cell list, compute the min weight edge
    {

        vector<int> a;
        int min_edge_id,min_edge_id2;
    //first round
        double min_edge = 1000000;
        for(int j=0; j<cell_list.size(); j++)
        {

            if( (graph[given_id][j] < min_edge) && (graph[given_id][j]!=0) && (graph[given_id][j]!=-1) )
            {
                min_edge = graph[given_id][j];
                min_edge_id = j;
            }
        }
        //first point invalid
        for(int n=0; n<cell_list.size(); n++)
        {
            graph[given_id][n] = -1;
            graph[n][given_id] = -1;
        }
    //second round
        min_edge = 1000000;
        for(int j=0; j<cell_list.size(); j++)
        {
            if( (graph[min_edge_id][j] < min_edge) && (graph[min_edge_id][j]!=0) && (graph[min_edge_id][j]!=-1))
            {
                min_edge = graph[min_edge_id][j];
                min_edge_id2 = j;
            }
        }
        //second point invalid
        for(int n=0; n<cell_list.size(); n++)
        {
            graph[min_edge_id][n] = -1;
            graph[n][min_edge_id] = -1;
        }
        //third point invalid
        for(int n=0; n<cell_list.size(); n++)
        {
            graph[min_edge_id2][n] = -1;
            graph[n][min_edge_id2] = -1;
        }
        //print test
//        cout << given_id<<"  "<<min_edge_id<<"  "<< min_edge_id2<<"\n"<<endl;
        a.push_back(given_id);
        a.push_back(min_edge_id);
        a.push_back(min_edge_id2);
//        cout<<"finish clustering!!!!!!!!!!!!!!!"<<endl;
        // write file of index cluster
//        ofstream cluster_index_file;
//        cluster_index_file.open ("/mnt/hgfs/work/ed/textfile/cluster_index4", std::ios_base::app);
//        cluster_index_file << given_id<<"  "<<min_edge_id<<"  "<< min_edge_id2<<"\n";
//        cluster_index_file.close();
        //set used to -1
        temp_cell[given_id].index = -1;
        temp_cell[min_edge_id].index=-1;
        temp_cell[min_edge_id2].index=-1;

        return a;
    }



    vector<vector<int>> strategy (vector<vector<int> > graph, const vector<Point>& whole_cell )  //greedy strategy
    {
        vector<vector<int>> mat3;
        vector<int> a;
        int id_t1,id_t2,id_t3,id_t4;
//        cout<<"N="<<whole_cell.size()/3<<endl;

        for(int i=0; i < whole_cell.size()/3; i++)     //based on cluster number
        {
            if (i%4 == 0)
            {
                id_t1 = left_cell(temp_cell);           //choose current left cell
//                id_t1 = boundary_cell(temp_cell);
//                cout<<"cluster id= "<<i<<endl;
                a = min_cluster(graph, id_t1, whole_cell);         //make cluster and delete used cell of temp
                mat3.push_back(a);
            }
            if (i%4 == 1)
            {
                id_t2 = bottom_cell(temp_cell);
//                id_t2 = boundary_cell(temp_cell);
//                cout<<"cluster id = "<<i<<endl;
                a= min_cluster(graph, id_t2, whole_cell);
                mat3.push_back(a);
            }
            if (i%4 == 2)
            {
                id_t3 = right_cell(temp_cell);
//                id_t2 = boundary_cell(temp_cell);
//                cout<<"cluster id = "<<i<<endl;
                a= min_cluster(graph, id_t3, whole_cell);
                mat3.push_back(a);
            }
            if (i%4 == 3)
            {
                id_t4 = top_cell(temp_cell);
//                id_t2 = boundary_cell(temp_cell);
//                cout<<"cluster id = "<<i<<endl;
                a= min_cluster(graph, id_t4, whole_cell);
                mat3.push_back(a);
            }
        }

        return mat3;
    }
};



//int main()
//{
//	vector<Point> cache_cell;
//    Get_def_data();
//    string cell_data_file = "/mnt/hgfs/work/ed/textfile/data63";
//    cache_cell = Get_cell_data(cell_data_file);
//
//    Greedy_3P Greedy;
//    Get_weight3_data(cache_cell);
//    Greedy.strategy(cache_cell);
//
//    cout << "testing ............ "<< cache_cell[0].x<<endl;
//}

