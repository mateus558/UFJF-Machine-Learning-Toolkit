#include <iostream>
#include <ctime>
#include <cmath>

#include "ufjfmltk/classifier/Perceptron.hpp"

namespace mltk{
    namespace classifier {
        using namespace std;

        template<typename T>
        PerceptronPrimal<T>::PerceptronPrimal(std::shared_ptr<Data<T> > samples, double q, double rate,
                                              Solution *initial_solution) {
            this->samples = samples;
            this->q = q;
            this->rate = rate;
            if (initial_solution)
                this->solution = *initial_solution;
        }

        template<typename T>
        PerceptronPrimal<T>::PerceptronPrimal(const Data<T> &samples, double q, double rate,
                                              Solution *initial_solution) {
            this->samples = mltk::make_data<T>(samples);
            this->q = q;
            this->rate = rate;
            if (initial_solution)
                this->solution = *initial_solution;
        }


        template<typename T>
        bool PerceptronPrimal<T>::train() {
            size_t size = this->samples->size(), dim = this->samples->dim();
            int i, j, e = 0, idx;
            double y, time = this->start_time + this->max_time;
            bool cond;
            vector<double> func(size, 0);
            vector<T> x;
            vector<int> index = this->samples->getIndex();

            if (this->solution.w.empty()) this->solution.w.resize(dim);
            this->solution.bias = 0;
            this->solution.norm = 0.0;

            this->timer.reset();
            while (this->timer.elapsed() - time <= 0) {
                for (e = 0, i = 0; i < size; ++i) {
                    idx = index[i];
                    auto p = this->samples->point(idx);
                    x = p->X();
                    y = p->Y();

                    //calculating function
                    for (func[idx] = this->solution.bias, j = 0; j < dim; ++j)
                        func[idx] += this->solution.w[j] * x[j];

                    //Checking if the point is a mistake
                    if (y * func[idx] <= 0.0) {
                        for (this->solution.norm = 0.0, j = 0; j < dim; ++j) {
                            this->solution.w[j] += this->rate * y * x[j];
                            this->solution.norm += this->solution.w[j] * this->solution.w[j];
                        }
                        this->solution.norm = sqrt(this->solution.norm);
                        this->solution.bias += this->rate * y;
                        this->ctot++;
                        e++;
                    } else if (this->steps > 0 && e > 1) break;
                }
                this->steps++;    //Number of iterations update

                //stop criterion
                if (e == 0) break;
                if (this->steps > this->MAX_IT) break;
                if (this->ctot > this->MAX_UP) break;
            }

            return (e == 0);
        }

        template<typename T>
        double PerceptronPrimal<T>::evaluate(const Point<T> &p, bool raw_value) {
            return PrimalClassifier<T>::evaluate(p, raw_value);
        }

        template<typename T>
        PerceptronFixedMarginPrimal<T>::PerceptronFixedMarginPrimal(const mltk::Data<T>& samples, double gamma,
                                                                    double q, double rate, Solution *initial_solution) {
            this->samples = mltk::make_data<T>(samples);
            this->q = q;
            this->rate = rate;
            this->gamma = gamma;
            if (initial_solution)
                this->solution = *initial_solution;
        }

        template<typename T>
        bool PerceptronFixedMarginPrimal<T>::train() {
            size_t i, j, k, e, s, r, dim = this->samples->dim();
            size_t size = this->samples->size(), n = dim;
            int idx, sign = 1, n_temp = 0, largn = 0;
            double norm = this->solution.norm, lambda = 1.0, y, time = this->start_time + this->max_time;
            double sumnorm = 0.0, bias = this->solution.bias, largw = 0.0, largw_temp = 0.0;
            bool cond;
            vector<double> func = this->solution.func, w = this->solution.w;
            vector<T> x;
            vector<int> index = this->samples->getIndex();
            shared_ptr<Point<T> > p;

            if (func.empty()) func.resize(size);
            if (w.empty()) w.resize(dim);
            e = s = 0;
            this->timer.reset();
            while (this->timer.elapsed() - time <= 0) {
                for (e = 0, i = 0; i < size; ++i) {
                    idx = index[i];
                    p = this->samples->point(idx);
                    x = p->X();
                    y = p->Y();

                    //calculating function
                    for (func[idx] = bias, j = 0; j < dim; ++j) {
                        func[idx] += w[j] * x[j];
                    }

                    //Checking if the point is a mistake
                    if (y * func[idx] <= this->gamma * norm - this->samples->point(idx)->Alpha() * this->flexible) {
                        lambda = (norm != 0.0) ? (1 - this->rate * this->gamma / norm) : 1;
                        for (r = 0; r < size; ++r) {
                            shared_ptr<Point<T> > b = this->samples->point(r);
                            b->Alpha() *= lambda;
                            this->samples->setPoint(r, b);
                        }

                        if (this->q == 1.0) { //Linf
                            for (sumnorm = 0.0, j = 0; j < dim; ++j) {
                                sign = (w[j] < 0) ? -1 : 1;
                                lambda = (norm > 0 && w[j] != 0) ? this->gamma * sign : 0;
                                w[j] += this->rate * (y * x[j] - lambda);
                                sumnorm += fabs(w[j]);
                            }
                            norm = sumnorm;
                        } else if (this->q == 2.0) { //L2
                            for (sumnorm = 0.0, j = 0; j < dim; ++j) {
                                lambda = (norm > 0 && w[j] != 0) ? w[j] * this->gamma / norm : 0;
                                w[j] += this->rate * (y * x[j] - lambda);
                                sumnorm += w[j] * w[j];
                            }
                            norm = sqrt(sumnorm);
                        } else if (this->q == -1.0) { //L1
                            largw_temp = fabs(w[0]);
                            n_temp = 1;
                            for (j = 0; j < dim; ++j) {
                                if (largw == 0 || fabs(largw - fabs(w[j])) / largw < this->EPS) {
                                    sign = (w[j] < 0) ? -1 : 1;
                                    lambda = (norm > 0 && w[j] != 0) ? this->gamma * sign / n : 0;
                                    w[j] += this->rate * (y * x[j] - lambda);
                                } else
                                    w[j] += this->rate * (y * x[j]);

                                if (j > 0) {
                                    if (fabs(largw_temp - fabs(w[j])) / largw_temp < this->EPS)
                                        n_temp++;
                                    else if (fabs(w[j]) > largw_temp) {
                                        largw_temp = fabs(w[j]);
                                        n_temp = 1;
                                    }
                                }
                            }
                            largw = largw_temp;
                            n = n_temp;
                            norm = largw;
                            if (n > largn) largn = n;
                        } else { //Other Lp formulations
                            for (sumnorm = 0, j = 0; j < dim; ++j) {
                                lambda = (norm > 0 && w[j] != 0) ? w[j] * this->gamma *
                                                                   std::pow(fabs(w[j]), this->q - 2.0) *
                                                                   std::pow(norm, 1.0 - this->q) : 0;
                                w[j] += this->rate * (y * x[j] - lambda);
                                sumnorm += std::pow(fabs(w[j]), this->q);
                            }
                            norm = std::pow(sumnorm, 1.0 / this->q);
                        }
                        bias += this->rate * y;
                        p->Alpha() += this->rate;
                        this->samples->setPoint(idx, p);

                        k = (i > s) ? s++ : e;
                        j = index[k];
                        index[k] = idx;
                        index[i] = j;
                        this->ctot++;
                        e++;
                    } else if (this->steps > 0 && e > 1 && i > s) break;
                }
                ++this->steps; //Number of iterations update

                //stop criterion
                if (e == 0) break;
                if (this->steps > this->MAX_IT) break;
                if (this->ctot > this->MAX_UP) break;
            }

            this->solution.norm = norm;
            this->solution.bias = bias;
            this->solution.w = w;
            return (e == 0);
        }

        template<typename T>
        double PerceptronFixedMarginPrimal<T>::evaluate(const Point<T> &p, bool raw_value) {
            double func = 0.0;
            int i;
            size_t dim = this->samples->dim();

            if (p.X().size() != dim) {
                cerr << "The point must have the same dimension of the feature set!" << endl;
                return 0;
            }

            for (func = this->solution.bias, i = 0; i < dim; i++) {
                func += this->solution.w[i] * p[i];
            }

            if (!raw_value) return (func >= this->solution.margin * this->solution.norm) ? 1 : -1;
            else return func;
        }

        template<typename T>
        PerceptronDual<T>::PerceptronDual(const Data<T>& samples, KernelType kernel_type,
                                          double kernel_param, double rate, Solution *initial_solution) {
            this->samples = mltk::make_data<T>(samples);
            if (initial_solution) {
                this->solution = *initial_solution;
                this->alpha = (*initial_solution).alpha;
            }
            this->rate = rate;
            this->kernel = new Kernel(kernel_type, kernel_param);
        }

        template<typename T>
        PerceptronDual<T>::PerceptronDual(const Data<T>& samples, double rate, Solution *initial_solution) {
            this->samples = mltk::make_data<T>(samples);
            if (initial_solution) {
                this->solution = *initial_solution;
                this->alpha = (*initial_solution).alpha;
            }
            this->initial = initial_solution;
            this->rate = rate;
            this->kernel = nullptr;
        }


        template<typename T>
        bool PerceptronDual<T>::train() {
            size_t e, i, j, idx, r, size = this->samples->size(), dim = this->samples->dim();
            double norm = this->solution.norm, time = this->start_time + this->max_time;
            double bias = this->solution.bias, f;
            const double sqrate = this->rate * this->rate;
            const double tworate = 2 * this->rate;
            vector<int> index(size); //= this->samples->getIndex();
            vector<double> func(size, 0.0), Kv;
            vector<shared_ptr<Point<T> > > points = this->samples->points();
            if(!this->kernel) this->kernel = new Kernel(mltk::KernelType::INNER_PRODUCT);
            this->kernel->compute(this->samples);
            dMatrix *K = this->kernel->getKernelMatrixPointer();
            int y;
            iota(index.begin(), index.end(), 0);
            if (this->alpha.empty()) {
                this->alpha.assign(size, 0.0);
            }
            if(!initial){
                this->solution = Solution();
            }
            this->timer.reset();
            e = 1;
            auto time_elapsed = this->timer.elapsed() - time;
            while (time_elapsed <= 0) {
                for (e = 0, i = 0; i < size; ++i) {
                    idx = index[i];
                    y = points[idx]->Y();

                    //Calculating function
                    for (f = bias, r = 0; r < size; ++r)
                        f += this->alpha[r] * points[index[r]]->Y() * (*K)[idx][index[r]];
                    func[idx] = f;

                    //Checking if the point is a mistake
                    if (y * f <= 0.0) {
                        norm = std::sqrt(norm * norm + tworate * points[idx]->Y() * (func[idx] - bias) + sqrate * (*K)[idx][idx]);
                        this->alpha[i] += this->rate;
                        bias += this->rate * y;
                        ++this->ctot, ++e;
                    } else if (this->steps > 0 && e > 1) break;
                }
                time_elapsed = this->timer.elapsed() - time;
                ++this->steps;

                //stop criterion
                if (e == 0) break;
                if (this->steps > this->MAX_IT) break;
                if (this->ctot > this->MAX_UP) break;
            }
            this->solution.bias = bias;
            this->solution.norm = norm;
            this->solution.alpha = this->alpha;
            this->solution.margin = 0.0;
            this->solution.w.resize(dim);

            for (i = 0; i < size; i++) {
                for (j = 0; j < dim; j++) {
                    this->solution.w[j] += this->alpha[i] * points[i]->Y() * points[i]->X()[j];
                }
            }

            return (e == 0);
        }

        template<typename T>
        PerceptronFixedMarginDual<T>::PerceptronFixedMarginDual(const mltk::Data<T>& samples, KernelType kernel_type,
                                                                double kernel_param, double gamma, double rate,
                                                                Solution *initial_solution) {
            this->samples = mltk::make_data<T>(samples);
            //this->solution = *initial_solution;
            this->rate = rate;
            this->kernel = new Kernel(kernel_type, kernel_param);
            this->gamma = gamma;
            if (initial_solution)
                this->alpha = (*initial_solution).alpha;
        }

        template<typename T>
        bool PerceptronFixedMarginDual<T>::train() {
            size_t e, i, j, k, s, idx, r, size = this->samples->size(), dim = this->samples->dim();
            double y, lambda, norm = this->solution.norm, time = this->start_time + this->max_time;
            double bias = this->solution.bias;
            const double sqrate = this->rate * this->rate;
            const double tworate = 2 * this->rate;
            vector<int> index = this->samples->getIndex();
            vector<double> func = this->solution.func, Kv;
            this->kernel->compute(this->samples);
            dMatrix *K = this->kernel->getKernelMatrixPointer();

            if(this->alpha.empty()){
                this->alpha.resize(this->samples->size());
            }

            if (func.empty()) { func.resize(size); }
            e = 1, s = 0;
            this->timer.reset();

            while (this->timer.elapsed() - time <= 0) {
                for (e = 0, i = 0; i < size; ++i) {
                    idx = index[i];
                    y = (*this->samples)[idx]->Y();

                    //Checking if the point is a mistake

                    if (y * func[idx] - this->gamma * norm <= 0) {
                        lambda = (this->gamma) ? (1 - this->rate * this->gamma / norm) : 1;
                        norm *= lambda;

                        for (r = 0; r < size; ++r) {
                            (*this->samples)[r]->Alpha() *= lambda;
                            func[r] = lambda * func[r] + this->rate * y * ((*K)[idx][r] + 1) + bias * (1 - lambda);
                        }

                        norm = sqrt(norm * norm + tworate * (*this->samples)[idx]->Y() * lambda * (func[idx] - bias) +
                                    sqrate * (*K)[idx][idx]);
                        (*this->samples)[idx]->Alpha() += this->rate;

                        bias += this->rate * y;

                        k = (i > s) ? ++s : e;
                        j = index[k];
                        index[k] = idx;
                        index[i] = j;
                        ++this->ctot;
                        ++e;
                    } else if (this->steps > 0 && e > 1 && i > s) break;
                }

                ++this->steps; //Number of iterations update
                //stop criterion
                if (e == 0) break;
                if (this->steps > this->MAX_IT) break;
                if (this->ctot > this->MAX_UP) break;
            }

            this->samples->setIndex(index);
            this->solution.bias = bias;
            this->solution.norm = norm;
            this->solution.func = func;
            this->solution.w.resize(dim);
            this->solution.alpha.resize(size);
            for (i = 0; i < dim; i++) {
                for (j = 0; j < size; j++) {
                    this->solution.alpha[j] = (*this->samples)[j]->Alpha();
                    this->solution.w[i] +=
                            (*this->samples)[j]->Alpha() * (*this->samples)[j]->Y() * (*this->samples)[j]->X()[i];
                }
            }

            return (e == 0);
        }


        template
        class PerceptronPrimal<int>;

        template
        class PerceptronPrimal<double>;

        template
        class PerceptronPrimal<float>;

        template
        class PerceptronPrimal<int8_t>;

        template
        class PerceptronPrimal<char>;

        template
        class PerceptronPrimal<long long int>;

        template
        class PerceptronPrimal<short int>;

        template
        class PerceptronPrimal<long double>;

        template
        class PerceptronPrimal<unsigned char>;

        template
        class PerceptronPrimal<unsigned int>;

        template
        class PerceptronPrimal<unsigned short int>;

        template
        class PerceptronFixedMarginPrimal<int>;

        template
        class PerceptronFixedMarginPrimal<double>;

        template
        class PerceptronFixedMarginPrimal<float>;

        template
        class PerceptronFixedMarginPrimal<int8_t>;

        template
        class PerceptronFixedMarginPrimal<char>;

        template
        class PerceptronFixedMarginPrimal<long long int>;

        template
        class PerceptronFixedMarginPrimal<short int>;

        template
        class PerceptronFixedMarginPrimal<long double>;

        template
        class PerceptronFixedMarginPrimal<unsigned char>;

        template
        class PerceptronFixedMarginPrimal<unsigned int>;

        template
        class PerceptronFixedMarginPrimal<unsigned short int>;

        template
        class PerceptronDual<int>;

        template
        class PerceptronDual<double>;

        template
        class PerceptronDual<float>;

        template
        class PerceptronDual<int8_t>;

        template
        class PerceptronDual<char>;

        template
        class PerceptronDual<long long int>;

        template
        class PerceptronDual<short int>;

        template
        class PerceptronDual<long double>;

        template
        class PerceptronDual<unsigned char>;

        template
        class PerceptronDual<unsigned int>;

        template
        class PerceptronDual<unsigned short int>;

        template
        class PerceptronFixedMarginDual<int>;

        template
        class PerceptronFixedMarginDual<double>;

        template
        class PerceptronFixedMarginDual<float>;

        template
        class PerceptronFixedMarginDual<int8_t>;

        template
        class PerceptronFixedMarginDual<char>;

        template
        class PerceptronFixedMarginDual<long long int>;

        template
        class PerceptronFixedMarginDual<short int>;

        template
        class PerceptronFixedMarginDual<long double>;

        template
        class PerceptronFixedMarginDual<unsigned char>;

        template
        class PerceptronFixedMarginDual<unsigned int>;

        template
        class PerceptronFixedMarginDual<unsigned short int>;
    }
}