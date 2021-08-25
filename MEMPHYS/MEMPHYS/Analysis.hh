#ifndef MEMPHYS_Analysis_h
#define MEMPHYS_Analysis_h

#ifdef APP_USE_INLIB_WROOT
#include <exlib/zlib>
#include <inlib/wroot/file>
#include <inlib/wroot/tree>
#include <iostream>
#endif

#include <string>

namespace MEMPHYS {

class Analysis  {
public:        
  Analysis(const std::string& a_file,bool a_use_file)
  :m_file_name(a_file)
  ,m_use_file(a_use_file) 
#ifdef APP_USE_INLIB_WROOT
  ,m_file(0)
#include "Event_tree_zero.icc"
#include "Geometry_tree_zero.icc"
#endif
  {
    // Event "trees in trees" structure :
    /*
    //JEC 16/11/05: add tubeId in the hit structure
    std::string column = "int eventId, inputEvtId, interMode, vtxVol, "; 
    column += "double[] vtxPos, ";
    column += "int nPart, leptonIndex, protonIndex, ";
    column += "ITuple track = { int pId, parent, float timeStart, ";
    column +=                  "double[] direction, ";
    column +=                  "double mass, pTot, ETot, ";
    column +=                  "double[] momentum, ";
    column +=                  "double[] startPos, ";
    column +=                  "double[] stopPos, ";
    column +=                  "int startVol, stopVol ";
    column +=                "}, ";
    column += "int nHits, ITuple hit = { int tubeId, int totalPE, float[] pe }, ";
    column += "int nDigits, double sumPE, ";
    column += "ITuple digit = { int tubeId, double pe, time }";
    */

    //Time tuple for Interactive session needed for the time beeing.
    // (G.Barrand does'nt yet manage the Tuple into Tuple in his Tuple Explorer (JEC 5/4/06)
    //column = "float time";
    //tf->create("HitTime","MEMPHYS WC hit time info",column,"");

    // Geometry "trees in trees" structure :
    //Needed for the visualization program.
    /*
    std::string column =  "double wcRadius, wcLength, double[] wcOffset, ";
    column += "double pmtRadius, int nPMTs,  ";
    column += "ITuple pmtInfos = { ";
    column +=                      "int pmtId, pmtLocation, ";
    column +=                      "double[] pmtOrient, ";
    column +=                      "double[] pmtPosition ";
    column +=                    "}";
    */
  
#ifdef APP_USE_INLIB_WROOT
    if(m_use_file) {
      m_file = new inlib::wroot::file(std::cout,m_file_name);
      m_file->add_ziper('Z',exlib::compress_buffer);
      m_file->set_compression(1); //max is 9.
#include "Event_tree_create.icc"
#include "Geometry_tree_create.icc"
  }
#endif
}
  
  virtual ~Analysis(){  
#ifdef APP_USE_INLIB_WROOT
    if(m_use_file && m_file) {
      unsigned int n;
      if(!m_file->write(n)) {
        std::cout << "file write failed." << std::endl;
      }
      m_file->close(); // m_file dstor will delete m_Event_tree, m_Geometry_tree.
      delete m_file;
    }
  
    delete m_Event_track_tree;
    delete m_Event_hit_tree;
    delete m_Event_digit_tree;
  
    delete m_Geometry_pmtInfos_tree;
#endif
  }


protected:
#ifdef APP_USE_INLIB_WROOT
  Analysis(const Analysis&):m_use_file(false),m_file(0) {}
#endif
  Analysis& operator=(const Analysis&) {return *this;}
public:
  bool use_file() const {return m_use_file;}
private:
  std::string m_file_name;
  bool m_use_file;
#ifdef APP_USE_INLIB_WROOT
public:
  inlib::wroot::file* m_file;
public:
#include "Event_tree_h.icc"
#include "Geometry_tree_h.icc"
#endif
};

}
#endif
