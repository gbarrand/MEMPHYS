
// A MEMPHYS.root file is read with the inlib/rroot API and plotting is done with inlib/exlib.

#include "read.icc"

#include "plotter_style.icc"

#include <inlib/histo/h1d>
#include <inlib/histo/h2d>

#include <inlib/viewplot>

#include <exlib/sg/text_freetype>

#define MEMPHYS_READ_PLOT_OFFSCREEN_USE_ZB_PNG
#define MEMPHYS_READ_PLOT_OFFSCREEN_USE_ZB_GL2PS

#ifdef MEMPHYS_READ_PLOT_OFFSCREEN_USE_ZB_PNG
#include <inlib/sg/zb_action>
#include <exlib/png>
#endif
#ifdef MEMPHYS_READ_PLOT_OFFSCREEN_USE_ZB_GL2PS
#include <inlib/wps>
#include <exlib/sg/gl2ps_action>
#endif

inline bool plot(std::ostream& a_out,
		 inlib::histo::h1d& aHisto1D,inlib::histo::h2d& aHisto2D,
		 bool a_ttf) {
  
  inlib::env_append_path("EXLIB_FONT_PATH",".");    

  unsigned int ww = 2000; //to have better antialising on freetype fonts.
  float A4 = 29.7f/21.0f;
  unsigned int wh = (unsigned int)(float(ww)*A4);

  exlib::sg::text_freetype ttf;
  inlib::viewplot viewer(a_out,ttf,1,1,ww,wh);

  viewer.plots().view_border = false;

  //if(inlib::file::exists("viewplot.style")) {
  //  exlib::xml::load_style_file(viewer.styles(),"viewplot.style");
  //} else {
  //load_embeded_styles(viewer.styles());
  //}

  viewer.styles().add_colormap("default",inlib::sg::style_default_colormap());
  viewer.styles().add_colormap("ROOT",inlib::sg::style_ROOT_colormap());

  //////////////////////////////////////////////////////////
  /// two pages with one plot per page : ///////////////////
  //////////////////////////////////////////////////////////
  viewer.open_file("read_plot_offscreen.ps");

  viewer.set_cols_rows(1,2);

  if(viewer.plots().set_current_plotter(0)) {
    inlib::sg::plotter& sgp = viewer.plots().current_plotter();
    set_region_style(sgp,a_ttf);
    sgp.x_axis().title = "time";
    sgp.y_axis().title = "Entries";
    inlib::sg::plottable* ptb = new inlib::sg::h1d2plot(aHisto1D);
    ptb->set_name(aHisto1D.title());
    sgp.add_plottable(ptb);
  }

  if(viewer.plots().set_current_plotter(1)) {
    inlib::sg::plotter& sgp = viewer.plots().current_plotter();
    set_region_style(sgp,a_ttf);
    sgp.x_axis().title = "time";
    sgp.y_axis().title = "PE";
    inlib::sg::plottable* ptb = new inlib::sg::h2d2plot(aHisto2D);
    ptb->set_name(aHisto2D.title());
    sgp.add_plottable(ptb);
  }

  viewer.write_page();
  
  viewer.close_file();

#if defined(MEMPHYS_READ_PLOT_OFFSCREEN_USE_ZB_PNG) || defined(MEMPHYS_READ_PLOT_OFFSCREEN_USE_ZB_GL2PS)
  bool do_png = true;
  std::string png_file = "read_plot_offscreen.png";
  bool do_zb_ps = true;
  std::string zb_ps_file = "read_plot_offscreen_zb.ps";
  
  inlib::sg::zb_manager mgr;
  inlib::sg::zb_action action(mgr,a_out,ww,wh);

  inlib::colorf clear_color = inlib::colorf_white();
  action.zbuffer().clear_color_buffer(0);
  action.add_color(clear_color.r(),clear_color.g(),clear_color.b());
  action.zbuffer().clear_depth_buffer();  

  viewer.sg().render(action);
#endif
  
#ifdef MEMPHYS_READ_PLOT_OFFSCREEN_USE_ZB_PNG
  if(do_png) {
    size_t sz;
    unsigned char* buffer = action.get_rgbas(sz);
    if(!buffer) {
      a_out << "can't get rgba image." << std::endl;
    } else {
      unsigned int bpp = 4;
      exlib::png::write(a_out,png_file,buffer,ww,wh,bpp);
      delete [] buffer;
    }
  }
#endif  
#ifdef MEMPHYS_READ_PLOT_OFFSCREEN_USE_ZB_GL2PS
  if(do_zb_ps) {
    inlib::wps wps(a_out);
    if(!wps.open_file(zb_ps_file)) {
      a_out << "can't open " << zb_ps_file << "." << std::endl;
    } else {
      wps.PS_BEGIN_PAGE();
      wps.PS_PAGE_SCALE(float(ww),float(wh));
      // put zbuffer in out.ps. The get_rgb function is the bridge
      // between zb_action and wps.
      wps.PS_IMAGE(ww,wh,inlib::wps::rgb_4,inlib::sg::zb_action::get_rgb,&action);
      wps.PS_END_PAGE();
      wps.close_file();  
    }
  }
#endif
  
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

  //bool ttf = args.is_arg("-ttf");
  bool ttf = true; //enforce ttf.
  
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
  std::cout << "MEMPHYS_read_plot_offscreen : exit(mem) ..." << std::endl;
#else  
  if(verbose) std::cout << "MEMPHYS_read_plot_offscreen : exit ..." << std::endl;
#endif //INLIB_MEM
  
  return EXIT_SUCCESS;
}
