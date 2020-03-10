//
// Created by mateus558 on 03/01/19.
//

#include "../includes/LMS.hpp"


template<typename T>
LMSPrimal<T>::LMSPrimal(std::shared_ptr<Data<T>> samples, double rate, int verbose, Solution *initial_solution) {
    this->samples = samples;
    this->rate = rate;
    this->verbose = verbose;

    if(initial_solution){
        this->solution = *initial_solution;
    }else{
        this->solution.w.resize(samples->getDim(), 0.0);
        this->solution.bias = 0.0;
    }
}

template<typename T>
bool LMSPrimal< T >::train() {
    size_t j = 0, k = 0, size = this->samples->getSize(), dim = this->samples->getDim();
    size_t time = this->start_time+this->max_time;
    double error = 0.0, diff = 0.0, diffAnt = 0.0, temp = 0.0, cost_func = 0.0;
    std::vector<double> func(size, 0.0);

    if(this->verbose)
    {
        std::cout << "------------------------------------------------------------------------------------\n";
        std::cout << " steps     updates              cost_function              diff          secs\n";
        std::cout << "------------------------------------------------------------------------------------\n";
    }

    this->timer.Reset();
    while(this->timer.Elapsed() - time <= 0) {
        for (this->steps = 0; this->steps < this->MAX_IT; this->steps++) {
            cost_func = 0;
            for (j = 0; j < size; j++) {
                auto input = (*this->samples)[j];
                //Compute function
                for (func[j] = this->solution.bias, k = 0; k < dim; k++) {
                    func[j] += (*input)[k] * this->solution.w[k];
                }
                cost_func += (func[j] - input->y) * (func[j] - input->y);

                //Verify if the point is a error
                if (input->y != func[j]) {
                    error = (input->y - func[j]) * (*input)[j];
                    //Update weights
                    for (k = 0; k < dim; k++) {
                        temp = this->solution.w[k];
                        this->solution.w[k] += this->rate * error * (*input)[k];
                        diff += fabs(this->solution.w[k] - temp);
                    }
                    ++this->ctot;
                }
            }
            cost_func *= 0.5;
            double secs = this->timer.Elapsed() / 1000;
            if (this->verbose)
                std::cout << " " << this->steps << "           " << this->ctot << "                   " << cost_func
                          << "            " << diff << "           " << secs << "\n";
            if (fabs(diff - diffAnt) <= this->EPS) break;
            diffAnt = diff;
        }
    }
    if(this->verbose >= 1){
        std::cout << "Number of steps through data: " << this->steps << std::endl;
        std::cout << "Number of updates: " << this->ctot << std::endl;

        if(this->verbose >= 2){
            for(j = 0; j < dim; ++j) std::cout << "W[" << this->samples->getFeaturesNames()[j] << "]: " << this->solution.w[j] << "\n";
            std::cout << "Bias: " << this->solution.bias << "\n\n";
        }
    }

    return false;
}

template<typename T>
KLMS<T>::KLMS(std::shared_ptr<Data<T>> samples, double rate, Kernel *K, int verbose, Solution *initial_solution) {
    this->samples = samples;
    this->rate = rate;
    this->kernel = K;
    this->verbose = verbose;
    this->solution = *initial_solution;
}

template<typename T>
bool KLMS<T>::train() {
    size_t y, e, i, j, idx, r, size = this->samples->getSize(), dim = this->samples->getDim();
    double norm = this->solution.norm, time = this->start_time+this->max_time;
    double bias = this->solution.bias, f, error = 0;
    std::vector<double> func(size, 0.0), Kv;
    auto points = this->samples->getPoints();
    dMatrix *K = this->kernel->getKernelMatrixPointer();

    if(this->alpha.size() == 0){
        this->alpha.assign(size, 0.0);
    }

    e = 1;

    while(this->timer.Elapsed() - time <= 0){
        for(e = 0, i = 0; i < size; ++i){
            y = points[i]->y;

            //Calculating function
            for(func[i] = bias, r = 0; r < size; ++r)
                func[i] += this->alpha[r] * points[r]->y*(*K)[i][r];

            //Checking if the point is a mistake
            if(y * func[i] <= 0.0){
                error = y;
                this->alpha[i] += this->alpha[i-1] + this->rate;
                bias += this->rate * y;
                ++this->ctot, ++e;
            }else if(this->steps > 0 && e > 1) break;
        }
        ++this->steps;

        //stop criterion
        if(e == 0)	   break;
        if(this->steps > this->MAX_IT) break;
        if(this->ctot > this->MAX_UP) break;
    }

    this->solution.bias = bias;
    this->solution.norm = norm;
    this->solution.alpha = this->alpha;
    this->solution.margin = 0.0;
    this->solution.w.resize(dim);

    for(i = 0; i < dim; i++){
        for(j = 0; j < size; j++){
            this->solution.w[i] += this->alpha[j]*points[j]->y*points[j]->x[i];
        }
    }

    return (e == 0);
}

template class LMSPrimal<int>;
template class LMSPrimal<double>;
template class LMSPrimal<float>;
template class LMSPrimal<int8_t>;
template class LMSPrimal<char>;
template class LMSPrimal<long long int>;
template class LMSPrimal<short int>;
template class LMSPrimal<long double>;
template class LMSPrimal<unsigned char>;
template class LMSPrimal<unsigned int>;
template class LMSPrimal<unsigned short int>;

template class KLMS<int>;
template class KLMS<double>;
template class KLMS<float>;
template class KLMS<int8_t>;
template class KLMS<char>;
template class KLMS<long long int>;
template class KLMS<short int>;
template class KLMS<long double>;
template class KLMS<unsigned char>;
template class KLMS<unsigned int>;
template class KLMS<unsigned short int>;


