

using namespace std;
string filename = "buffer_info";


typedef struct buffer_strategy {
    string driver;
    string receiver;
    int fanout;
    int dist;
}buffer_strategy;

vector<buffer_strategy> buf;
void read_buf_file (vector<buffer_strategy>& vec, string filename)
{
    ifstream infile;
    infile.open(filename);
    buffer_strategy p;
    while(infile >> p.driver >> p.receiver >> p.fanout >> p.dist)
    {
        vec.push_back(p);
    }
}

void report_buf(vector<buffer_strategy>& vec)
{
    for(int i = 0; i < vec.size(); i++)
    {
        cout << vec[i].driver << " " << vec[i].receiver << " " << vec[i].fanout << " " << vec[i].dist << endl;
    }
}

buffer_strategy select_driver_fanout1()
{
    int d = 0;
    int buf_id = 0;
    for(int i=0;i<buf.size();i++)
    {
        if(buf[i].fanout == 1)
        {
            if(buf[i].dist > d)
            {
                d = buf[i].dist;
                buf_id = i;
            }
        }
    }
    return buf[buf_id];
}

buffer_strategy select_driver_fanout2()
{
    int d = 0;
    int buf_id = 0;
    for(int i=0;i<buf.size();i++)
    {
        if(buf[i].fanout == 2)
        {
            if(buf[i].dist > d)
            {
                d = buf[i].dist;
                buf_id = i;
            }
        }
    }
    return buf[buf_id];
}


buffer_strategy select_driver_fanout3()
{
    int d = 0;
    int buf_id = 0;
    for(int i=0;i<buf.size();i++)
    {
        if(buf[i].fanout == 3)
        {
            if(buf[i].dist > d)
            {
                d = buf[i].dist;
                buf_id = i;
            }
        }
    }
    return buf[buf_id];
}

buffer_strategy select_driver_fanout5()
{
    int d = 0;
    int buf_id = 0;
    for(int i=0;i<buf.size();i++)
    {
        if(buf[i].fanout == 5)
        {
            if(buf[i].dist > d)
            {
                d = buf[i].dist;
                buf_id = i;
            }
        }
    }
    return buf[buf_id];
}

//buffer_location(Point pare,r);


//int main()
//{
//
//
//
//    return 0;
//}








//vector< vector<string,string,int,int> > table = {
//                                            {"CKND12BWP12TM1P","CKND12BWP12TM1P",2,265},
//                                            {"CKND12BWP12TM1P","CKND8BWP12TM1P",1,300},
//                                            {"CKND8BWP12TM1P","CKND12BWP12TM1P",1,168},
//                                            {"CKND12BWP12TM1P","CKND8BWP12TM1P",2,300},
//                                            {"CKND8BWP12TM1P","CKND12BWP12TM1P",2,150},
//                                            {"CKND8BWP12TM1P","CKND8BWP12TM1P",1,187},
//                                            {"CKND8BWP12TM1P","CKND8BWP12TM1P",2,168},
//                                            {"CKND12BWP12TM1P","CKND12BWP12TM1P",1,281} };

//buffer1  buffer2  fanout  dist









//if(fanout)
//index i = max_element(dist);
//buf1 buf2
