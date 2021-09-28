//volatge area
vector <double> vol_area {0,0,200000,200000};

//obstacle area
vector<vector<double> > obs {
                               {27763,46498,27770,46500},
                                {277,464,278,465},
                              {1000,1200,100,200},
                              {50,50,55,55} };

struct CmdRow {
//    string row_name;
//    string site_name;
//    long orig_x;
      double orig_y=0;
//    string site_orientation;
//    unsigned long site_num_x;
//    unsigned long site_num_y;
//    unsigned int site_step_x;
      double site_step_y=1.68;
};

//CmdRow.orig_y = 0.00;
//CmdRow.site_step_y = 1.68;


