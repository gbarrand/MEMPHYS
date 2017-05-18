
// A MEMPHYS.root file is read with the AIDA API and plotting is done with inlib/exlib.

#include "analysis.icc"

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
inline void set_region_style(inlib::sg::plotter& a_plotter) {
  
  a_plotter.right_axis_visible = true;
  a_plotter.top_axis_visible = true;
  
  // ROOT is in NDC, then we take a plotter with width = height = 1

  // PAW :
  //float XSIZ = 20.0F;
  //float XLAB = 1.4F;
  //float YSIZ = 20.0F;
  //float YLAB = 1.4F;

  float XLAB = 1.4F/20.0F; //0.07 //x distance of y title to data frame.
  float YLAB = 0.8F/20.0F; //0.04 //y distance of x title to data frame.
  YLAB = 0.05F; //FIXME : cooking.

  // ROOT def margins 0.1. SoPlotter def 0.1.
  a_plotter.right_margin = 0.1f;
  a_plotter.top_margin = 0.1f;
  a_plotter.left_margin = 0.1f;
  a_plotter.bottom_margin = 0.1f;

  a_plotter.wall_style().visible = false;
  a_plotter.grid_style().visible = false;
  a_plotter.inner_frame_style().line_width = 1;
  
  std::string font = "times.ttf";
  bool smoothing = true;
  
  // To have a good matching with ROOT for text size :
  //double majic = 0.014/0.027;
  float majic = 0.6f;
  
  // X axis :
  a_plotter.x_axis().labels_style().scale = majic;
  a_plotter.x_axis().labels_style().font = font;
  a_plotter.x_axis().labels_style().smoothing = smoothing;
  a_plotter.x_axis().title_style().scale = majic;
  a_plotter.x_axis().title_style().font = font;
  a_plotter.x_axis().title_style().smoothing = smoothing;
  a_plotter.x_axis().mag_style().scale = majic;
  a_plotter.x_axis().mag_style().font = font;
  a_plotter.x_axis().mag_style().smoothing = smoothing;
  a_plotter.x_axis().line_style().width = 2;
  a_plotter.x_axis().ticks_style().width = 2;
  a_plotter.x_axis().label_height = 0.05f;
  a_plotter.x_axis().title_height = 0.06f;
  a_plotter.x_axis().title_to_axis = 1.1f*YLAB;
//a_plotter.x_axis().label_to_axis = 0.01f;
  // Set hplot tick modeling :
  a_plotter.x_axis().modeling = inlib::sg::tick_modeling_hplot();
  a_plotter.x_axis().divisions = 505;

  // Y axis :
  a_plotter.y_axis().labels_style().scale = majic;
  a_plotter.y_axis().labels_style().font = font;
  a_plotter.y_axis().labels_style().smoothing = smoothing;
  a_plotter.y_axis().title_style().font = font;
  a_plotter.y_axis().title_style().scale = majic;
  a_plotter.y_axis().title_style().smoothing = smoothing;
  a_plotter.y_axis().mag_style().scale = majic;
  a_plotter.y_axis().mag_style().font = font;
  a_plotter.y_axis().mag_style().smoothing = smoothing;
  a_plotter.y_axis().line_style().width = 2;
  a_plotter.y_axis().ticks_style().width = 2;
  a_plotter.y_axis().label_height = 0.05f;
  a_plotter.y_axis().title_height = 0.06f;
  a_plotter.y_axis().title_to_axis = XLAB;
  a_plotter.y_axis().label_to_axis = 0.01f;
  // Set hplot tick modeling :
  a_plotter.y_axis().modeling = inlib::sg::tick_modeling_hplot();
  a_plotter.y_axis().divisions = 505;
  
  // title :
  a_plotter.title_height = 0.04f;
  a_plotter.title_to_axis = 0.02f;
  a_plotter.title_style().font = font;
  a_plotter.title_style().smoothing = smoothing;
  a_plotter.title_style().color = inlib::colorf_black();

  // info box :
  a_plotter.infos_style().modeling = inlib::sg::infos_modeling_ROOT();
  a_plotter.infos_style().font = font;
  a_plotter.infos_style().front_face = inlib::sg::winding_cw;
  a_plotter.infos_width = 0.2f;
  a_plotter.infos_x_margin = 0.01f; //percent of plotter width.
  a_plotter.infos_y_margin = 0.01f; //percent of plotter height.
  /*
  // legend box :
  a_plotter.setParameter("legendRegionVisible","TRUE");
  a_plotter.setParameter("legendRegion.viewportRegion.backgroundColor","1 1 0");
  a_plotter.setParameter("legendRegionSize","0.15 0.16");
  a_plotter.setParameter("legendRegionOrigin","0.1 0.1");
  a_plotter.setParameter("legendRegion.horizontalMargin","2");
  a_plotter.setParameter("legendRegion.verticalMargin","2");
  */
  
  //gStyle->SetPadBottomMargin(0.15);
  //gStyle->SetPadLeftMargin(0.15);
  //gStyle->SetTitleSize(0.06,"XYZ"); //ROOT def 0.04. SoPlotter : 0.014
  //gStyle->SetLabelOffset(0.01,"Y"); //ROOT def 0.005. SoPlotter def 0.02
  //gStyle->SetTitleOffset(1.1,"Y");
  //gStyle->SetLabelSize(0.05,"XYZ"); //ROOT def 0.04. SoPlotter def 0.014.  
  
}
inline bool plot(inlib::histo::h1d& aHisto1D,inlib::histo::h2d& aHisto2D) {
  
  exlib::EXLIB_SCREEN_MGR::session smgr(std::cout);
  if(!smgr.is_valid()) return false;

  exlib::EXLIB_SCREEN_MGR::plotter plotter(smgr,1,2,0,0,800,600);
  if(!plotter.window()) return false;
  inlib::env_append_path("EXLIB_FONT_PATH",".");    
  plotter.plots().view_border = false;

  if(plotter.plots().set_current_plotter(0)) {
    inlib::sg::plotter& sgp = plotter.plots().current_plotter();
    set_region_style(sgp);
    sgp.x_axis().title = "time";
    sgp.y_axis().title = "Entries";
    inlib::sg::plottable* ptb = new inlib::sg::h1d2plot(aHisto1D);
    ptb->set_name(aHisto1D.title());
    sgp.add_plottable(ptb);
  }

  if(plotter.plots().set_current_plotter(1)) {
    inlib::sg::plotter& sgp = plotter.plots().current_plotter();
    set_region_style(sgp);
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

#ifdef APP_USE_ARCHIVE
#include <BatchLab/Core/Main.h>
extern "C" {
  void BatchLabRioInitialize(Slash::Core::ISession&);
}
#endif

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
int main(int aArgc,char** aArgv) {

  ////////////////////////////////////////////////////////
  // Create histograms : /////////////////////////////////
  ////////////////////////////////////////////////////////
  inlib::histo::h1d hits_times("Hits times",100,0,3000);
  inlib::histo::h2d digits_time_pe("Digits PE time",100,0,3000,100,0,10);

  ////////////////////////////////////////////////////////
  /// Read data : ////////////////////////////////////////
  ////////////////////////////////////////////////////////
#ifdef APP_USE_ARCHIVE
  BatchLab::Main* session = new BatchLab::Main(std::vector<std::string>());
  Slash::Core::ILibraryManager* libraryManager = Slash::libraryManager(*session);
  if(!libraryManager) {std::cout << "LibraryManager not found." << std::endl;return EXIT_FAILURE;}
  Slash::Core::ILibrary* library = libraryManager->addLibrary("Rio","main","");
  if(!library) {std::cout << "addLibrary() failed." << std::endl;return EXIT_FAILURE;}
  ::BatchLabRioInitialize(*session);
  AIDA::IAnalysisFactory* aida = static_cast<AIDA::IAnalysisFactory*>(session);
#else
  AIDA::IAnalysisFactory* aida = AIDA_createAnalysisFactory();
#endif
  if(!aida) {
    std::cout << "AIDA not found." << std::endl;
    return EXIT_FAILURE;
  }

  if(!read_data(*aida,hits_times,digits_time_pe)) {
    std::cout << "can't read data file." << std::endl;
    delete aida;
    return EXIT_FAILURE;
  }

  delete aida;

  ////////////////////////////////////////////////////////
  /// plot histos : //////////////////////////////////////
  ////////////////////////////////////////////////////////
  plot(hits_times,digits_time_pe);

  return EXIT_SUCCESS;
}
