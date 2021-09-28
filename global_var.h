using namespace std;
extern int sink_num;
extern int level_num;
extern vector<double> R;

typedef struct Point
{
	int index;
	double x;
	double y;
	string buf_name;
	Point(int i, double a, double b) :index(i), x(a), y(b) {}
	Point() : x(0), y(0) {}
}Point;
//struct Point
//{
//    int index;
//    double x;
//    double y;
//    Point(double a, double b): x(a), y(b) {}
//    Point():x(0),y(0) {}
//};


