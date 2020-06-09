
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Visualize_include_gnuplot_i.hpp:

Program Listing for File gnuplot_i.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Visualize_include_gnuplot_i.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Visualize/include/gnuplot_i.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   
   #ifndef _GNUPLOT_PIPES_H_
   #define _GNUPLOT_PIPES_H_
   
   
   #include <iostream>
   #include <string>
   #include <vector>
   #include <fstream>
   #include <sstream>              // for std::ostringstream
   #include <stdexcept>
   #include <cstdio>
   #include <cstdlib>              // for getenv()
   #include <list>                 // for std::list
   
   #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
   //defined for 32 and 64-bit environments
    #include <io.h>                // for _access(), _mktemp()
    #define GP_MAX_TMP_FILES  27   // 27 temporary files it's Microsoft restriction
   #elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
   //all UNIX-like OSs (Linux, *BSD, MacOSX, Solaris, ...)
   #include <unistd.h>            // for access(), mkstemp()
   #define GP_MAX_TMP_FILES  64
   #else
   #error unsupported or unknown operating system
   #endif
   
   //declare classes in global namespace
   
   
   class GnuplotException : public std::runtime_error
   {
   public:
       GnuplotException(const std::string &msg) : std::runtime_error(msg){}
   };
   
   
   
   class Gnuplot
   {
   private:
   
       //----------------------------------------------------------------------------------
       // member data
       FILE                    *gnucmd;
       bool                     valid;
       bool                     two_dim;
       int                      nplots;
       std::string              pstyle;
       std::string              smooth;
       std::vector<std::string> tmpfile_list;
   
       //----------------------------------------------------------------------------------
       // static data
       static int               tmpfile_num;
       static std::string       m_sGNUPlotFileName;
       static std::string       m_sGNUPlotPath;
       static std::string       terminal_std;
   
       //----------------------------------------------------------------------------------
       // member functions (auxiliary functions)
       // ---------------------------------------------------
       // ---------------------------------------------------
       void           init();
       // ---------------------------------------------------
       // ---------------------------------------------------
       std::string    create_tmpfile(std::ofstream &tmp);
   
       //----------------------------------------------------------------------------------
       // ---------------------------------------------------------------------------------
       static bool    get_program_path();
   
       // ---------------------------------------------------------------------------------
       // ---------------------------------------------------------------------------------
       bool file_available(const std::string &filename);
   
       // ---------------------------------------------------------------------------------
       // ---------------------------------------------------------------------------------
       static bool    file_exists(const std::string &filename, int mode=0);
   
   public:
   
       // ----------------------------------------------------------------------------
       // ----------------------------------------------------------------------------
       static bool set_GNUPlotPath(const std::string &path);
   
   
       // ----------------------------------------------------------------------------
       // ----------------------------------------------------------------------------
       static void set_terminal_std(const std::string &type);
   
       //-----------------------------------------------------------------------------
       // constructors
       // ----------------------------------------------------------------------------
   
   
       Gnuplot(const std::string &style = "points");
   
       Gnuplot(const std::vector<double> &x,
               const std::string &title = "",
               const std::string &style = "points",
               const std::string &labelx = "x",
               const std::string &labely = "y");
   
       Gnuplot(const std::vector<double> &x,
               const std::vector<double> &y,
               const std::string &title = "",
               const std::string &style = "points",
               const std::string &labelx = "x",
               const std::string &labely = "y");
   
       Gnuplot(const std::vector<double> &x,
               const std::vector<double> &y,
               const std::vector<double> &z,
               const std::string &title = "",
               const std::string &style = "points",
               const std::string &labelx = "x",
               const std::string &labely = "y",
               const std::string &labelz = "z");
   
       ~Gnuplot();
   
   
       //----------------------------------------------------------------------------------
   
       Gnuplot& cmd(const std::string &cmdstr);
       // ---------------------------------------------------------------------------------
       // ---------------------------------------------------------------------------------
       inline Gnuplot& operator<<(const std::string &cmdstr){
           cmd(cmdstr);
           return(*this);
       }
   
   
   
       //----------------------------------------------------------------------------------
       // show on screen or write to file
   
       Gnuplot& showonscreen(); // window output is set by default (win/x11/aqua)
   
       Gnuplot& savetops(const std::string &filename = "gnuplot_output");
   
   
       //----------------------------------------------------------------------------------
       // set and unset
   
       Gnuplot& set_style(const std::string &stylestr = "points");
   
       Gnuplot& set_smooth(const std::string &stylestr = "csplines");
   
       // ----------------------------------------------------------------------
       // ----------------------------------------------------------------------
       inline Gnuplot& unset_smooth(){ smooth = ""; return *this;};
   
   
       Gnuplot& set_pointsize(const double pointsize = 1.0);
   
       inline Gnuplot& set_grid()  {cmd("set grid");return *this;};
       inline Gnuplot& unset_grid(){cmd("unset grid");return *this;};
   
       // -----------------------------------------------
       // -----------------------------------------------
       inline Gnuplot& set_multiplot(){cmd("set multiplot") ;return *this;};
   
       // -----------------------------------------------
       // -----------------------------------------------
       inline Gnuplot& unset_multiplot(){cmd("unset multiplot");return *this;};
   
   
   
       Gnuplot& set_samples(const int samples = 100);
       Gnuplot& set_isosamples(const int isolines = 10);
   
       // --------------------------------------------------------------------------
       // --------------------------------------------------------------------------
       Gnuplot& set_hidden3d(){cmd("set hidden3d");return *this;};
   
       // ---------------------------------------------------------------------------
       // ---------------------------------------------------------------------------
       inline Gnuplot& unset_hidden3d(){cmd("unset hidden3d"); return *this;};
   
       Gnuplot& set_contour(const std::string &position = "base");
       // --------------------------------------------------------------------------
       // ------------------------------------------------------------------
       inline Gnuplot& unset_contour(){cmd("unset contour");return *this;};
   
       // ------------------------------------------------------------
       // ------------------------------------------------------------------
       inline Gnuplot& set_surface(){cmd("set surface");return *this;};
   
       // ----------------------------------------------------------
       // ------------------------------------------------------------------
       inline Gnuplot& unset_surface(){cmd("unset surface"); return *this;}
   
   
       Gnuplot& set_legend(const std::string &position = "default");
   
       // ------------------------------------------------------------------
       // ------------------------------------------------------------------
       inline Gnuplot& unset_legend(){cmd("unset key"); return *this;}
   
       // -----------------------------------------------------------------------
       // -----------------------------------------------------------------------
       inline Gnuplot& set_title(const std::string &title = "")
       {
           std::string cmdstr;
           cmdstr = "set title \"";
           cmdstr+=title;
           cmdstr+="\"";
           *this<<cmdstr;
           return *this;
       }
   
       //----------------------------------------------------------------------------------
       // ---------------------------------------------------------------------------------
       inline Gnuplot& unset_title(){this->set_title();return *this;}
   
   
       Gnuplot& set_ylabel(const std::string &label = "x");
       Gnuplot& set_xlabel(const std::string &label = "y");
       Gnuplot& set_zlabel(const std::string &label = "z");
   
       Gnuplot& set_xrange(const double iFrom,
                           const double iTo);
       Gnuplot& set_yrange(const double iFrom,
                           const double iTo);
       Gnuplot& set_zrange(const double iFrom,
                           const double iTo);
       // -----------------------------------------------
       inline Gnuplot& set_xautoscale(){cmd("set xrange restore");cmd("set autoscale x");return *this;};
   
       // -----------------------------------------------
       // -----------------------------------------------
       inline Gnuplot& set_yautoscale(){cmd("set yrange restore");cmd("set autoscale y");return *this;};
   
       // -----------------------------------------------
       // -----------------------------------------------
       inline Gnuplot& set_zautoscale(){cmd("set zrange restore");cmd("set autoscale z");return *this;};
   
   
       Gnuplot& set_xlogscale(const double base = 10);
       Gnuplot& set_ylogscale(const double base = 10);
       Gnuplot& set_zlogscale(const double base = 10);
   
       // -----------------------------------------------
       // -----------------------------------------------
       inline Gnuplot& unset_xlogscale(){cmd("unset logscale x"); return *this;};
   
       // -----------------------------------------------
       // -----------------------------------------------
       inline Gnuplot& unset_ylogscale(){cmd("unset logscale y"); return *this;};
   
       // -----------------------------------------------
       // -----------------------------------------------
       inline Gnuplot& unset_zlogscale(){cmd("unset logscale z"); return *this;};
   
   
       Gnuplot& set_cbrange(const double iFrom, const double iTo);
   
   
       //----------------------------------------------------------------------------------
       // plot
   
       Gnuplot& plotfile_x(const std::string &filename,
                           const unsigned int column = 1,
                           const std::string &title = "");
       template<typename X>
       Gnuplot& plot_x(const X& x, const std::string &title = "");
   
   
       Gnuplot& plotfile_xy(const std::string &filename,
                            const unsigned int column_x = 1,
                            const unsigned int column_y = 2,
                            const std::string &title = "");
       template<typename X, typename Y>
       Gnuplot& plot_xy(const X& x, const Y& y, const std::string &title = "");
   
   
       Gnuplot& plotfile_xy_err(const std::string &filename,
                                const unsigned int column_x  = 1,
                                const unsigned int column_y  = 2,
                                const unsigned int column_dy = 3,
                                const std::string &title = "");
       template<typename X, typename Y, typename E>
       Gnuplot& plot_xy_err(const X &x, const Y &y, const E &dy,
                            const std::string &title = "");
   
   
       Gnuplot& plotfile_xyz(const std::string &filename,
                             const unsigned int column_x = 1,
                             const unsigned int column_y = 2,
                             const unsigned int column_z = 3,
                             const std::string &title = "");
       template<typename X, typename Y, typename Z>
       Gnuplot& plot_xyz(const X &x,
                         const Y &y,
                         const Z &z,
                         const std::string &title = "");
   
   
   
       Gnuplot& plot_slope(const double a,
                           const double b,
                           const std::string &title = "");
   
   
       Gnuplot& plot_equation(const std::string &equation,
                              const std::string &title = "");
   
       Gnuplot& plot_equation3d(const std::string &equation,
                                const std::string &title = "");
   
   
       Gnuplot& plot_image(const unsigned char *ucPicBuf,
                           const unsigned int iWidth,
                           const unsigned int iHeight,
                           const std::string &title = "");
   
   
       //----------------------------------------------------------------------------------
       //----------------------------------------------------------------------------------
       inline Gnuplot& replot(void){if (nplots > 0) cmd("replot");return *this;};
   
       Gnuplot& reset_plot();
   
       Gnuplot& reset_all();
   
       void remove_tmpfiles();
   
       // -------------------------------------------------------------------
       // -------------------------------------------------------------------
       inline bool is_valid(){return(valid);};
   
   };
   
   #endif
