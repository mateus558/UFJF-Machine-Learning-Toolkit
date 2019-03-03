//
// Created by Mateus Coutinho Mari on 6/26/2018.
//

#include <algorithm>
#include <cmath>
#include <memory>
#include "../includes/RFE.hpp"

#define first_decay 0.25

using namespace std;

template < typename T >
RFE< T >::RFE(std::shared_ptr<Data< T > > samples, Classifier< T > *classifier, typename Validation< T >::CrossValidation *cv, int depth, int skip, int jump, bool leave_one_out) {
    this->samples = samples;
    this->classifier = classifier;
    this->depth = depth;
    this->skip = skip;
    this->jump = jump;
    this->cv = cv;
    this->leave_one_out = leave_one_out;
}

template < typename T >
std::shared_ptr<Data< T > > RFE< T >::selectFeatures() {
    size_t dim = this->samples->getDim(), partial_dim = 0, i = 0, j = 0;
    vector<int> features, partial_features, choosen_feats, fnames;
    vector<double> w, new_w;
    vector<select_weight> weight;
    shared_ptr<Data< T > > stmp_partial, stmp(make_shared<Data< T > >());
    Validation< T > validation(this->samples, this->classifier);
    Solution sol;
    int svcount = 0, level = 0, leveljump = 0, partial_svs = 0;
    int partial = 0; //verifica se última solução é uma solução recuperada (parcial)
    double max_time = this->classifier->getMaxTime(), time_mult = this->samples->getTime_mult();
    double margin = 0, leave_oo = 0, kfolderror = 0, partial_time = 0, partial_margin = 0;
    double START_TIME = 100.0f * clock() / CLOCKS_PER_SEC;
    double n0 = 1;

    *stmp = (*this->samples).copy();
    /*error check*/
    if(this->depth < 1 || this->depth >= dim){
        cerr << "Invalid depth!\n";
        return 0;
    }

    features.resize(this->depth);
    validation.setSamples(stmp);

    /*inicializando o cross-validation*/
    if (this->cv->qtde > 0) {
        //utils_initialize_random();
        this->cv->seed.resize(this->cv->qtde);
        for (i = 0; i < this->cv->qtde; i++)
            this->cv->seed[i] = i; //rand();
        this->cv->initial_error = 0;
        this->cv->actual_error = 0;
    }

    this->classifier->setVerbose(0);
    validation.setVerbose(0);

    while(true) {
        svcount = 0;
        margin = 0;

        //if(level != 0) // || level == depth) //else stmp->max_time = max_time;
        if (level == 1) {
            n0 = max_time *= first_decay;
        }else if (level > 1) {
            max_time = n0 * exp(-time_mult * ((double) dim / (dim - level)));
        }

        this->classifier->setGamma(margin);
        this->classifier->setSolution(sol);
        this->classifier->setSamples(stmp);

        /*training sample*/
        if (!this->classifier->train()) {
            sol = this->classifier->getSolution();
            svcount = sol.svs;
            margin = sol.margin;
            sol.w.erase(sol.w.begin(), sol.w.end());

            if (this->verbose) cerr << "Training Failed!\n";
            if (level > 0) {
                cout << "---------------\n :: FINAL :: \n---------------\n";
                choosen_feats = stmp_partial->getFeaturesNames();
                cout << "Choosen Features: ";
                for (i = 0; i < stmp_partial->getDim() - 1; ++i) cout << choosen_feats[i] << ", ";
                cout << choosen_feats[i] << endl;

                if (this->cv->qtde > 0) {
                    if ((dim - partial_dim) % this->cv->jump != 0) {
                        for (this->cv->actual_error = 0, i = 0; i < this->cv->qtde; i++) {
                            this->cv->actual_error += validation.kFold(this->cv->fold, this->cv->seed[i]);
                        }
                        kfolderror = this->cv->actual_error / this->cv->qtde;
                    }
                    cout << "Dim: " << partial_dim << ", Margin: " << partial_dim << ", SVs: " << partial_svs
                         << ", Error " << this->cv->fold << "-fold: " << kfolderror << "%\n";
                } else
                    cout << "Dim: " << partial_dim << ", Margin: " << partial_dim << ", SVs: " << partial_svs << endl;
                cout << "---------------\nTotal time: " << partial_time << "\n\n";
                partial = 1;
                //data_write(filename, stmp_parcial, 0);
                weight.clear();
            }
            break;
        }
        sol = this->classifier->getSolution();
        margin = sol.margin;
        svcount = sol.svs;
        partial_margin = margin;
        partial_svs = svcount;
        partial_time = (100.0f*clock()/CLOCKS_PER_SEC-START_TIME)/100.0f;
        partial_dim = dim-level;

        stmp_partial.reset();
        stmp_partial = make_shared<Data< T > >();
        *stmp_partial = (*this->samples).copy();

        partial_features.clear();

        int levelminusjump = (level-this->jump);

        if(levelminusjump > 0)
        {
            partial_features.resize((size_t)(level-this->jump));
        }

        for(i = 0; (i < levelminusjump && levelminusjump > 0); ++i) {
            partial_features[i] = features[i];
        }

        if(this->cv->qtde > 0)
        {
            if(level == 0)
            {
                for (this->cv->initial_error = 0, i = 0; i < this->cv->qtde; i++) {
                    this->cv->initial_error += validation.kFold(this->cv->fold, this->cv->seed[i]);
                }
                kfolderror = this->cv->initial_error / this->cv->qtde;
            }
            else if(level % this->cv->jump == 0)
            {
                for (this->cv->actual_error = 0, i = 0; i < this->cv->qtde; i++) {
                    this->cv->actual_error += validation.kFold(this->cv->fold, this->cv->seed[i]);
                }
                kfolderror = this->cv->actual_error / this->cv->qtde;
            }
        }

        /*leave one out*/
        if(this->leave_one_out)
        {
            //   leave_oo = utils_leave_one_out(stmp, train, skip, 0);
            cout << "LeaveOO -- Dim: "<< (dim-level) << ", Margin: " << margin << ", LeaveOO: " << leave_oo << ", SVs: " << svcount << endl;
        }
        else if(this->verbose)
        {
            if(this->cv->qtde > 0 && level % this->cv->jump == 0)
                cout << "Dim: " << (dim - level) <<", Margin: " << margin << ", SVs: " << svcount <<", Erro " << this->cv->fold << "-fold: " << kfolderror << "%\n";
            else
                cout << "Dim: " << (dim-level) << ", Margin: " << margin << ", SVs: " << svcount << endl;
            //printf("Dim: %d, Margem: %lf, Distancia entre os centros: %f, SVs: %d\n", (dim-level), data_get_dist_centers(stmp), margin, svcount);
        }

        w = this->classifier->getSolutionRef()->w;
        weight.resize(stmp->getDim());
        auto fnames = stmp->getFeaturesNames();

        for(i = 0; i < stmp->getDim(); ++i)
        {
            weight[i].w = w[i];
            weight[i].fname = fnames[i];
        }

        sort(weight.begin(), weight.end(), compare_weight_greater);

        cout << "---------------------\n";
        if(this->verbose > 1)
        {
            for(i = 0; i < dim; ++i)
                cout << weight[i].fname << ": " << weight[i].w << endl;
            cout << "---------------------\n";
        }

        /*stopping criterion*/
        if(level >= this->depth || (this->cv->qtde > 0 && this->cv->actual_error-this->cv->initial_error > this->cv->limit_error))
        {
            cout << "---------------\n :: FINAL :: \n---------------\n";
            choosen_feats = stmp->getFeaturesNames();
            cout << "Choosen Features: ";
            for(i = 0; i < stmp->getDim()-1; ++i) cout << choosen_feats[i] <<",";
            cout << choosen_feats[i] << endl;

            cout << "---------------\nEliminated Features: ";
            for(i = 0; i < leveljump-1; ++i) cout << features[i] << ",";
            cout << features[i] << endl;

            if(this->cv->qtde > 0)
            {
                if(level % this->cv->jump != 0)
                {
                    for (this->cv->actual_error = 0, i = 0; i < this->cv->qtde; i++) {
                        this->cv->actual_error += validation.kFold(this->cv->fold, this->cv->seed[i]);
                    }
                    kfolderror = this->cv->actual_error / this->cv->qtde;
                }
                cout << "Dim: " << (dim - level) <<", Margin: " << margin << ", SVs: " << svcount <<", Erro " << this->cv->fold << "-fold: " << kfolderror << "%\n";
            }
            else
                cout << "Dim: " << (dim-level) << ", Margem: " << margin << ", SVs: " << svcount << endl;

            cout << "---------------\nTotal time: " << (100.0f*clock()/CLOCKS_PER_SEC-START_TIME)/100.0f << "\n\n";

            //data_write(filename, stmp, 0);
            weight.clear();
            break;
        }

        if(level+this->jump > this->depth)
            leveljump = this->depth;
        else
            leveljump = level+this->jump;

        /*manutencao do w do pai para o IMA Primal*/
        if(this->classifier->classifierType() == "Primal")
        {
            for(j = 0; j < stmp->getDim(); ++j)
                for(i = level; i < leveljump; ++i)
                    if(weight[i-level].w == w[j])
                        w[j] = 0;

            new_w.resize(dim-leveljump);

            for(i = 0, j = 0; j < stmp->getDim(); ++j)
                if(w[j] != 0)
                    new_w[i++] = w[j];
            //novo_w[i] = w[j]; //bias nao copia mais
            w.clear();
            w = new_w;
        }
        else //IMA Dual e SMO
        {
            w.clear();
        }

       // if(*stmp != *this->samples){ stmp.reset(); }

        /*saving removed feature name*/
        for(i = level; i < leveljump; ++i)
        {
            cout << "Removing w = " << weight[i-level].w << "\n";
            features[i] = weight[i-level].fname;
        }
        cout << "---------------------\n";
        weight.clear();

        /*increment*/
        if(level+this->jump > this->depth)
        {
            level = this->depth;
            this->jump  = 0;
        }
        else {
            level += this->jump;

        }
        /*get temp data struct*/
        stmp->removeFeatures(features);
    }
    /*free stuff*/

    if(this->cv->qtde > 0) this->cv->seed.clear();

    //samples.reset();
    if(partial)
    {
        stmp.reset();
        return std::move(stmp_partial);
    }
    else
    {
        stmp_partial.reset();
        return std::move(stmp);
    }
}

/*----------------------------------------------------------*
 * Returns 1 for a > b, -1 a < b, 0 if a = b                *
 *----------------------------------------------------------*/
template < typename T >
int RFE< T >::compare_weight_greater(const select_weight &a, const select_weight &b) {
    /*                 V (greater)*/
    //printf("%d\n",(fabs(ia->w) > fabs(ib->w)) - (fabs(ia->w) < fabs(ib->w)));
    return  fabs(a.w) < fabs(b.w);
    //return (fabs(a.w) > fabs(b.w)) - (fabs(a.w) < fabs(b.w));
}

template class RFE<int>;
template class RFE<double>;
template class RFE<float>;
template class RFE<int8_t>;
template class RFE<char>;
template class RFE<long long int>;
template class RFE<short int>;
template class RFE<long double>;
template class RFE<unsigned char>;
template class RFE<unsigned int>;
template class RFE<unsigned short int>;