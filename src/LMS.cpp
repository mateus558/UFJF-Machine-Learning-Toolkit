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
    double error = 0.0, diff = 0.0, diffAnt = 0.0, cost_func = 0.0;
    std::vector<double> func(size, 0.0);

    if(this->verbose)
    {
        std::cout << "------------------------------------------------------------------------------------\n";
        std::cout << " steps     updates              cost_function              diff          secs\n";
        std::cout << "------------------------------------------------------------------------------------\n";
    }

    this->solution.w.assign(dim, 0.0);
    this->timer.Reset();

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
            error = (input->y - func[j]) * (*input)[j];
            if (error > this->EPS) {
                for (k = 0; k < dim; k++) {
                    this->solution.w[k] += this->rate * error * (*input)[k];
                }
                ++this->ctot;
            }
        }
        cost_func *= 0.5;
        double secs = this->timer.Elapsed();
        if (this->verbose) {
            diff = fabs(cost_func - diffAnt);
            std::cout << " " << this->steps << "           " << this->ctot << "                   " << cost_func
                      << "            " << diff << "           " << secs << "\n";
        }
        if(fabs(cost_func - diffAnt) <= this->EPS) break;
        diffAnt = cost_func;
        if(time -this->timer.Elapsed()*1000 <= 0) break;
    }
    if(this->verbose >= 1){
        std::cout << "Number of steps through data: " << this->steps << std::endl;
        std::cout << "Number of updates: " << this->ctot << std::endl;

        if(this->verbose >= 2){
            for(j = 0; j < dim; ++j) std::cout << "W[" << this->samples->getFeaturesNames()[j] << "]: " << this->solution.w[j] << "\n";
            std::cout << "Bias: " << this->solution.bias << "\n\n";
        }
    }

    return true;
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