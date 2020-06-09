
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Regressor_include_LMS.hpp:

Program Listing for File LMS.hpp
================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Regressor_include_LMS.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Regressor/include/LMS.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   //
   // Created by mateus558 on 03/01/19.
   //
   
   #ifndef UFJF_MLTK_LMS_H
   #define UFJF_MLTK_LMS_H
   
   #include "PrimalRegressor.hpp"
   //#include "DualRegressor.hpp"
   
   template < typename T >
   class LMSPrimal: public PrimalRegressor< T > {
   public:
       explicit LMSPrimal(std::shared_ptr<Data< T > > samples = nullptr, double rate = 0.5, int verbose = 0, Solution *initial_solution = nullptr);
       bool train() override;
   };
   
   #endif //UFJF_MLTK_LMS_H
