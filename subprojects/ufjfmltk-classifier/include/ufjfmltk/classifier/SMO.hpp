//
// Created by Mateus Coutinho Marim on 7/28/2018.
//
/*****************************************************
 * SMO classifier lib                                *
 *                                                   *
 * Saul Leite <lsaul@lncc.br>                        *
 * sep 23, 2004                                      *
 *****************************************************/

/*****************************************************
 * SMO classifier lib                                *
 *****************************************************/

#ifndef UFJF_MLTK_SMO_H
#define UFJF_MLTK_SMO_H
#pragma once

#include "DualClassifier.hpp"
#include "int_dll.h"
#include <list>

namespace mltk{
        namespace classifier {

            template<typename T>
            class SMO : public DualClassifier<T> {
            private:
                struct smo_learning_data {
                    double error;
                    char done;
                    int_dll *sv = nullptr;

                    smo_learning_data() {
                        error = 0.0;
                        done = 0;
                        sv = nullptr;
                    }
                };
                double param = 0;
                const int C = 9999; //0.05
                const double TOL = 0.0001;
                std::vector<smo_learning_data> l_data;
                int_dll *head = nullptr;

                bool examine_example(int i1);

                bool max_errors(int i1, double e1);

                bool iterate_non_bound(int i1);

                bool iterate_all_set(int i1);

                int take_step(int i1, int i2);

                double function(int index);

                bool training_routine();

                void test_learning();

                int train_matrix(Kernel<T> *matrix);

            public:
                SMO() = default;
                explicit SMO(const Data<T>& samples, KernelType kernel_type = KernelType::INNER_PRODUCT,
                             double param = 0, int verbose = 0);

                bool train() override;

                ~SMO();
            };

            template<typename T>
            SMO<T>::SMO(const Data<T>& samples, KernelType kernel_type, double param, int verbose) {
                this->samples = mltk::make_data<T>(samples);
                this->verbose = verbose;
                this->kernel_type = kernel_type;
                this->kernel_param = param;
                this->head = nullptr;
            }

            template<typename T>
            SMO<T>::~SMO() {
                if (this->head != nullptr) {
                    int_dll::free(&this->head);
                    this->head = nullptr;
                }
                if(this->kernel) delete this->kernel;
            }

            template<typename T>
            bool SMO<T>::train() {
                size_t i = 0, size = this->samples->size(), dim = this->samples->dim();
                bool ret = true;
                dMatrix matrix;
                double norm = 1;
                std::vector<double> w_saved;
                this->solution = Solution();
                this->head = new int_dll;
                this->l_data.resize(size);

                /*clear data*/
                this->solution.bias = 0;
                for (i = 0; i < size; i++)
                    (*this->samples)[i]->Alpha() = 0;
                this->alpha.assign(size, 0.0);
                if(this->kernel) delete this->kernel;
                this->kernel = new mltk::Kernel<T>(this->kernel_type, this->kernel_param);
                this->kernel->compute(this->samples);

                this->timer.reset();
                /*run training algorithm*/
                ret = training_routine();

                norm = this->kernel->featureSpaceNorm(this->samples);
                if (this->kernel->getType() == 0)
                    w_saved = this->getWeight().X();
                else {
                    if (this->kernel->getType() == 1 && this->kernel->getParam() == 1)
                        w_saved = this->getDualWeightProdInt().X();
                    else
                        w_saved = this->getDualWeight().X();
                }

                this->solution.w = w_saved;
                this->solution.margin = 1.0 / norm;
                this->solution.alpha.assign(size, 0.0);
                this->solution.svs = 0;

                for (i = 0; i < size; ++i) {
                    this->solution.alpha[i] = (*this->samples)[i]->Alpha();
                    if ((*this->samples)[i]->Alpha() > 0) ++this->solution.svs;
                    if ((*this->samples)[i]->Alpha() > this->C) ret = false;
                }

                if (this->verbose) {
                    std::cout << "Number of Support Vectors: " << this->solution.svs << std::endl;
                    std::cout << "Margin found: " << this->solution.margin << "\n\n";

                    if (this->verbose > 1) {
                        std::vector<int> fnames = this->samples->getFeaturesNames();
                        for (i = 0; i < dim; i++)
                            std::cout << "W[" << i << "]: " << this->solution.w[i] << std::endl;
                        std::cout << "Bias: " << this->solution.bias << "\n\n";
                    }
                }

                int_dll::free(&this->head);
                this->l_data.clear();

                return ret;
            }


            template<typename T>
            bool SMO<T>::examine_example(int i1) {
                size_t i = 0, size = this->samples->size();
                double y1 = 0;
                double e1 = 0;
                double r1 = 0;
                double alpha1 = 0;

                /*cleaning up done list*/
                for (i = 0; i < size; ++i) this->l_data[i].done = 0;
                this->l_data[i1].done = 1;

                /*reading stuff from array*/
                auto p = (*this->samples)[i1];
                y1 = p->Y();
                alpha1 = p->Alpha();
                if (alpha1 > 0 && alpha1 < this->C) e1 = this->l_data[i1].error;
                else e1 = function(i1) - y1;

                /*calculating r1*/
                r1 = y1 * e1;

                /*try to find next example by 3 different ways*/
                if ((r1 < -this->TOL && alpha1 < this->C) || (r1 > this->TOL && alpha1 > 0)) {
                    if (max_errors(i1, e1)) return true;
                    else if (iterate_non_bound(i1)) return true;
                    else if (iterate_all_set(i1)) return true;
                } else if (this->verbose > 2) std::cout << "Return0 -1\n";

                return false;
            }

            template<typename T>
            bool SMO<T>::max_errors(int i1, double e1) {
                int k = 0;
                int i2 = -1;
                double tmax = 0;
                double e2 = 0;
                double temp = 0;
                int_dll *list = nullptr;

                if (this->verbose > 2) std::cout << "  Max errors iterations\n";

                /*iterate through the non-bond examples*/
                list = this->head->next;
                while (list != nullptr) {
                    k = list->index;
                    if (this->l_data[k].done == 0 && (*this->samples)[k]->Alpha() < this->C) {
                        e2 = this->l_data[k].error;
                        temp = fabs(e1 - e2);

                        if (temp > tmax) {
                            tmax = temp;
                            i2 = k;
                        }
                    }
                    list = list->next;
                }

                return (i2 >= 0 && take_step(i1, i2));
            }

            template<typename T>
            bool SMO<T>::iterate_non_bound(int i1) {
                int k = 0;
                int_dll *list = nullptr;

                if (this->verbose > 2) printf("  Non-bound iteration\n");

                /* look through all non-bound examples*/
                list = this->head->next;
                while (list != nullptr) {
                    k = list->index;
                    if (this->l_data[k].done == 0 && (*this->samples)[k]->Alpha() < this->C)
                        if (take_step(i1, k)) return true;
                    list = list->next;
                }

                return false;
            }

            template<typename T>
            bool SMO<T>::iterate_all_set(int i1) {
                int k0 = 0;
                int k = 0;
                int i2 = 0;
                size_t size = this->samples->size();

                if (this->verbose > 2) std::cout << "  All-set iteration\n";

                srand(0);
                /*random starting point*/
                //k0 = 0;
                k0 = rand() % size;

                for (k = k0; k < size + k0; ++k) {
                    i2 = k % size;
                    if (this->l_data[i2].done == 0 && take_step(i1, i2))
                        return true;
                }
                return false;
            }

            template<typename T>
            int SMO<T>::take_step(int i1, int i2) {
                int i = 0, y1 = 0, y2 = 0, s = 0; //, size=0;
                double alpha1 = 0, alpha2 = 0, new_alpha1 = 0, new_alpha2 = 0;
                double e1 = 0, e2 = 0, min_val = 0, max_val = 0, eta = 0;
                double max_val_f = 0, min_val_f = 0;
                double bnew = 0, b = 0; //delta_b=0 , b=0;
                double t1 = 0, t2 = 0, error_tot = 0;
                int_dll *itr = nullptr;
                dMatrix *matrix = this->kernel->getKernelMatrixPointer();

                /*this sample is done*/
                this->l_data[i2].done = 1;

                /*get info from sample struct*/
                b = -this->solution.bias;
                y1 = (*this->samples)[i1]->Y();
                y2 = (*this->samples)[i2]->Y();
                //size   = sample->size;
                alpha1 = (*this->samples)[i1]->Alpha();
                alpha2 = (*this->samples)[i2]->Alpha();

                /*get error values for i1*/
                if (alpha1 > 0 && alpha1 < this->C) e1 = this->l_data[i1].error;
                else e1 = function(i1) - y1;

                /*get error values for i2*/
                if (alpha2 > 0 && alpha2 < this->C) e2 = this->l_data[i2].error;
                else e2 = function(i2) - y2;

                /*calculate s*/
                s = y1 * y2;

                /*compute min and max*/
                if (s == -1) {
                    min_val = std::max(0.0, alpha2 - alpha1);
                    max_val = std::min(double(this->C), this->C + alpha2 - alpha1);
                } else {
                    min_val = std::max(0.0, alpha2 + alpha1 - this->C);
                    max_val = std::min(double(this->C), alpha1 + alpha2);
                }
                if (min_val == max_val) {
                    if (this->verbose > 2) std::cout << "return0 2\n";
                    return false;
                }

                /*compute eta*/
                eta = 2.0 * (*matrix)[i1][i2] - (*matrix)[i1][i1] - (*matrix)[i2][i2];

                /*compute new alpha2*/
                if (eta < 0) {
                    new_alpha2 = alpha2 + y2 * (e2 - e1) / eta;

                    if (new_alpha2 < min_val) new_alpha2 = min_val;
                    else if (new_alpha2 > max_val) new_alpha2 = max_val;
                } else {
                    /*computing min and max functions*/
                    double c1 = eta / 2.0;
                    double c2 = y2 * (e1 - e2) - eta * alpha2;
                    min_val_f = c1 * min_val * min_val + c2 * min_val;
                    max_val_f = c1 * max_val * max_val + c2 * min_val;

                    if (min_val_f > max_val_f + this->EPS) new_alpha2 = min_val;
                    else if (min_val_f < max_val_f - this->EPS) new_alpha2 = max_val;
                    else new_alpha2 = alpha2;
                }

                /*exit if no change made*/
                if (fabs(new_alpha2 - alpha2) < this->EPS * (new_alpha2 + alpha2 + this->EPS)) {
                    if (this->verbose > 2)std::cout << "return0 3\n";
                    return false;
                }

                /*calculate new alpha1*/
                new_alpha1 = alpha1 - s * (new_alpha2 - alpha2);
                if (new_alpha1 < 0) {
                    new_alpha2 += s * new_alpha1;
                    new_alpha1 = 0;
                } else if (new_alpha1 > this->C) {
                    new_alpha2 += s * (new_alpha1 - this->C);
                    new_alpha1 = this->C;
                }
                /*saving new alphas*/
                (*this->samples)[i1]->Alpha() = new_alpha1;
                (*this->samples)[i2]->Alpha() = new_alpha2;

                /*saving new stuff into sv list*/
                if (new_alpha1 > 0 && this->l_data[i1].sv == nullptr) {
                    int_dll *list = int_dll::append(this->head);

                    list->index = i1;
                    this->l_data[i1].sv = list;
                } else if (new_alpha1 == 0 && this->l_data[i1].sv != nullptr) {
                    int_dll::remove(&(this->l_data[i1].sv));
                }

                if (new_alpha2 > 0 && this->l_data[i2].sv == nullptr) {
                    int_dll *list = int_dll::append(this->head);
                    list->index = i2;
                    this->l_data[i2].sv = list;
                } else if (new_alpha2 == 0 && this->l_data[i2].sv != nullptr) {
                    int_dll::remove(&(this->l_data[i2].sv));
                }

                /*update bias*/
                t1 = y1 * (new_alpha1 - alpha1);
                t2 = y2 * (new_alpha2 - alpha2);

                if (new_alpha1 > 0 && new_alpha1 < this->C)
                    bnew = b + e1 + t1 * (*matrix)[i1][i1] + t2 * (*matrix)[i1][i2];
                else {
                    if (new_alpha2 > 0 && new_alpha2 < this->C)
                        bnew = b + e2 + t1 * (*matrix)[i1][i2] + t2 * (*matrix)[i2][i2];
                    else {
                        double b1 = 0, b2 = 0;
                        b2 = b + e1 + t1 * (*matrix)[i1][i1] + t2 * (*matrix)[i1][i2];
                        b1 = b + e2 + t1 * (*matrix)[i1][i2] + t2 * (*matrix)[i2][i2];
                        bnew = (b1 + b2) / 2.0;
                    }
                }
                //delta_b = bnew - b;
                b = bnew;
                this->solution.bias = -b;

                /*updating error cache*/
                error_tot = 0;
                itr = this->head->next;
                while (itr != nullptr) {
                    i = itr->index;
                    if ((i != i1 && i != i2) && (*this->samples)[i]->Alpha() < C) {
                        this->l_data[i].error = function(i) - (*this->samples)[i]->Y();
                        error_tot += this->l_data[i].error;
                    }
                    itr = itr->next;
                }

                this->l_data[i1].error = 0.0;
                this->l_data[i2].error = 0.0;

                if (this->verbose > 1)
                    std::cout << "Total error= " << error_tot << ", alpha(" << i1 << ")= " << new_alpha1 << ", alpha(" << i2
                         << ")= " << new_alpha2 << std::endl;

                return 1;
            }

            template<typename T>
            double SMO<T>::function(int index) {
                int i = 0;
                double sum = 0;
                dMatrix *matrix = this->kernel->getKernelMatrixPointer();
                int_dll *list = this->head->next;

                while (list != nullptr) {
                    i = list->index;
                    if ((*this->samples)[i]->Alpha() > 0)
                        sum += (*this->samples)[i]->Alpha() * (*this->samples)[i]->Y() * (*matrix)[i][index];
                    list = list->next;
                }
                sum += this->solution.bias;

                return sum;
            }

            template<typename T>
            bool SMO<T>::training_routine() {
                size_t size = this->samples->size();
                size_t epoch = 0;
                int k = 0;
                int num_changed = 0;
                int tot_changed = 0;
                int examine_all = 1;

                /*initialize variables*/
                this->solution.bias = 0;
                for (k = 0; k < size; ++k) {
                    (*this->samples)[k]->Alpha() = 0;
                    this->l_data[k].error = 0;
                    this->l_data[k].done = 0;
                }

                /*training*/
                while (num_changed > 0 || examine_all) {
                    /*stop if iterated too much!*/
                    if (epoch > this->MAX_EPOCH) return false;

                    num_changed = 0;
                    if (examine_all)
                        for (k = 0; k < size; ++k) {
                            num_changed += examine_example(k);
                        }
                    else
                        for (k = 0; k < size; ++k)
                            if ((*this->samples)[k]->Alpha() > 0 && (*this->samples)[k]->Alpha() < C)
                                num_changed += examine_example(k);

                    if (examine_all == 1) examine_all = 0;
                    else if (num_changed == 0) examine_all = 1;
                    tot_changed += num_changed;
                    ++epoch;
                }

                /*final verbose*/
                if (this->verbose) {
                    test_learning();
                    //printf("Margin = %lf, number of changes %d\n",1.0/norm,tot_changed);
                    //free(w);
                }
                return true;
            }

            template<typename T>
            void SMO<T>::test_learning() {
                size_t i = 0, size = this->samples->size();
                for (i = 0; i < size; ++i)
                    std::cout << i + 1 << " -> " << function(i) << " (error=" << this->l_data[i].error << ") (alpha="
                         << (*this->samples)[i]->Alpha() << ")\n";
            }

            template<typename T>
            int SMO<T>::train_matrix(Kernel<T> *matrix) {
                size_t i = 0, size = this->samples->size();
                bool ret = true;
                double norm = 1;
                std::vector<smo_learning_data> l_data(size);

                //srand(0);

                /*clear data*/
                this->solution.bias = 0;
                for (i = 0; i < size; i++)
                    (*this->samples)[i]->Alpha() = 0;

                /*run training algorithm*/
                ret = training_routine();

                norm = matrix->featureSpaceNorm(this->samples);
                this->solution.margin = 1.0 / norm;

                this->solution.svs = 0;
                for (i = 0; i < size; ++i) {
                    if ((*this->samples)[i]->Alpha() > 0) ++this->solution.svs;
                    if ((*this->samples)[i]->Alpha() > this->C) ret = false;
                }

                int_dll::free(&this->head);

                return ret;
            }
        }
}
#endif //UFJF_MLTK_SMO_H
