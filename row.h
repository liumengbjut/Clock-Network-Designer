
void row_convert(CmdRow& row,pair<double,double> merge_point)
{
    double N;
    N = (merge_point.second-row.orig_y) / row.site_step_y;
    merge_point.second = row.orig_y + round(N)*row.site_step_y;
}
