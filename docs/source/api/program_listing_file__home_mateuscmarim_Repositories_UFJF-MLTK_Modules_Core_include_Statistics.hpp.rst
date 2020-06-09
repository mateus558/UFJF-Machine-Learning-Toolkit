
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Core_include_Statistics.hpp:

Program Listing for File Statistics.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Core_include_Statistics.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Core/include/Statistics.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef STATISTICS__HPP
   #define STATISTICS__HPP
   
   #include <vector>
   #include <cmath>
   #include <iostream>
   #include "Point.hpp"
   #include "Data.hpp"
   
   template < typename T > class Data;
   
   template  < typename T >
   class Statistics {
       // Attributes
       friend class Data< T >;
   private :
       Point< T > centroid;
       Point< T > pos_centroid;
       Point< T > neg_centroid;
       size_t n_pos = 0;
       size_t n_neg = 0;
   // Operations
   public:
       Statistics (){}
       static double mean (std::vector< T > p);
       static double getFeatureMean (std::shared_ptr<Data< T > > data, int index);
       static double variance (std::vector< T > p);
       static double variance (std::shared_ptr<Data< T > > data, int index);
       static double stdev (std::vector< T > p);
       static double getFeatureStdev (std::shared_ptr<Data< T > > data, int index);
       static double getRadius(std::shared_ptr<Data< T > > data, int index, double q);
       static double getDistCenters(std::shared_ptr<Data< T > > data, int index);
       static double getDistCentersWithoutFeats(std::shared_ptr<Data< T > > data, std::vector<int> feats, int index);
   };
   
   #endif
