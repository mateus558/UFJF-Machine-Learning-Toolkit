/*! Incremental Margin Algorithm implementations
   \file IMA.hpp
   \author Mateus Coutinho Marim
*/

//
// Created by mateus558 on 01/08/17.
//

#ifndef CLASSIFICATION_ALGORITHMS_SYSTEM_IMA_HPP
#define CLASSIFICATION_ALGORITHMS_SYSTEM_IMA_HPP

#include <vector>
#include <float.h>

#include "PrimalClassifier.hpp"
#include "DualClassifier.hpp"
#include "Perceptron.hpp"

namespace mltk{
    namespace classifier {
        /**
         * \brief Wrapper for the implementation of the Incremental Margin Algorithm primal.
         */
        template<typename T = double>
        class IMAp : public PrimalClassifier<T> {
        private:
            double margin;
            /// Support vector.
            std::vector<int> svs;

        public:
            IMAp() = default;

            explicit IMAp(const Data <T>& samples, int q = 2, double flexible = 0.0, double margin = 0.0,
                          Solution* initial_solution = nullptr);

            bool train() override;

            double evaluate(const Point <T> &p, bool raw_value = false) override;

            /**
             * \brief Get the indexes of the support vectors.
             * \return std::vector<int>
             **/
            std::vector<int> getSupportVectors() { return svs; }
        };

        /**
         * \brief Wrapper for the implementation of the Incremental Margin Algorithm primal with fixed margin.
         */
        template<typename T = double>
        class IMApFixedMargin : public PrimalClassifier<T> {
        private:
            int n, maiorn = 0, flagNao1aDim;
            double margin = 0.0, maiorw = 0.0;
            unsigned long tMax;

            /// Support vector.
            std::vector<int> svs;

        public:
            IMApFixedMargin() = default;
            explicit IMApFixedMargin(const mltk::Data<T>& samples, double gamma = 0,
                                     Solution *initial_solution = nullptr);

            bool train() override;

            double evaluate(const Point <T> &p, bool raw_value = false) override;

            inline int *getFlagNot1aDim() { return &flagNao1aDim; }

            inline unsigned long *gettMax() { return &tMax; }
        };

        template<typename T = double>
        class IMADual : public DualClassifier<T> {
        private:
            double margin = 0;
            /// Support vector.
            std::vector<int> svs;
        public:
            IMADual() = default;
            explicit IMADual(KernelType kernel_type, double kernel_param=0,
                             double rate = 1, Solution* initial_solution = nullptr);
            explicit IMADual(const Data<T>& samples, KernelType kernel_type=KernelType::INNER_PRODUCT, double kernel_param=0,
                             double rate = 1, Solution* initial_solution = nullptr);

            bool train() override;
            /**
             * \brief Get the indexes of support vectors.
             * \return std::vector<int>
             **/
            std::vector<int> getSupportVectors() { return svs; }
        };

        template<typename T>
        IMAp<T>::IMAp(const Data<T> &samples, int q, double flexible, double margin, Solution *initial_solution) {
            this->samples = mltk::make_data<T>(samples);
            this->margin = margin;
            this->flexible = flexible;
            this->q = q;

            this->hasInitialSolution = false;

            if (initial_solution) {
                this->solution.w = initial_solution->w;
                this->solution.bias = initial_solution->bias;
                this->hasInitialSolution = true;
            } else {
                if (this->samples) this->w.resize(this->samples->dim());
            }
        }

        template<typename T>
        bool IMAp<T>::train() {
            unsigned int tMax = 0;
            int i, j, n, maiorn = 0, flagNao1aDim = 0, y, it, sign = 1, svs = 0;
            size_t size = this->samples->size(), dim = this->samples->dim(), t1 = 1, t3 = 1;
            double gamma = 0.0, secs, bias = 0.0, alpha, rmargin = margin, inc, stime;
            double min = 0.0, max = 0.0, norm = 1.0, maiorw = 0.0;
            std::vector<double> w_saved, func;
            std::vector<int> index = this->samples->getIndex(), fnames = this->samples->getFeaturesNames();
            auto points = this->samples->points();
            IMApFixedMargin<T> imapFixMargin;
            imapFixMargin.setGamma(gamma);
            Solution tempSol;

            n = dim;
            this->rate = 1.0;
            //Initializing data struct
            this->solution.norm = 1.0;

            //Allocating space for w_saved and func
            w_saved.resize(dim);
            func.resize(size);

            //Allocating space for w
            if (this->hasInitialSolution) {
                if (this->q == 1)
                    for (this->solution.norm = 0.0, i = 0; i < dim; ++i)
                        this->solution.norm += fabs(this->solution.w[i]);
                else if (this->q == 2) {
                    for (this->solution.norm = 0.0, i = 0; i < dim; ++i)
                        this->solution.norm += this->solution.w[i] * this->solution.w[i];
                    this->solution.norm = sqrt(this->solution.norm);
                } else {
                    for (this->solution.norm = 0.0, i = 0; i < dim; ++i)
                        this->solution.norm += std::pow(fabs(this->solution.w[i]), this->q);
                    this->solution.norm = std::pow(this->solution.norm, 1.0 / this->q);
                }

                for (i = 0; i < dim; ++i) this->solution.w[i] /= this->solution.norm;

                this->solution.bias /= this->solution.norm;
                this->solution.norm = 1;
                flagNao1aDim = 1;
                int flag = 0;

                for (min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i) {
                    y = points[i]->Y();
                    for (func[i] = 0, j = 0; j < dim; ++j)
                        func[i] += this->w[j] * points[i]->X()[j];
                    if (y == 1 && func[i] < min) min = func[i];
                    else if (y == -1 && func[i] > max) max = func[i];
                }

                this->solution.bias = -(min + max) / 2.0;

                for (min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i) {
                    y = points[i]->Y();
                    for (func[i] = this->solution.bias, j = 0; j < dim; ++j)
                        func[i] += this->solution.w[j] * points[i]->X()[j];
                    if (func[i] * y < 0) flag++;
                    if (y == 1 && func[i] < min) min = func[i];
                    else if (y == -1 && func[i] > max) max = func[i];
                }

                if (flag) rmargin = 0;
                else rmargin = fabs(min);

                if (margin == 0) tMax = this->MAX_UP;
                else {
                    double raio = mltk::stats::radius(*this->samples, -1, this->q);//data_get_radius(sample, -1, this->q);
                    tMax = (raio * raio - rmargin * rmargin) / std::pow(this->margin - rmargin, 2);
                    if (rmargin == 0) tMax *= 1.5;
                }

                *imapFixMargin.gettMax() = tMax;
            }

            //Allocating space for index and initializing
            if (index.size() == 0) {
                index.resize(size);
                for (i = 0; i < size; ++i) index[i] = i;
            }
            this->samples->setIndex(index);

            //Initializing alpha
            for (i = 0; i < size; ++i) { points[i]->Alpha() = 0.0; }
            if (this->verbose) {
                std::cout << "-----------------------------------------------------------------------------\n";
                std::cout << " pmf    steps     updates              margin              norm          secs\n";
                std::cout << "-----------------------------------------------------------------------------\n";
            }

            it = 0;
            this->ctot = 0;
            this->steps = 0;
            gamma = 0.0;

            imapFixMargin.setSamples(this->samples);
            imapFixMargin.setCtot(this->ctot);
            imapFixMargin.setqNorm(this->q);
            imapFixMargin.setSteps(this->steps);
            imapFixMargin.setGamma(gamma);
            imapFixMargin.setFlexible(this->flexible);
            imapFixMargin.setLearningRate(this->rate);
            if (this->hasInitialSolution) {
                imapFixMargin.setSolution(this->solution);
            }
            imapFixMargin.setMaxUpdates(this->MAX_UP);
            imapFixMargin.setMaxIterations(this->MAX_IT);
            imapFixMargin.setMaxTime(this->max_time);
            this->timer.reset();
            stime = this->timer.elapsed();
            imapFixMargin.setStartTime(stime);
            *imapFixMargin.getFlagNot1aDim() = flagNao1aDim;
            while (imapFixMargin.train()) {
                stime += this->timer.elapsed();

                this->ctot = imapFixMargin.getCtot();
                this->steps = imapFixMargin.getSteps();
                //Finding minimum and maximum functional values
                tempSol = imapFixMargin.getSolution();
                norm = tempSol.norm;
                bias = tempSol.bias;
                func = tempSol.func.X();

                for (min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i) {
                    y = points[i]->Y();
                    alpha = points[i]->Alpha();
                    if ((func[i] + y * alpha * this->flexible) >= 0 &&
                        min > (func[i] + y * alpha * this->flexible) / norm) {
                        min = (func[i] + y * alpha * this->flexible) / norm;
                    }
                    else if ((func[i] + y * alpha * this->flexible) < 0 &&
                             max < (func[i] + y * alpha * this->flexible) / norm)
                        max = (func[i] + y * alpha * this->flexible) / norm;
                }
                //Saving good weights
                for (i = 0; i < dim; i++) w_saved[i] = tempSol.w[i];

                //Obtaining real margin
                rmargin = (fabs(min) > fabs(max)) ? fabs(max) : fabs(min);

                //Shift no bias
                double mmargin = (fabs(max) + fabs(min)) / 2.0;
                if (fabs(max) > fabs(min))
                    tempSol.bias += fabs(mmargin - rmargin);
                else
                    tempSol.bias -= fabs(mmargin - rmargin);

                //Obtaining new gamma_f
                gamma = (min - max) / 2.0;
                inc = (1 + this->alpha_aprox) * rmargin;
                if (gamma < inc) gamma = inc;
                rmargin = mmargin;

                if (it == 2)
                    t1 = this->ctot;
                t3 = this->ctot;

                if (it > 1) {
                    this->rate = sqrt(t1) / sqrt(t3);
                    if (this->verbose) std::cout << "RATE: " << this->rate << "\n";
                } else if (it == 1 && this->verbose)
                    std::cout << "RATE: " << this->rate << "\n";

                secs = stime / 1000;
                if (this->verbose)
                    std::cout << " " << it + 1 << "        " << this->steps << "           " << this->ctot
                         << "              " << rmargin << "            " << norm << "           " << secs << " ";

                ++it; //IMA iteration increment

                imapFixMargin.setGamma(gamma);
                imapFixMargin.setSolution(tempSol);
                imapFixMargin.setLearningRate(this->rate);
                this->svs.clear();
                for (i = 0; i < size; ++i) {
                    y = points[i]->Y();
                    alpha = points[i]->Alpha();
                    if (alpha > this->EPS * this->rate) { this->svs.push_back(i); }
                }
                this->steps = imapFixMargin.getSteps();
                this->ctot = imapFixMargin.getCtot();
                this->solution.w = w_saved;
                this->solution.margin = rmargin;
                this->solution.norm = norm;
                this->solution.bias = bias;
                this->solution.svs = this->svs.size();
                //  break;
                if(it > this->MAX_IT) break;
                if(stime >= this->max_time) break;
                if (flagNao1aDim) break;
            }
             this->svs.erase(this->svs.begin(), this->svs.end());
             for(i = 0; i < size; ++i)
             {
                 y = points[i]->Y();
                 alpha = points[i]->Alpha();
                 if(alpha > this->EPS * this->rate) { this->svs.push_back(i); }
             }

             this->steps = imapFixMargin.getSteps();
             this->ctot = imapFixMargin.getCtot();
             this->solution.w.clear();
             this->solution.w = w_saved;
             this->solution.margin = rmargin;
             this->solution.norm = norm;
             this->solution.bias = bias;
             this->solution.svs = this->svs.size();

            if (this->verbose) {
                std::cout << "\n-----------------------------------------------------------------------------\n";
                std::cout << "Number of times that the Fixed Margin Perceptron was called: " << it + 1 << "\n";
                std::cout << "Number of steps through data: " << this->steps << "\n";
                std::cout << "Number of updates: " << this->ctot << "\n";
                std::cout << "Margin found: " << rmargin << "\n";
                std::cout << "Min: " << fabs(min) << " / Max: " << fabs(max) << "\n";
                std::cout << "Number of Support Vectors: " << this->svs.size() << "\n\n";
                if (this->verbose >= 2) {
                    for (i = 0; i < dim; ++i) std::cout << "W[" << i << "]: " << w_saved[i] << "\n";
                    std::cout << "Bias: " << this->solution.bias << "\n\n";
                }
            }

            this->samples->resetIndex();

            if (!it) {
                if (this->verbose) std::cout << "FMP convergency wasn't reached!\n";
                return false;
            }
            return true;
        }

        template<typename T>
        double IMAp<T>::evaluate(const Point<T> &p, bool raw_value) {
            double func = 0.0;
            int i;
            size_t dim = this->solution.w.size();

            // if(p.X().size() != dim){
            //     cerr << "The point must have the same dimension of the feature set! (" << p.X().size() << "," << dim << ")" << endl;
            //     return 0;
            // }

            for (func = this->solution.bias, i = 0; i < dim; i++) {
                func += this->solution.w[i] * p[i];
            }
            if (!raw_value) return (func >= this->solution.margin * this->solution.norm) ? 1 : -1;
            else return func;
        }



        template<typename T>
        IMApFixedMargin<T>::IMApFixedMargin(const mltk::Data<T>& samples, double gamma,
                                            Solution *initial_solution) {
            this->gamma = gamma;
            this->samples = mltk::make_data<T>(samples);

            if (initial_solution) {
                this->w = initial_solution->w.X();
                this->solution.bias = initial_solution->bias;
                this->solution.norm = initial_solution->norm;
            } else {
                this->w.resize(samples.dim());
            }
        }

        template<typename T>
        bool IMApFixedMargin<T>::train() {
            int c, e = 1, i, k, s = 0, j;
            int t, idx, r;
            size_t size = this->samples->size(), dim = this->samples->dim();
            double norm = this->solution.norm, bias = this->solution.bias, lambda = 1, y, time =
                    this->max_time + this->start_time;
            double sumnorm = 0; //soma das normas para o calculo posterior (nao mais sqrt)
            double maiorw_temp = 0;
            int n_temp, sign = 1;
            bool cond;
            std::vector<double> func(size, 0.0);
            std::vector<int> index = this->samples->getIndex();
            std::vector<T> x;
            this->timer.reset();
            if (!this->solution.w.empty())
                this->w = this->solution.w.X();
            else this->w.resize(this->samples->dim(), 0.0);

            while (this->timer.elapsed() - time <= 0) {
                for (e = 0, i = 0; i < size; ++i) {
                    //shuffling data r = i + rand()%(size-i); j = index[i]; idx = index[i] = index[r]; index[r] = j;
                    idx = index[i];
                    //cout << idx << endl;
                    x = (*this->samples)[idx]->X();
                    y = (*this->samples)[idx]->Y();
                    //if(i == 100) return 1;
                    //calculating function
                    for (func[idx] = bias, j = 0; j < dim; ++j) {
                        func[idx] += this->w[j] * x[j];
                    }
                    //cout << "funcidx: " << y*func[idx] << " marg: " << this->gamma*norm - points[idx]->Alpha()*this->flexible <<"\n ";
                    //Checking if the point is a mistake
                    if (y * func[idx] <= this->gamma * norm - (*this->samples)[idx]->Alpha() * this->flexible) {
                        lambda = (norm) ? (1 - this->rate * this->gamma / norm) : 1;
                        for (r = 0; r < size; ++r)
                            (*this->samples)[r]->Alpha() *= lambda;

                        if (this->q == 1.0) //Linf
                        {
                            for (sumnorm = 0, j = 0; j < dim; ++j) {
                                sign = 1;
                                if (this->w[j] < 0) sign = -1;
                                lambda = (norm > 0 && this->w[j] != 0) ? this->gamma * sign : 0;
                                this->w[j] += this->rate * (y * x[j] - lambda);
                                sumnorm += fabs(this->w[j]);
                            }
                            norm = sumnorm;
                        } else if (this->q == 2.0) //L2
                        {
                            for (sumnorm = 0, j = 0; j < dim; ++j) {
                                lambda = (norm > 0 && this->w[j] != 0) ? this->w[j] * this->gamma / norm : 0;
                                this->w[j] += this->rate * (y * x[j] - lambda);
                                sumnorm += this->w[j] * this->w[j];
                            }
                            norm = sqrt(sumnorm);
                        } else if (this->q == -1.0) //L1
                        {
                            maiorw_temp = fabs(this->w[0]);
                            n_temp = 1;
                            for (j = 0; j < dim; ++j) {
                                if (this->maiorw == 0 ||
                                    fabs(this->maiorw - fabs(this->w[j])) / this->maiorw < this->EPS) {
                                    sign = 1;
                                    if (this->w[j] < 0) sign = -1;
                                    lambda = (norm > 0 && this->w[j] != 0) ? this->gamma * sign / this->n : 0;
                                    this->w[j] += this->rate * (y * x[j] - lambda);
                                } else
                                    this->w[j] += this->rate * (y * x[j]);

                                if (j > 0) {
                                    if (fabs(maiorw_temp - fabs(this->w[j])) / maiorw_temp < this->EPS)
                                        n_temp++;
                                    else if (fabs(this->w[j]) > maiorw_temp) {
                                        maiorw_temp = fabs(this->w[j]);
                                        n_temp = 1;
                                    }
                                }
                            }
                            this->maiorw = maiorw_temp;
                            this->n = n_temp;
                            norm = this->maiorw;
                            if (this->n > this->maiorn) this->maiorn = this->n;
                        } else //outras formula��es - Lp
                        {
                            for (sumnorm = 0, j = 0; j < dim; ++j) {
                                lambda = (norm > 0 && this->w[j] != 0) ? this->w[j] * this->gamma *
                                                                         std::pow(fabs(this->w[j]), this->q - 2.0) *
                                                                         std::pow(norm, 1.0 - this->q) : 0;
                                this->w[j] += this->rate * (y * x[j] - lambda);
                                sumnorm += std::pow(fabs(this->w[j]), this->q);
                            }
                            norm = std::pow(sumnorm, 1.0 / this->q);
                        }
                        bias += this->rate * y;
                        (*this->samples)[idx]->Alpha() += this->rate;

                        k = (i > s) ? s++ : e;
                        j = index[k];
                        index[k] = idx;
                        index[i] = j;
                        this->ctot++;
                        e++;
                    } else if (this->steps > 0 && e > 1 && i > s) break;
                }
                this->steps++; //Number of iterations update
                //cout << e << endl;
                //stop criterion
                if (e == 0) break;
                if (this->steps > this->MAX_IT) break;
                if (this->ctot > this->MAX_UP) break;
                if (this->flagNao1aDim) if (this->ctot > tMax) break;
            }

            this->samples->setIndex(index);
            this->solution.norm = norm;
            this->solution.bias = bias;
            this->solution.w = this->w;
            this->solution.func = func;

            if (e == 0) return 1;
            else return 0;
        }

        template<typename T>
        double IMApFixedMargin<T>::evaluate(const Point<T> &p, bool raw_value) {
            double func = 0.0;
            int i;
            size_t dim = this->solution.w.size();

            if (p.X().size() != dim) {
                std::cerr << "The point must have the same dimension of the feature set!" << std::endl;
                return 0;
            }

            for (func = this->solution.bias, i = 0; i < dim; i++) {
                func += this->solution.w[i] * p[i];
            }

            if (!raw_value) return (func >= this->solution.margin * this->solution.norm) ? 1 : -1;
            else return func;
        }

        template<typename T>
        IMADual<T>::IMADual(KernelType kernel_type, double kernel_param, double rate, Solution *initial_solution) {
            this->samples = nullptr;
            this->kernel = new Kernel<T>(kernel_type, kernel_param);
            this->rate = rate;

            if (initial_solution) {
                this->solution.w = initial_solution->w;
                this->solution.bias = initial_solution->bias;
                this->hasInitialSolution = true;
            }
        }

        template<typename T>
        IMADual<T>::IMADual(const Data<T> &samples, KernelType kernel_type, double kernel_param, double rate,
                            Solution *initial_solution) {
            this->samples = make_data<T>(samples);
            this->kernel = new Kernel<T>(kernel_type, kernel_param);
            this->rate = rate;

            if (initial_solution) {
                this->solution.w = initial_solution->w;
                this->solution.bias = initial_solution->bias;
                this->hasInitialSolution = true;
            } else {
                this->solution.w.resize(this->samples->dim());
            }
        }

        template<typename T>
        bool IMADual<T>::train() {
            double rmargin = 0, old_rmargin = 0,secs;
            size_t i, j, it;
            size_t sv = 0, size = this->samples->size(), dim = this->samples->dim();
            double min, max, norm = 0, stime = 0;
            dMatrix K;
            std::vector<int> index = this->samples->getIndex();
            std::vector<double> w_saved(dim), saved_alphas(size), func(size);
            std::vector<std::shared_ptr<Point<T> > > points = this->samples->points();

            if(!this->samples){
                return false;
            }
            this->kernel->compute(this->samples);
            this->timer.reset();

            //Allocating space for index
            if (index.size() == 0) {
                index.resize(size);

                //Initializing alpha and bias
                for (i = 0; i < size; ++i) { index[i] = i; }
            }
            this->solution.bias = 0;
            this->solution.w.resize(this->samples->dim());

            if (this->verbose) {
                std::cout << "-------------------------------------------------------------------\n";
                std::cout << "  steps      updates        margin          norm       svs     secs\n";
                std::cout << "-------------------------------------------------------------------\n";
            }

            it = 0;
            this->ctot = 0;
            this->steps = 0;
            this->gamma = 0;

            PerceptronFixedMarginDual<T> percDual;
            Solution sol, *solr;

            percDual.setSamples(this->samples);
            percDual.setKernel(this->kernel);
            percDual.setGamma(this->gamma);
            percDual.setLearningRate(this->rate);
            percDual.setMaxTime(this->max_time);
            percDual.setMaxUpdates(this->MAX_UP);
            percDual.setMaxIterations(this->MAX_IT);

            stime = this->timer.elapsed();

            while (percDual.train()) {
                stime += percDual.getElapsedTime();

                //Finding minimum and maximum functional values
                this->ctot = percDual.getCtot();
                this->steps = percDual.getSteps();
                solr = percDual.getSolutionRef();
                norm = solr->norm;
                this->solution.bias = solr->bias;

                for (sv = 0, min = DBL_MAX, max = -DBL_MAX, i = 0; i < size; ++i) {
                    if (points[i]->Alpha() > this->EPS * this->rate) {
                        sv++;
                        saved_alphas[i] = points[i]->Alpha();
                    }
                    else { saved_alphas[i] = 0.0; }
                    if (solr->func[i] >= 0 && min > solr->func[i] / norm) min = solr->func[i] / norm;
                    else if (solr->func[i] < 0 && max < solr->func[i] / norm) max = solr->func[i] / norm;
                }

                //Obtaining real margin
                rmargin = (fabs(min) > fabs(max)) ? fabs(max) : fabs(min);

                //Obtaining new gamma_f
                this->gamma = (min - max) / 2.0;
                if (this->gamma < this->MIN_INC * rmargin) this->gamma = this->MIN_INC * rmargin;

                percDual.setGamma(this->gamma);
                secs = stime / 1000;

                if (this->verbose)
                    std::cout << "    " << this->steps << "         " << this->ctot << "          " << rmargin << "         "
                         << norm << "     " << sv << "     " << secs << std::endl;
                ++it; //IMA iteration increment
                if((it > 0) && (rmargin - old_rmargin) < this->EPS){
                    break;
                }
                old_rmargin = rmargin;
            }
            this->ctot = percDual.getCtot();
            this->steps = percDual.getSteps();
            sol = percDual.getSolution();
            this->alpha = percDual.getAlphaVector();
            norm = sol.norm;
            this->solution.bias = sol.bias;
            func = sol.func.X();

            for (i = 0; i < size; ++i) {
                if (points[i]->Alpha() > this->EPS * this->rate) { this->svs.push_back(i); }
            }

            for (i = 0; i < size; ++i) points[i]->Alpha() = saved_alphas[i];

            this->solution.norm = this->kernel->norm(*this->samples);

            /*recuperando o vetor DJ -- "pesos" das componentes*/
            int kernel_type = this->kernel->getType();
            double kernel_param = this->kernel->getParam();

            if (kernel_type == 0)
                for (i = 0; i < dim; i++) {
                    for (j = 0; j < size; j++) {
                        w_saved[i] += points[j]->Alpha() * points[j]->Y() * points[j]->X()[i];
                    }
                }
            else {
                if (kernel_type == 1 && kernel_param == 1)
                    w_saved = DualClassifier<T>::getDualWeightProdInt().X();
                else
                    w_saved = DualClassifier<T>::getDualWeight().X();
                if (it) {
                    this->solution.w = mltk::normalize(this->solution.w, 2.0).X();
                }
            }

            this->solution.w = w_saved;
            this->solution.margin = rmargin;
            this->solution.alpha.assign(size, 0.0);
            for(i = 0; i < size; i++){
                this->solution.alpha[i] = points[i]->Alpha();
            }

            if (this->verbose) {
                std::cout << "-------------------------------------------------------------------\n";
                std::cout << "Number of times that the Fixed Margin Perceptron was called:: " << it + 1 << std::endl;
                std::cout << "Number of steps through data: " << this->steps << std::endl;
                std::cout << "Number of updates: " << this->ctot << std::endl;
                std::cout << "Number of support vectors: " << sv << std::endl;
                std::cout << "Margin found: " << rmargin << "\n\n";
                if (this->verbose > 2) {
                    std::vector<int> fnames = this->samples->getFeaturesNames();
                    for (i = 0; i < dim; i++)
                        std::cout << "W[" << i << "]: " << this->solution.w[i] << std::endl;
                    std::cout << "Bias: " << this->solution.bias << "\n\n";
                }
            }

            if (!it) {
                if (this->verbose) std::cout << "FMP convergency wasn't reached!\n";
                return false;
            }
            return true;
        }
    }
}

#endif //CLASSIFICATION_ALGORITHMS_SYSTEM_IMAP_HPP
