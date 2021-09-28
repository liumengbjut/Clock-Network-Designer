//for (int i=0;i<level_num;i++)
//{
////    report_tree(TreeNode *root)
////    {
//
//
//
//        int dist = select_driver_fanout1(root.buf_name);
//        int buf_num = R[i]/dist;
//        cout<< "connect" <<endl;
//
//
//
//
////    }
//
//}

typedef struct buffer_cell {
    int index;
    double x;
    double y;
    string buf_name;
}buffer_cell;
typedef struct connect {
    string driver_pin;
    string receiver_pin;
    string net_name;
    double net_length;
}connect;

vector<buffer_cell> buffer(1000);
vector< pair<string,string> > connection(1000);
int counter = 0;

void report_buf_connection(Point parent,Point child,double r,int buf_num)
{
    string s1 = "cell";
    string s2 = "buffer";
    for(int n=0;n<buf_num;n++)
    {
        if(n == 0)
        {
//            buffer_location(Point parent,r);
            double d = r/buf_num+1;
            buffer[n+counter].x = 1;
            buffer[n+counter].y = 1;
        }
        if(n > 0)
        {
            buffer[n+counter].x = 2;
            buffer[n+counter].y = 2;
        }
        cout<< "create_buf"<< n+counter <<endl;
    }
    for(int i=0;i<=buf_num;i++)
    {
        pair<string,string> connection_tmp;
        buffer[i+counter].index = i+counter;
//        buffer[i+counter].x = 1;
//        buffer[i+counter].y = 1;
        if(i == 0)
        {
            connection_tmp.first =  s1 + to_string(parent.index) ;
            connection_tmp.second = s2 + to_string(buffer[i+counter].index);
        }
        if(i>0 && i<buf_num)
        {
            connection_tmp.first = s2 + to_string(buffer[i+counter-1].index);
            connection_tmp.second = s2 + to_string(buffer[i+counter].index);
        }
        if(i == buf_num)
        {
            connection_tmp.first = s2 + to_string(buffer[i+counter-1].index);
            connection_tmp.second = s1 + to_string(child.index);
        }
        connection.push_back(connection_tmp);

        cout<< "connect from " << connection_tmp.first << " to " << connection_tmp.second <<endl;

    }
    counter = buf_num + counter;
//    cout<< "connect_cell from cell" << parent.index << " to cell" <<child.index<<endl;
}

