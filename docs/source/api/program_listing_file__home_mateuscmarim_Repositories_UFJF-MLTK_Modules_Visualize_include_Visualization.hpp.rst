
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Visualize_include_Visualization.hpp:

Program Listing for File Visualization.hpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Visualize_include_Visualization.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Visualize/include/Visualization.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef VISUALIZATION__HPP
   #define VISUALIZATION__HPP
   #ifdef __unix__
   #include <dirent.h>
   #include "gnuplot_i.hpp"
   #elif _WIN32
   #include <windows.h>
   #endif
   #include "Data.hpp"
   #include "Solution.hpp"
   
   #include <string>
   
   
   template < typename T >
   class Visualization {
       // Attributes
   private :
       Data< T > *samples;
   #ifdef __unix__
       Gnuplot g;
   #elif _WIN32
   
   #endif
   
       std::vector<std::string> createTempFiles();
       bool valid_file(std::string file);
       std::vector<std::string> getTempFilesNames();
       void removeTempFiles();
       // Operations
   public :
       Visualization ();
       Visualization (Data<T> &sample);
   
       /*********************************************
        *               Setters                     *
        *********************************************/
   
       void setSample (Data< T > *sample);
       void setTitle (std::string title);
       void setStyle (std::string style);
       
       void plot2D(int x, int y);
       void plot3D(int x, int y, int z);
       void plot2DwithHyperplane(int x, int y, Solution w);
       void plot3DwithHyperplane(int x, int y, int z, Solution w);
       ~Visualization();
   };
   
   #endif
