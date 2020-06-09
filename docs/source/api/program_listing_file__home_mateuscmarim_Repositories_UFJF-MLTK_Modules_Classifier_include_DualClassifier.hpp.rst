
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Classifier_include_DualClassifier.hpp:

Program Listing for File DualClassifier.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Classifier_include_DualClassifier.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Classifier/include/DualClassifier.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #ifndef DUALCLASSIFIER__HPP
   #define DUALCLASSIFIER__HPP
   
   #include "Kernel.hpp"
   #include "Classifier.hpp"
   #include <vector>
   
   template < typename T >
   class DualClassifier : public Classifier< T > {
       // Associations
       // Attributes
   protected:
       std::vector<double> alpha;
       Kernel *kernel = nullptr;
   public:
   
       double evaluate(Point< T > p) override {
           double func, bias = this->solution.bias, fk = 0.0, lambda;
           size_t size = this->samples->getSize(), dim = this->samples->getDim(), r;
           auto po = std::make_shared<Point< T > >(p);
   
           if(p.x.size() != dim){
               std::cerr << "The point must have the same dimension of the feature set!" << std::endl;
               return 0;
           }
   
           for(func = bias, r = 0; r < size; ++r){
               fk = this->kernel->function(po, (*this->samples)[r], dim);
               func  += (*this->samples)[r]->alpha * (*this->samples)[r]->y * fk;
           }
   
           return (func >= 0)?1:-1;
       }
   
       /*********************************************
        *               Setters                     *
        *********************************************/
   
       inline void setKernel(Kernel *K){ this->kernel = K; }
       inline void setKernelType(int type){ kernel->setType(type); }
       inline void setKernelParam(double param){ kernel->setParam(param); }
   
       /*********************************************
        *               Getters                     *
        *********************************************/
   
       std::string getFormulationString() override { return "Dual"; }
   
       inline double getKernelParam(){ return kernel->getParam(); }
       inline double getKernelType(){ return kernel->getType(); }
       inline std::vector<double> getAlphaVector() { return alpha; }
       std::vector<double> getWeight(){
           size_t i, j, dim = this->samples->getDim(), size = this->samples->getSize();
           std::vector<double> w(dim);
   
           for(i = 0; i < dim; i++){
               for(j = 0; j < size; j++){
                   w[i] += (*this->samples)[j]->alpha*(*this->samples)[j]->y*(*this->samples)[j]->x[i];
               }
           }
   
           return w;
       }
       std::vector<double> getDualWeight(){
           int i = 0, j = 0, k = 0;
           size_t size = this->samples->getSize(), dim = this->samples->getDim();
           dMatrix *H, *Hk, matrixdif(size, std::vector<double>(size));
           std::vector<double> alphaaux(size);
   
           H = kernel->generateMatrixH(this->samples);
   
           this->solution.w.resize(dim);
   
           for(k = 0; k < dim; ++k)
           {
               Hk = kernel->generateMatrixHwithoutDim(this->samples, k);
   
               for(i = 0; i < size; ++i)
                   for(j = 0; j < size; ++j)
                       matrixdif[i][j] = (*H)[i][j] - (*Hk)[i][j];
   
               for(i = 0; i < size; ++i)
                   for(alphaaux[i] = 0, j = 0; j < size; ++j)
                       alphaaux[i] += this->samples->getPoint(j)->alpha * matrixdif[i][j];
   
               for(this->solution.w[k] = 0, i = 0; i < size; ++i)
                   this->solution.w[k] += alphaaux[i] * this->samples->getPoint(i)->alpha;
           }
   
           return this->solution.w;
       }
       std::vector<double> getDualWeightProdInt(){
           register int i = 0, j = 0, k = 0;
           size_t size = this->samples->getSize(), dim = this->samples->getDim();
           std::vector<double> alphaaux(size);
           dMatrix H(size, std::vector<double>(size));
   
           this->solution.w.resize(dim);
   
           for(k = 0; k < dim; ++k)
           {
               for(i = 0; i < size; ++i)
                   for(j = 0; j < size; ++j)
                       H[i][j] = (*this->samples)[i]->x[k] * (*this->samples)[j]->x[k]
                                 * (*this->samples)[i]->y * (*this->samples)[j]->y;
               if(this->verbose >= 3) std::clog << "\n H matrix without dim generated.\n";
               for(i = 0; i < size; ++i)
                   for(alphaaux[i] = 0, j = 0; j < size; ++j)
                       alphaaux[i] += this->samples->getPoint(j)->alpha * H[i][j];
   
               for(this->solution.w[k] = 0, i = 0; i < size; ++i)
                   this->solution.w[k] += alphaaux[i] * this->samples->getPoint(i)->alpha;
           }
   
           return this->solution.w;
       }
   };
   
   #endif
