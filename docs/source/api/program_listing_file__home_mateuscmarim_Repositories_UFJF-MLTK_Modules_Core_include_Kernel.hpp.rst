
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Core_include_Kernel.hpp:

Program Listing for File Kernel.hpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Core_include_Kernel.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Core/include/Kernel.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef KERNEL__HPP
   #define KERNEL__HPP
   
   #include <vector>
   #include <memory>
   #include <cmath>
   #include <utility>
   
   #include "Data.hpp"
   #include "Utils.hpp"
   
   class Kernel {
       // Attributes
   private :
       int type{};
       double param{};
       dMatrix K;
       dMatrix H;
       dMatrix HwithoutDim;
   public :
       Kernel(int type = 0, double param = 0);
       dMatrix* getKernelMatrixPointer();
       Kernel(dMatrix kernel_matrix);
       void setType(int type);
       void setParam(double param);
       int getType();
       double getParam();
       void setKernelMatrix(dMatrix K);
       dMatrix getKernelMatrix();
       template < typename T >
       void compute(std::shared_ptr<Data< T > > samples);
       template < typename T >
       dMatrix* generateMatrixH(std::shared_ptr<Data< T > > samples);
       template < typename T >
       dMatrix* generateMatrixHwithoutDim(std::shared_ptr<Data< T > > samples, int dim);
       template < typename T >
       double function(std::shared_ptr<Point< T > > one, std::shared_ptr<Point< T > > two, int dim);
       template < typename T >
       double functionWithoutDim(std::shared_ptr<Point< T > > one, std::shared_ptr<Point< T > > two, int j, int dim);
       template < typename T >
       double norm(Data< T > data);
       template < typename T >
       double featureSpaceNorm(std::shared_ptr<Data< T > > data);
   };
   
   template < typename T >
   void Kernel::compute(const std::shared_ptr<Data< T > > samples){
       size_t i, j, size = samples->getSize(), dim = samples->getDim();
   
       K.assign(size, std::vector<double>(size, 0.0));
   
       //Calculating Matrix
       for(i = 0; i < size; ++i){
           for(j = i; j < size; ++j){
               K[i][j] = function((*samples)[i], (*samples)[j], dim);
               K[j][i] = K[i][j];
           }
       }
   }
   
   template < typename T >
   dMatrix* Kernel::generateMatrixH(const std::shared_ptr<Data< T > > samples) {
       int i = 0, j = 0;
       size_t size = samples->getSize(), dim = samples->getDim();
   
       H.resize(size, std::vector<double>(size));
   
       /* Calculating Matrix */
       for(i = 0; i < size; ++i) {
           for (j = i; j < size; ++j) {
               H[i][j] = function(samples->getPoint(i), samples->getPoint(j), dim) * samples->getPoint(i)->y *
                         samples->getPoint(j)->y;
               H[j][i] = H[i][j];
           }
       }
       std::clog << "\nH matrix generated.\n";
       return &H;
   }
   
   template < typename T >
   dMatrix* Kernel::generateMatrixHwithoutDim(const std::shared_ptr<Data< T > > samples, int dim) {
       int i = 0, j = 0;
       size_t size = samples->getSize();
   
       HwithoutDim.resize(size, std::vector<double>(size));
   
       /* Calculating Matrix */
       for(i = 0; i < size; ++i) {
           for (j = i; j < size; ++j) {
               HwithoutDim[i][j] = functionWithoutDim((*samples)[i], (*samples)[j], dim, samples->getDim()) *
                                   samples->getPoint(i)->y * samples->getPoint(j)->y;
               HwithoutDim[j][i] = HwithoutDim[i][j];
           }
       }
      // clog << "\nH matrix without dim generated.\n";
       return &HwithoutDim;
   }
   
   template < typename T >
   double Kernel::function(std::shared_ptr<Point< T > > one, std::shared_ptr<Point< T > > two, int dim){
       int i = 0;
       double t, sum = 0.0;
       std::vector< T > a = one->x, b = two->x;
   
       // a.erase(a.end());
       //b.erase(b.end());
   
       switch(type)
       {
           case 0: //Produto Interno
               for(i = 0; i < dim; ++i)
                   sum += a[i] * b[i];
               break;
           case 1: //Polinomial
               for(i = 0; i < dim; ++i)
                   sum += a[i] * b[i];
               //    sum = (param > 1) ? pow(sum+1, param) : sum;
               sum = (param > 1) ? pow(sum, param) : sum;
               break;
   
           case 2: //Gaussiano
               for(i = 0; i < dim; ++i)
               { t = a[i] - b[i]; sum += t * t; }
               sum = exp(-1 * sum * param);
               break;
       }
   
       /*The '+1' here accounts for the bias term "b" in SVM formulation since
         <w,x> = \sum_i \alpha_i y_i k(x_i,x) + b and b=\sum_i \alpha_i y_i*/
   
       return sum;// + 1.0f;
   }
   
   template < typename T >
   double Kernel::functionWithoutDim(std::shared_ptr<Point< T > > one, std::shared_ptr<Point< T > > two, int j, int dim) {
       int i = 0;
       double t, sum = 0.0;
   
       switch(type)
       {
           case 0: //Produto Interno
               for(i = 0; i < dim; ++i)
                   if(i != j)
                       sum += one->x[i] * two->x[i];
               break;
   
           case 1: //Polinomial
               for(i = 0; i < dim; ++i)
                   if(i != j)
                       sum += one->x[i] * two->x[i];
               sum = (param > 1) ? pow(sum+1, param) : sum;
               break;
   
           case 2: //Gaussiano
               for(i = 0; i < dim; ++i)
                   if(i != j)
                   { t = one->x[i] - two->x[i]; sum += t * t; }
               sum = exp(-1 * sum * param);
               break;
       }
       /*The '+1' here accounts for the bias term "b" in SVM formulation since
         <w,x> = \sum_i \alpha_i y_i k(x_i,x) + b and b=\sum_i \alpha_i y_i*/
       return sum;// + 1.0f;
   }
   
   template < typename T >
   double Kernel::norm(Data< T > data){
       size_t i, j, size = data.getSize();
       double sum, sum1;
       auto points = data.getPoints();
   
       sum = sum1 = 0;
   
       for(i = 0; i < size; ++i){
           for(j = 0; j < size; j++){
               sum1 += points[j]->alpha * points[j]->y * K[i][j];
               sum += points[i]->y * points[i]->alpha * sum1;
           }
       }
   
       return sqrt(sum);
   }
   
   template < typename T >
   double Kernel::featureSpaceNorm(std::shared_ptr<Data< T > > data) {
       size_t i = 0, j = 0, size = data->getSize();
       double sum1 = 0.0;
       double sum  = 0.0;
   
       for(i = 0; i < size; ++i)
       {
           if((*data)[i]->alpha > 0)
           {
               sum1 = 0.0;
               for(j = 0; j < size; ++j)
               {
                   if((*data)[j]->alpha > 0)
                       sum1 += (*data)[j]->y * (*data)[j]->alpha * K[j][i];
               }
               sum += (*data)[i]->alpha * (*data)[i]->y * sum1;
           }
       }
       sum = sqrt(sum);
   
       return sum;
   }
   
   #endif
