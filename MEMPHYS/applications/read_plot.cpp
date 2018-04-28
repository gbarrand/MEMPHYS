
// A MEMPHYS.root file is read with the inlib/rroot API and plotting is done with inlib/exlib.

#include "read.icc"

#include "plotter_style.icc"

#include <inlib/histo/h1d>
#include <inlib/histo/h2d>

//////////////////////////////////////////////////////////////////////////////
/// plotting done with inlib/exlib : //////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include <exlib/Windows/plotter>
#define EXLIB_SCREEN_MGR Windows
#else
#include <exlib/X11/plotter>
#define EXLIB_SCREEN_MGR X11
#endif

inline bool plot(std::ostream& a_out,
		 inlib::histo::h1d& aHisto1D,inlib::histo::h2d& aHisto2D,
		 bool a_ttf) {
  
  exlib::EXLIB_SCREEN_MGR::session smgr(a_out);
  if(!smgr.is_valid()) return false;

  exlib::EXLIB_SCREEN_MGR::plotter plotter(smgr,1,2,0,0,800,600);
  if(!plotter.window()) return false;
  inlib::env_append_path("EXLIB_FONT_PATH",".");    
  plotter.plots().view_border = false;

  if(plotter.plots().set_current_plotter(0)) {
    inlib::sg::plotter& sgp = plotter.plots().current_plotter();
    set_region_style(sgp,a_ttf);
    sgp.x_axis().title = "time";
    sgp.y_axis().title = "Entries";
    inlib::sg::plottable* ptb = new inlib::sg::h1d2plot(aHisto1D);
    ptb->set_name(aHisto1D.title());
    sgp.add_plottable(ptb);
  }

  if(plotter.plots().set_current_plotter(1)) {
    inlib::sg::plotter& sgp = plotter.plots().current_plotter();
    set_region_style(sgp,a_ttf);
    sgp.x_axis().title = "time";
    sgp.y_axis().title = "PE";
    inlib::sg::plottable* ptb = new inlib::sg::h2d2plot(aHisto2D);
    ptb->set_name(aHisto2D.title());
    sgp.add_plottable(ptb);
  }

  plotter.show();

  smgr.steer();

  return true;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include <inlib/args>
#include <iostream>

int main(int a_argc,char** a_argv) {
  
#ifdef INLIB_MEM
  inlib::mem::set_check_by_class(true);{
#endif //INLIB_MEM

  inlib::args args(a_argc,a_argv);
  std::string file;
  if(!args.file(file)) file = "MEMPHYS.root";

  bool ttf = args.is_arg("-ttf");
  bool verbose = args.is_arg("-verbose");
  
  ////////////////////////////////////////////////////////
  // Create histograms : /////////////////////////////////
  ////////////////////////////////////////////////////////
  inlib::histo::h1d hits_times("Hits times",100,0,3000);
  inlib::histo::h2d digits_time_pe("Digits PE time",100,0,3000,100,0,10);

  ////////////////////////////////////////////////////////
  /// Read data : ////////////////////////////////////////
  ////////////////////////////////////////////////////////
  if(!read_data(std::cout,file,hits_times,digits_time_pe,verbose)) {
    std::cout << "can't read data file." << std::endl;
    return EXIT_FAILURE;
  }

  ////////////////////////////////////////////////////////
  /// plot histos : //////////////////////////////////////
  ////////////////////////////////////////////////////////
  plot(std::cout,hits_times,digits_time_pe,ttf);

#ifdef INLIB_MEM
  }inlib::mem::balance(std::cout);
  std::cout << "MEMPHYS_read_plot : exit(mem) ..." << std::endl;
#else  
  if(verbose) std::cout << "MEMPHYS_read_plot : exit ..." << std::endl;
#endif //INLIB_MEM
  
  return EXIT_SUCCESS;
}
