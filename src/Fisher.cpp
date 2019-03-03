//
// Created by Mateus Coutinho Mari on 7/27/2018.
//

#include "../includes/Fisher.hpp"
#include <algorithm>
#include <cmath>

using namespace std;

template<typename T>
int Fisher<T>::fisher_select_compare_score_greater(const Fisher::fisher_select_score &a,
                                                   const Fisher::fisher_select_score &b) {
    return a.score < b.score;
}

template<typename T>
Fisher<T>::Fisher(std::shared_ptr<Data<T> > samples, Classifier<T> *classifier, int final_dim) {
    this->samples = samples;
    this->classifier = classifier;
    this->final_dim = final_dim;
}

template<typename T>
shared_ptr<Data<T>> Fisher<T>::selectFeatures() {
    size_t i, j;
    size_t num_pos = 0, num_neg = 0, svs = 0, dim = this->samples->getDim(), size = this->samples->getSize();
    int partial = 0;
    double margin = 0.0;
    vector<int> remove(dim-number), fnames;
    vector<double> avg_neg(dim), avg_pos(dim), sd_neg(dim), sd_pos(dim), w;
    vector<fisher_select_score> scores(dim);
    shared_ptr<Data< T > > stmp(make_shared<Data< T > >()), stmp_partial(make_shared<Data< T > >());
    Solution sol;

    this->number = this->final_dim;

    /*calc average*/
    for(i = 0; i < dim; ++i)
    {
        num_neg = 0;
        num_pos = 0;
        avg_neg[i] = 0;
        avg_pos[i] = 0;
        for(j = 0; j < size; ++j)
        {
            if((*this->samples)[j]->y == -1)
            {
                avg_neg[i] += (*this->samples)[j]->x[i];
                ++num_neg;
            }
            else
            {
                avg_pos[i] += (*this->samples)[j]->x[i];
                ++num_pos;
            }
        }
        avg_neg[i] /= num_neg;
        avg_pos[i] /= num_pos;
    }

    /*calc standard deviation*/
    for(i = 0; i < dim; ++i)
    {
        sd_neg[i] = 0;
        sd_pos[i] = 0;
        for(j = 0; j < size; ++j)
        {
            if((*this->samples)[j]->y == -1) sd_neg[i] += pow((*this->samples)[j]->x[i]-avg_neg[i], 2);
            else                          sd_pos[i] += pow((*this->samples)[j]->x[i]-avg_pos[i], 2);
        }
        sd_neg[i] = sqrt(sd_neg[i]/(num_neg-1));
        sd_pos[i] = sqrt(sd_pos[i]/(num_pos-1));
    }

    fnames = this->samples->getFeaturesNames();

    /*calc scores*/
    for(i = 0; i < dim; ++i)
    {
        scores[i].score = pow(avg_pos[i]-avg_neg[i], 2)/(sd_pos[i]+sd_neg[i]);
        scores[i].fname = fnames[i];
        if(this->verbose)
            cout << "Score: " << scores[i].score << ", Fname: " << scores[i].fname << endl;
    }
    if(this->verbose) cout << "----------------------------\n";

    if(this->verbose) cout << "Dim: "<< dim << " -- ";

    /*training sample*/
    this->classifier->setVerbose(0);
    this->classifier->setGamma(margin);
    this->classifier->setSamples(this->samples);
    if(!this->classifier->train())
    {
        w.erase(w.begin(), w.end());
        if(this->verbose) cout << "Training failed!\n";
        //break;
    }
    else
    {
        sol = this->classifier->getSolution();
        cout << "Training sucessful...\n";
        cout << "Margin = " << sol.margin << ", Support Vectors = "<< sol.svs << "\n";
        cout << "----------------------------\n";
    }

    sort(scores.begin(), scores.end(), fisher_select_compare_score_greater);

    *stmp_partial = this->samples->copy();
    *stmp = this->samples->copy();

    for(i = 0; i < (dim-this->number); ++i)
    {
        if(this->verbose) cout << "Score: " << scores[i].score << ", Fname: " << scores[i].fname << "\n";
        remove[i] = scores[i].fname;
        stmp->removeFeatures(remove);

        if(this->verbose)
            cout << "Dim: " << dim-i-1 << " -- ";

        /*training sample*/
        w.erase(w.begin(), w.end());
        this->classifier->setGamma(margin);
        this->classifier->setSamples(stmp);
        if(!this->classifier->train())
        {
            if(this->verbose) cout << "Training failed!\n";
            partial = 1;
            break;
        }
        else
        {
            sol = this->classifier->getSolution();
            cout << "Training sucessful...\n";
            cout << "Margin = " << sol.margin << ", Support Vectors = "<< sol.svs << "\n";
            cout << "----------------------------\n";
        }
        stmp_partial.reset();
        stmp_partial = make_shared<Data< T > >();
        *stmp_partial = stmp->copy();
    }

    if(partial)
    {
        stmp.reset();
        return stmp_partial;
    }
    else
    {
        stmp_partial.reset();
        return stmp;
    }
}

template class Fisher<int>;
template class Fisher<double>;
template class Fisher<float>;
template class Fisher<int8_t>;
template class Fisher<char>;
template class Fisher<long long int>;
template class Fisher<short int>;
template class Fisher<long double>;
template class Fisher<unsigned char>;
template class Fisher<unsigned int>;
template class Fisher<unsigned short int>;