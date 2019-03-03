//
// Created by Mateus Coutinho Mari on 7/27/2018.
//

#include "../includes/Golub.hpp"
#include <algorithm>
#include <cmath>

using namespace std;

template<typename T>
Golub<T>::Golub(shared_ptr<Data<T>> samples, Classifier<T> *classifier, int number) {
    this->samples = samples;
    this->classifier = classifier;
    this->number = number;
}

template<typename T>
shared_ptr<Data< T > > Golub< T >::selectFeatures() {
    size_t i, j;
    size_t num_pos = 0, num_neg = 0, svs = 0, dim = this->samples->getDim(), size = this->samples->getSize();
    int partial = 0;
    double margin = 0.0;
    vector<int> remove(dim-number), fnames;
    vector<double> avg_neg(dim), avg_pos(dim), sd_neg(dim), sd_pos(dim), w;
    vector<golub_select_score> scores(dim);
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
        scores[i].score = fabs(avg_pos[i]-avg_neg[i])/(sd_pos[i]+sd_neg[i]);
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

    sort(scores.begin(), scores.end(), golub_select_compare_score_greater);

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

/*----------------------------------------------------------*
 * Returns 1 for a > b, -1 a < b, 0 if a = b                *
 *----------------------------------------------------------*/
template < typename T >
int Golub< T >::golub_select_compare_score_greater(const golub_select_score &a, const golub_select_score &b)
{
    //const golub_select_score *ia = (const golub_select_score*) a;
    //const golub_select_score *ib = (const golub_select_score*) b;

    /*                V (greater)*/
    return a.score < b.score;
    //return (a.score < b.score) - (a.score > b.score);
}

template class Golub<int>;
template class Golub<double>;
template class Golub<float>;
template class Golub<int8_t>;
template class Golub<char>;
template class Golub<long long int>;
template class Golub<short int>;
template class Golub<long double>;
template class Golub<unsigned char>;
template class Golub<unsigned int>;
template class Golub<unsigned short int>;