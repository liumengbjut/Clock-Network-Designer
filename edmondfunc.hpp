
using namespace std;
//vector<vector<int>> edmondfunc(const string& mnweightfile,const string& result);

vector<vector<int>> edmondfunc(const string& mnweightfile,const string& result)
{
    ifstream infile(mnweightfile);// vertex
    ofstream outfile(result);
    vector<vector<int>> mat;
    int n, m;
    cout << "Starting edmond_algorithm ..." << endl;
//    ifstream infile("/home/lm/work/edmonds_0815/input.txt");
    infile >> n >> m;
    cout << "vertex number ="  << n <<"  edge number ="<< m << endl;
    EdmondsMatching matching(n);
    for (int i = 0; i < m; i++)
    {
        int v1, v2, w;
        infile >> v1 >> v2 >> w;
        matching.addEdge(v1, v2, w);
    }
    bool success = matching.findMinimumWeightMatching();
    int weight = matching.getMatchingWeight();
    auto edges = matching.getMatchedEdges();
    for (auto e: edges)
    {
        vector<int> a;
        cout << e.first << " " << e.second << endl;
        a.push_back(e.first);
        a.push_back(e.second);
        mat.push_back(a);
        outfile << e.first << " " << e.second << endl;
    }
    return mat;
}

