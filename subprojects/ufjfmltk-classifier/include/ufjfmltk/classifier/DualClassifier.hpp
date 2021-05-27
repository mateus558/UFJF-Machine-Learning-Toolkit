/*! DualClassifier base class.
   \file DualClassifier.hpp
   \author Mateus Coutinho Marim
*/

#ifndef DUALCLASSIFIER__HPP
#define DUALCLASSIFIER__HPP

#include "ufjfmltk/core/Kernel.hpp"
#include "Classifier.hpp"
#include <vector>

namespace mltk{
    namespace classifier {
        template<typename T>
        class DualClassifier : virtual public Classifier<T> {
            // Associations
            // Attributes
        protected:
            /// Alphas vector.
            std::vector<double> alpha;
            KernelType kernel_type = KernelType::INNER_PRODUCT;
            double kernel_param = 0;
            /// Object for kernel computations.
            Kernel<T> *kernel = nullptr;
        public:

            virtual double evaluate(const Point <T> &p, bool raw_value = false) override {
                double func, bias = this->solution.bias, fk = 0.0, lambda;
                size_t size = this->samples->size(), dim = this->samples->dim(), r;
                auto po = std::make_shared<Point<T> >(p);

                if (p.X().size() != dim) {
                    std::cerr << "The point must have the same dimension of the feature set!" << std::endl;
                    return 0;
                }

                for (func = bias, r = 0; r < size; ++r) {
                    fk = this->kernel->function(po, (*this->samples)[r], dim);
                    func += this->solution.alpha[r] * (*this->samples)[r]->Y() * fk;
                }

                return (func >= 0) ? 1 : -1;
            }

            inline void recomputeKernel(){ if(this->kernel) this->kernel->recompute(); }

            /*********************************************
             *               Setters                     *
             *********************************************/

            /**
             * \brief setKernel Set the kernel used by the dual classifier.
             * \param q Norm that will be used by the classifier.
             */
            inline void setKernel(Kernel<T> *K) { this->kernel = K; }

            /**
             * \brief Set the type of the kernel.
             * \param type The type of the selected kernel.
             */
            inline void setKernelType(KernelType type) {
                this->kernel_type = type;
                if (this->kernel) this->kernel->setType(type);
            }

            /**
             * \brief Set the parameter of the kernel.
             * \param param The parameter of the selected kernel.
             */
            inline void setKernelParam(double param) {
                this->kernel_param = param;
                if (this->kernel) kernel->setParam(param);
            }

            /*********************************************
             *               Getters                     *
             *********************************************/

            virtual std::string getFormulationString() override { return "Dual"; }

            inline Kernel<T> *getKernel() { return kernel; }

            /**
             * \brief Get the parameter of the kernel.
             * \return double
             */
            inline double getKernelParam() { return kernel_param; }

            /**
             * \brief Get the type of the kernel.
             * \return double
             */
            inline KernelType getKernelType() { return kernel_type; }

            /**
             * \brief Get the vector of alphas.
             * \return std::vector<double>
             */
            inline std::vector<double> getAlphaVector() { return alpha; }

            /**
             * \brief getWeight Get the vector of weights when the linear kernel is used.
             * \return std::vector<double>
             */
            std::vector<double> getWeight() {
                size_t i, j, dim = this->samples->dim(), size = this->samples->size();
                std::vector<double> w(dim);

                for (i = 0; i < dim; i++) {
                    for (j = 0; j < size; j++) {
                        w[i] += (*this->samples)[j]->Alpha() * (*this->samples)[j]->Y() * (*this->samples)[j]->X()[i];
                    }
                }

                return w;
            }

            /**
             * \brief Compute the weights of the dual classifier (with H matrix).
             * \return std::vector<double>
             */
            std::vector<double> getDualWeight() {
                int i = 0, j = 0, k = 0;
                size_t size = this->samples->size(), dim = this->samples->dim();
                dMatrix *H, *Hk, matrixdif(size, std::vector<double>(size));
                std::vector<double> alphaaux(size);

                H = kernel->generateMatrixH(this->samples);

                this->solution.w.resize(dim);

                for (k = 0; k < dim; ++k) {
                    Hk = kernel->generateMatrixHwithoutDim(this->samples, k);

                    for (i = 0; i < size; ++i)
                        for (j = 0; j < size; ++j)
                            matrixdif[i][j] = (*H)[i][j] - (*Hk)[i][j];

                    for (i = 0; i < size; ++i)
                        for (alphaaux[i] = 0, j = 0; j < size; ++j)
                            alphaaux[i] += this->samples->point(j)->Alpha() * matrixdif[i][j];

                    for (this->solution.w[k] = 0, i = 0; i < size; ++i)
                        this->solution.w[k] += alphaaux[i] * this->samples->point(i)->Alpha();
                }

                return this->solution.w;
            }

            /**
             * \brief Compute the weights with inner product of the dual classifier.
             * \return std::vector<double>
             */
            std::vector<double> getDualWeightProdInt() {
                int i = 0, j = 0, k = 0;
                size_t size = this->samples->size(), dim = this->samples->dim();
                std::vector<double> alphaaux(size);
                dMatrix H(size, std::vector<double>(size));

                this->solution.w.resize(dim);

                for (k = 0; k < dim; ++k) {
                    for (i = 0; i < size; ++i)
                        for (j = 0; j < size; ++j)
                            H[i][j] = (*this->samples)[i]->X()[k] * (*this->samples)[j]->X()[k]
                                      * (*this->samples)[i]->Y() * (*this->samples)[j]->Y();
                    if (this->verbose >= 3) std::clog << "\n H matrix without dim generated.\n";
                    for (i = 0; i < size; ++i)
                        for (alphaaux[i] = 0, j = 0; j < size; ++j)
                            alphaaux[i] += this->samples->point(j)->Alpha() * H[i][j];

                    for (this->solution.w[k] = 0, i = 0; i < size; ++i)
                        this->solution.w[k] += alphaaux[i] * this->samples->point(i)->Alpha();
                }

                return this->solution.w;
            }
        };
    }
}
#endif
