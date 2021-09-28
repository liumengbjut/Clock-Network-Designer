# Clock-Network-Designer
提出了一种可自动综合且可多扇出配置的均衡型时钟树的设计优化方法。

该研究内容旨在解决传统的均衡树需要手工全定制设计的现状。该设计解决了多扇出均衡型时钟树的拓扑规划问题，可充分利用时钟缓冲器的多驱动能力配置扇出选择；针对多扇出时钟树的设计要求，基于贪心算法和平面划分策略解决了节点簇的匹配聚类问题，因此可以灵活解决不同时钟树层次的节点簇的不同数目要求的情况，并针对多节点簇的父节点生成问题建立了多曼哈顿区域合成方法，从而建立了自底向上的均衡树拓扑节点组成结构；基于动态规划思想设计了低功耗缓冲器插入策略，该缓冲器插入策略充分考虑了版图中标准单元布局的诸多情况，并考虑了版图复杂障碍单元存在条件下的缓冲器的布线规划问题，保证了布线后的均衡树设计仍然具有均衡性。该均衡型时钟树既可以作为顶层驱动级时钟树设计方案，同时也可以作为时序要求苛刻的局部电路的时钟网络解决方案。
该工具方法学可以解决工艺波动条件下树形时钟结构时序结果不可控且不易优化的难点，使其具备独立驱动多级子时钟树的能力，同时也可以作为子时钟树的方案。


工具核心程序基于C++代码开发，并提供了面向ISPD和ISCAS基准程序的解析接口。

1.	坐标点采用C++标准库中的pair类型，该类型包含在utility头文件中：
#include <utility>
typedef pair<double, double> coor;

2.	矩形以其左下角和右上角的坐标表示：
#include <utility>
typedef pair<coor, coor> rect;

3．Rectinear形状切割为最大的Rectangular形状，否则会出现长宽判断错误
 
3.	属于某个子树的时序节点或者Multi-Source Tree的TAP节点或者Clock Mesh的Mesh Buffer节点采用C++标准库中的vector类型表示，作为函数输入：
#include <vector>
#include <string>
vector<string> tree_load_nodes;
vector<string> tree_references;

4.	并非所有Fixed的单元都作为Obstacle的检测标准，一个例外是Flip Chip的Bump Cell，因为它所占用的层与下方的标准单元没有交集。

5.	Cell Row的处理：
a.	Cell Row在DEF中的格式：
      ROW rowName siteName origX origY siteOrient DO numX BY numY STEP stepX stepY
b.	处理流程：
Step 1：按上述Row格式读取一行DEF中的Row定义，可采用C++中的
正则表达式实现：#include <regex>
Step 2：在LEF中查找DEF Row定义中的siteName的信息，如果找不
到，则报Error，提示用户修改LEF中的SITE名称与TF相同
       Step 3：是否存在一个Stack数据结构其栈顶元素具有与当前解析的Row
              具有相同siteName、STEP信息、DO信息，如果水平Row，具
有相同origX信息，如果竖直Row，具有相同origY信息
       Step 4：如果Step 3找到一个Stack，且其中数量元素大于1，则弹栈栈顶元素，压栈当前Row的信息；如果没有找到一个Stack，则创建一个新的Stack，将当前Row的信息压栈。一个栈最多两个元素。堆栈的数据结构采用vector实现
       Step 5：等到DEF中所有Row都读入后，将各个Stack中记录的坐标分别合并成一个矩形，只有那些与tree_reference中cell具有相同SITE的矩形可以摆放单元

6.	TF解析的数据结构：
struct CmdMetalLayer {
   CmdMetalLayer();
   oaBoolean operator != (const CmdMetalLayer& other_metal) {
     return layer_number != other_metal.layer_number; 
   }
   string       layer_name;
   unsigned int  layer_number;
   string       mask_name;
   double      pitch;
   double           default_width;
   double           min_width;
   double           min_spacing;
   double           max_width;
   double           min_area;
   double           fat_tbl_dimension;
   vector<double>    fat_tbl_threshold;
   vector<double>    fat_tbl_parallel_length;
   vector<vector<double>> fat_tbl_spacing; // Only Record the metal layer with fatTblSpacing definition
};

struct CmdTFSite {
  string tile_name;
  double site_width;
  double site_height;
};

class CmdTechnology {
 public:
   CmdTechnology(const string& tech_file);
   CmdTechnology(const CmdTechnology&);
   ~CmdTechnology() {}
   unsigned int get_max_metal_layer() const;
   const string& get_mask_name(const string& layer_name) const;
   const CmdMetalLayer& get_metal_layer(const unsigned int layer_number) const;
   double get_metal_layer_max_width(const unsigned int layer_number) const;
   double get_metal_layer_min_width(const unsigned int layer_number) const;
   double get_metal_layer_min_spacing(const unsigned int layer_number) const;
   double get_metal_layer_min_area(const unsigned int layer_number) const;
   double get_metal_layer_default_width(const unsigned int layer_number) const;
   unsigned int get_num_of_recorded_layers() const;
   const vector<CmdSite>& get_unit_tiles() const;
   const CmdSite& get_unit_tile(const string& tile_name) const;
   bool is_finfet_tech() const;
   double get_diffusion_spacing() const;
   double get_implant_spacing() const;
 private:
   // Technology Name
   string tech_name_ = "";
   // Technology Unit Definition
   string unit_time_name_    = "ns";
   string unit_length_name_  = "micron";
   string unit_voltage_name_ = "v";
   string unit_current_name_ = "mA";
   string unit_power_name_   = "nw";
   string unit_resistance_name_  = "kohm";
   string unit_capacitance_name_ = "pf"; 
   string unit_inductance_name_  = "";
   unsigned long time_precision_    = 1000;
   unsigned long length_precision_  = 1000;
   unsigned long voltage_precision_ = 100000;
   unsigned long current_precision_ = 100;
   unsigned long power_precision_   = 1000;
   unsigned long resistance_precision_  = 10000000;
   unsigned long capacitance_precision_ = 10000000;
   unsigned long inductance_precision_ = 1000;
   unsigned int grid_resolution_ = 5;
   unsigned int max_metal_layer_;  // 最高金属层
   double     finfet_xpitch_ = 0;
   double     finfet_ypitch_ = 0;
   double     finfet_xoffset_ = 0;
   double     finfet_yoffset_ = 0;
   double     diffusion_spacing_;
   double    implant_spacing_;  // Only record the maximum of each implant layers
   vector<CmdMetalLayer> metal_layers_;
   vector<CmdTFSite> tile_;
};

7.	Lib数据结构
struct CmdOperatingCondition {
  string oc_name;
  double process;
  double voltage;
  int    temperature;
};

class CmdLibPin {
public:
CmdLibPin(const string& pin_name, const string& direction, const string& pin_type, const double rise_capacitance, const double fall_capacitance, const double max_capacitance, const string& function);
CmdLibPin(const CmdLibPin&);
~CmdLibPin();
const string& get_pin_direction() const;
const string& get_pin_type() const;
double get_pin_rise_cap() const;
double get_pin_fall_cap() const;
double get_pin_max_cap() const;
const string& get_comb_function() const;
private:
string pin_name_;
string direction_;
string pin_type_;  // clock_pin / comb_pin / clock_gate_clock_pin / clock_gate_out_pin / clock_gate_enable_pin / clock_gate_test_pin / data_pin / async_set_reset / sync_set_reset / scan_in_pin / scan_out_pin / data_out_pin / comb_out_pin
double rise_capacitance_ = 0; // When direction is input
double fall_capacitance_ = 0; // When direction is input
double max_capacitance_ = 0; // When direction is output
string function_ = “”;  // Only when direction is comb_out_pin
};

class CmdLibCell {
  public:
CmdLibCell(const string& lib_cell_name, const string& lib_cell_type, const vector<CmdLibPin>& lib_cell_pins);
CmdLibCell(const CmdLibCell&);
~CmdLibCell();
bool is_inv() const;
bool is_buf() const;
bool is_icg() const;
bool is_memory() const;
bool is_ff() const;
bool is_latch() const;
bool is_multibit() const;
bool is_black_box() const;
bool is_combinational() const;
bool is_sequential() const;
const vector<CmdLibPin>& get_pins(const string& pin_name) const;
const vector<CmdLibPin>& get_all_input_pins() const;
const vector<CmdLibPin>& get_all_output_pins() const;
const vector<CmdLibPin>& get_clock_pins() const; // Dual-Port Memory may have two clk pins 
const vector<CmdLibPin>& get_async_set_reset_pins() const;
const vector<CmdLibPin>& get_sync_set_reset_pins() const;
const vector<CmdLibPin>& get_clock_gate_enable_pins() const;
const vector<CmdLibPin>& get_clock_gate_test_pins() const;
const vector<CmdLibPin>& get_data_pins() const;
const vector<CmdLibPin>& get_scan_in_pins() const;
const vector<CmdLibPin>& get_scan_out_pins() const;
const vector<CmdLibPin>& get_clock_gate_out_pins() const;
const vector<CmdLibPin>& get_data_out_pins() const; // Multibit may have multiple data out pins
  private:
    string cell_name_;
    string cell_type_; // icg / comb_cell / flip_flop / latch / memory / black_box / multibit
    vector<CmdLibPin> cell_pins_;
};

class CmdLibrary {
  public:
CmdLibrary(const string& lib_file);
CmdLibrary(const CmdLibrary&);
~CmdLibrary();
double get_slew_derate_from_library() const;
double get_slew_lower_threshold_pct_rise() const;
double get_slew_upper_threshold_pct_rise() const;
double get_slew_lower_threshold_pct_fall() const;
double get_slew_upper_threshold_pct_fall() const;
double get_input_threshold_pct_rise() const;
double get_output_threshold_pct_rise() const;
double get_input_threshold_pct_fall() const;
double get_output_threshold_pct_fall() const;
double get_default_max_transition() const;
const CmdOperatingCondition& get_operating_condition() const;
const CmdLibCell& get_lib_cells(const string& lib_cell_name) const;
  private:
string lib_name_;
double slew_derate_from_library_;
double slew_lower_threshold_pct_rise_;
double slew_upper_threshold_pct_rise_;
double slew_lower_threshold_pct_fall_;
double slew_upper_threshold_pct_fall_;
double input_threshold_pct_fall_;
double output_threshold_pct_fall_;
double input_threshold_pct_rise_;
double output_threshold_pct_rise_;
double default_max_transition_;
CmdOperatingCondition oc_;
vector<CmdLibCell> lib_cells_;
};

8．LEF数据结构
  struct CmdLefSite {
string site_name;
string class_name; 
string site_symmetry;  // X / Y / R90
double site_width;
double site_height;
  };

  class CmdLefPin {
    public:
      CmdLefPin(const string& pin_name, const string& pin_direction, const string& pin_usage, const vector<pair<string, vector<rect>>>& pin_shapes);
      CmdLefPin(const CmdLefPin&);
      ~CmdLefPin();
      const string& get_pin_name() const;
      const string& get_pin_direction() const;
      const string& get_pin_usage() const;
      const coor& get_pin_centroid(const string& metal_layer) const;  // 获得某个金属层PIN的抽象坐标点，以其重心来表示，在实际芯片版图中可以根据该CELL的orientation、该值和CELL的坐标求得抽象PIN在整个芯片的坐标
      const vector<string>& get_pin_layers() const; // 最高层金属PIN在0号位
private:
  string pin_name_;
  string pin_direction_;  // INPUT / OUTPUT / INOUT
  string pin_usage_(“SIGNAL”);  // POWER / GROUND / SIGNAL
  vector<pair<string, vector<rect>>> pin_shapes_; // metal layer name and corresponding metal rectangles。最高层金属PIN在0号位
 
  };

  class CmdLefCell {
    public:
      CmdLefCell(const string& cell_name, const string& class_name, const double cell_width, const double cell_height, const coor& foreign, const coor& origin, const vector<string>& cell_symmetry, const CmdLefSite& cell_site, const vector<CmdLefPin>& cell_pins);
      CmdLefCell(const CmdLefCell&);
      ~CmdLefCell();
      const CmdLefSite& get_cell_site() const;
      const string& get_cell_class() const;
      const vector<string>& get_allowed_symmetry() const;
      const pair<double, double>& get_cell_dimension() const;
      const vector<CmdLefPin>& get_power_pins() const; // Cells may have more than one PG pins
      const vector<CmdLefPin>& get_ground_pins() const;
      const CmdLefPin& get_signal_pins(const string& pin_name) const;
private:
  string cell_name_;
  string class_name_; // CORE / BLOCK / PAD / COVER BUMP / ENDCAP BOTTOMLEFT
  double cell_width_;
  double cell_height_;
  coor foreign_;
  coor origin_;
  vector<string> cell_symmetry_
CmdLefSite cell_site_;
  vector<CmdLefPin> cell_pins_;
  };

  class CmdLefLibrary {
    public:
      CmdLefLibrary(const string& lef_file);
      CmdLefLibrary(const CmdLefLibrary&);
      ~CmdLefLibrary();
      double get_lef_version() const;
      double get_lef_manufacturing_grid() const;
      const string& get_lef_divider_char() const;
      const string& get_lef_busbit_chars() const;
      const CmdLefCell& get_lef_cells(const string& lef_cell_name) const;
      const vector<CmdLefSite>& get_lef_sites() const;
      const CmdLefSite& get_lef_site(const string& lef_site_name) const;
      const CmdLefSite& get_cell_site(const string& lef_cell_name) const;
private:
  double lef_verison_;
  string lef_busbit_chars_ = “[]”;
  string lef_divider_char_ = “/”;
  double lef_manufacturing_grid_ = 0.005;
  vector<CmdLefCell> lef_cells_;
  vector<CmdLefSite> lef_sites_;
  };

9．DEF解析程序
struct CmdRow {
    string row_name;
string site_name;
long orig_x;
long orig_y;
string site_orientation;
unsigned long site_num_x;
unsigned long site_num_y;
unsigned int site_step_x;
unsigned int site_step_y;
};

struct CmdTrack {
  string metal_layer;
  string direction; // Y means horizontal; X means vertical
  long start_coor; // Location of the first track
  long num_of_tracks;
  unsigned int track_spacing;
};

struct CmdDefCell {
  string cell_name;
  string ref_name;
  string cell_orientation;
  coor orig_point;
  string cell_status;  // FIXED / PLACED
};

struct CmdPrerouteWire {
  string metal_layer;
  unsigned int width;
  string metal_shape; // RING / STRIPE / FOLLOWPIN / IOWIRE / COREWIRE / BLOCKWIRE / BLOCKAGEWIRE / FILLWIRE / FILLWIREOPC / DRCFILL / PADRING
  coor start_coor;
  coor end_coor;
};

struct CmdPlacementBlockage {
  string blockage_type; // hard/soft/partial. CTS ignores soft/partial blockage
vector<rect> blockages;
};

struct CmdRouteBlockage {
  string layer_name;
  vector<rect> blockages;
  unsigned int spacing; // minimum spacing allowed between the blockage and any other routing shape
};

struct CmdRegion {
  string region_name;
  string region_type;  // FENCE / GUIDE
  vector<rect> region_area;
  vector<CmdDefCell> region_cells;
};

class CmdDefLibrary {
  public:
    CmdDefLibrary(const string& def_file);
CmdDefLibrary(const CmdDefLibrary&);
~CmdDefLibrary();
double get_def_verision() const;
const string& get_divider_char() const;
const string& get_busbit_chars() const;
const string& get_design_name() const;
const string& get_technology_name() const;
unsigned long get_length_precision() const;
const vector<rect>& get_die_area() const;
const vector<string>& get_region_names() const;
const CmdRegion& get_region(const string& region_name) const; // bound / plangroup / voltage area are considered as regions in DEF
vector<rect> get_allowed_cts_area(const string& tile_name, const string& row_direction) const; // tile_name is the tile of clock tree reference, excluding hard macro, hard placement blockage, route guide, route blockage from the available row area. We also need to consider the blockage effect of power straps and power rings
const vector<CmdDefCell>& get_macro_cells() const;
const CmdDefCell& get_cells(const string& cell_inst_name) const;
const vector<CmdDefCell>& get_io_cells() const;
const CmdDefCell& get_io_cell(const string& io_cell_name) const;
const vector<CmdDefCell>& get_endcap_cells() const;
const vector<CmdDefCell>& get_tap_cells() const;
const vector<CmdPlacementBlockage>& get_placement_blockages() const;
const CmdPlacementBlockage& get_placement_blockage(const string& placement_blockage_name) const;
const vector<CmdRouteBlockage>& get_route_blockages() const;
const CmdRouteBlockage>& get_route_blockage(const string& route_blockage_name) const;
const vector<CmdPrerouteWire>& get_preroute_wires() const;
const vector<CmdPrerouteWire>& get_preroute_wire_by_name(const string& preroute_wire_name) const;
const vector<CmdPrerouteWire>& get_preroute_wire_by_layer(const string& metal_layer) const;
private:
double def_version_;
string divider_char_ = “/”;
string busbit_chars_ = “[]”;
string design_name_;
string technology_name_;
unsigned long length_precision_;
vector<rect> die_area_;  // Rectilinear shape can be divided into several rectangular shapes. 
vector<CmdRow> rows_;
vector<CmdTrack> tracks_;
vector<CmdRegion> regions_;
vector<CmdDefCell> cells_;
vector<CmdPlacementBlockage> placement_blockages_;
vector<CmdRouteBlockage> route_blockages_;
vector<pair<string, vector<CmdPrerouteWire>>> preroute_wires_; // string is net name, vector is net shape
};

 9.	Tcl Package的使用及二进制封装流程
A．程序示例：
pkg_func.tcl:

package provide cmd_pkg 1.0  ;# Package名称及版本号

namespace eval ::cmd_pkg {
  namespace export *        ;# 提供Package中可被调用的函数名称
}

proc ::cmd_pkg::print {str} {  ;# 注意此处层次化名空间的使用
  set fid [open ./test_result.txt w]
puts $fid $str
close $fid
}

main.tcl:

lappend auto_path ./tcl_pkg       ;# 指定Package所在的路径
package require cmd_pkg  ;# 指定所需Package的名称

cmd_pkg::print "clock mesh designer"  ;# 调用Package内部的函数


   B. 运行tclsh8.6
     % pkg_mkIndex ./tcl_pkg *.tcl  ;# 产生pkgIndex.tcl文件，告诉程序如何使用该Package


   C．在https://sourceforge.net上下载tclexecomp，该工具是freewrap的增强版
(https://sourceforge.net/projects/tclexecomp/?source=directory)，
(http://freewrap.sourceforge.net/)
利用tclexecomp来进行Package的二进制封装

Eg: /home/sunwq/TEST/tclexecomp-V1.0.4/tclexecomp64 test.tcl ./tcl_pkg/*.tcl
-w /home/sunwq/TEST/tclexecomp-V1.0.4/tclexecomp64 –o mytest -forcewrap



10.	对设计版图解析程序cmd_get_design_info_func.tcl的补充解释：
A．	cmd_get_available_row_areas 根据用户给定的某个CLKBUF/CLKINV名字求得该类型单元instance可以摆放的cell row区域，算法所确定的最终驱动单元的位置必须落在该函数计算出来的允许的cell row区域内部；
B．	cmd_get_voltage_areas算法所确定的最终驱动单元的位置必须落在用户给定的Voltage Area内部，而不能落到其它的Voltage Area之中。其它的Voltage Area可以视为Placement Blockage在规避算法中加以考虑；
C．	cmd_get_preroute_net_shapes算法所确定的驱动单元的位置最好不要放在preroute net shape的下方，因为这样preroute net shape会阻挡从驱动单元到布线层的走线，为了规避阻挡而产生的绕线会增加时钟走线的skew；
D．	cmd_get_track_info时钟走线必须走在track上，即时钟线的中间线与相应track重叠；
E．	cmd_get_clock_source获取时钟的源头，从时钟源头将时钟引到其所控制寄存器的重心位置再生成tree；
F．	cmd_get_preplace_stdcells出于简化DRC考虑，要求时钟驱动单元距离fixed的preplace单元距离不小于2um，同时必须在unit tile的整数倍上面unit tile的宽度可以通过get_attr [get_site_rows xx] site_space，xx为cmd_get_available_row_areas中的任意一个row，时钟驱动单元与fixed单元的间距为site_space数值的整数倍
