
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Core_src_Kernel.cpp:

Program Listing for File Kernel.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Core_src_Kernel.cpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Core/src/Kernel.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "Kernel.hpp"
   
   #include <utility>
   
   using namespace std;
   
   Kernel::Kernel(int type, double param){
       this->type = type;
       this->param = param;
   }
   
   Kernel::Kernel(dMatrix kernel_matrix){
       this->K = std::move(kernel_matrix);
   }
   
   int Kernel::getType(){
       return type;
   }
   
   double Kernel::getParam(){
       return param;
   }
   
   void Kernel::setType(int _type){
       this->type = _type;
   }
   
   void Kernel::setParam(double _param){
       this->param = _param;
   }
   
   void Kernel::setKernelMatrix(dMatrix _K){
       this->K = std::move(_K);
   }
   
   dMatrix Kernel::getKernelMatrix(){
       return K;
   }
   
   dMatrix* Kernel::getKernelMatrixPointer(){
       return &K;
   }
   
   
