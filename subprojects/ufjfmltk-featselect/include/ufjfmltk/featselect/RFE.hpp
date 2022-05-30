//
// Created by Mateus Coutinho Mari on 6/26/2018.
//

#pragma once
#define first_decay 0.25

#include "FeatureSelection.hpp"


namespace mltk{
    namespace featselect {
        template<typename T = double>
        class RFE : public FeatureSelection<T> {
        private:
            struct select_weight {
                double w;
                double val;
                double fname;
            };

            static int compare_weight_greater(const select_weight &a, const select_weight &b);

        public:
            RFE() = default;
            explicit RFE(const Data<T>& samples, classifier::Classifier<T> *classifier, int final_dim,
                         validation::CrossValidation *cv = nullptr, int skip = 1, int jump = 1,
                         bool leave_one_out = false);

            Data<T> selectFeatures() override;
        };

        template<typename T>
        RFE<T>::RFE(const Data<T>& samples, classifier::Classifier<T> *classifier, int final_dim,
                    validation::CrossValidation *cv, int skip, int jump, bool leave_one_out) {
            this->samples = mltk::make_data<T>(samples);
            this->classifier = classifier;
            this->depth = this->samples->dim()-final_dim;
            this->final_dim = final_dim;
            this->skip = skip;
            this->jump = jump;
            this->cv = cv;
            if(!this->cv){
                this->cv = new validation::CrossValidation();
                this->cv->seed = std::vector<unsigned int>(1, 0);
                this->cv->fold = 10;
                this->cv->qtde = 0;
                this->cv->jump = this->jump;
            }
            this->leave_one_out = leave_one_out;
        }

        template<typename T>
        Data<T> RFE<T>::selectFeatures() {
            size_t dim = this->samples->dim(), partial_dim = 0, i = 0, j = 0;
            std::vector<int> features, partial_features, choosen_feats, fnames;
            std::vector<double> w, new_w;
            std::vector<select_weight> weight;
            std::shared_ptr<Data<T> > stmp_partial, stmp(std::make_shared<Data<T> >());
            Solution sol;
            int svcount = 0, level = 0, leveljump = 0, partial_svs = 0;
            int partial = 0; //verifica se última solução é uma solução recuperada (parcial)
            double max_time = this->classifier->getMaxTime(), time_mult = this->samples->getTime_mult();
            double margin = 0, leave_oo = 0, kfolderror = 0, partial_time = 0, partial_margin = 0;
            double START_TIME = 100.0f * clock() / CLOCKS_PER_SEC;
            double n0 = 1;

            stmp->copy(*this->samples);
            /*error check*/
            if (this->depth < 1 || this->depth >= dim) {
                std::cerr << "Invalid depth!\n";
                return Data<T>();
            }

            features.assign(this->depth, -1);
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

            while (true) {
                svcount = 0;
                margin = 0;

                //if(level != 0) // || level == depth) //else stmp->max_time = max_time;
                if (level == 1) {
                    n0 = max_time *= first_decay;
                } else if (level > 1) {
                    max_time = n0 * std::exp(-time_mult * ((double) dim / (dim - level)));
                }

                this->classifier->setGamma(margin);
                this->classifier->setSolution(sol);
                this->classifier->setSamples(stmp);

                /*training sample*/
                if (!this->classifier->train()) {
                    sol = this->classifier->getSolution();
                    svcount = sol.svs;
                    margin = sol.margin;
                    sol.w.X().erase(sol.w.X().begin(), sol.w.X().end());

                    if (this->verbose) std::cerr << "Training Failed!\n";
                    if (level > 0) {
                        std::cout << "---------------\n :: FINAL :: \n---------------\n";
                        choosen_feats = stmp_partial->getFeaturesNames();
                        std::cout << "Choosen Features: ";
                        for (i = 0; i < stmp_partial->dim() - 1; ++i) std::cout << choosen_feats[i] << ", ";
                        std::cout << choosen_feats[i] << std::endl;

                        if (this->cv->qtde > 0) {
                            if ((dim - partial_dim) % this->cv->jump != 0) {
                                for (this->cv->actual_error = 0, i = 0; i < this->cv->qtde; i++) {
                                    this->cv->actual_error += 100- validation::kfold(*stmp, *this->classifier,
                                                                                     this->cv->fold, this->cv->seed[i], 0).accuracy;
                                }
                                kfolderror = this->cv->actual_error / this->cv->qtde;
                            }
                            std::cout << "Dim: " << partial_dim << ", Margin: " << partial_dim << ", SVs: " << partial_svs
                                 << ", Error " << this->cv->fold << "-fold: " << kfolderror << "%\n";
                        } else
                            std::cout << "Dim: " << partial_dim << ", Margin: " << partial_dim << ", SVs: " << partial_svs
                                 << std::endl;
                        std::cout << "---------------\nTotal time: " << partial_time << "\n\n";
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
                partial_time = (100.0f * clock() / CLOCKS_PER_SEC - START_TIME) / 100.0f;
                partial_dim = dim - level;

                stmp_partial.reset();
                stmp_partial = std::make_shared<Data<T> >();
                stmp_partial->copy(*this->samples);

                partial_features.clear();

                int levelminusjump = (level - this->jump);

                if (levelminusjump > 0) {
                    partial_features.resize((size_t) (level - this->jump));
                }

                for (i = 0; (i < levelminusjump && levelminusjump > 0); ++i) {
                    partial_features[i] = features[i];
                }

                if (this->cv->qtde > 0) {
                    if (level == 0) {
                        for (this->cv->initial_error = 0, i = 0; i < this->cv->qtde; i++) {
                            auto report = validation::kfold(*stmp, *this->classifier, this->cv->fold, this->cv->seed[i], 0);
                            this->cv->initial_error = report.error;
                        }
                        kfolderror = this->cv->initial_error / this->cv->qtde;
                    } else if (level % this->cv->jump == 0) {
                        for (this->cv->actual_error = 0, i = 0; i < this->cv->qtde; i++) {
                            this->cv->actual_error += 100-validation::kfold(*stmp, *this->classifier, this->cv->fold,
                                                                            this->cv->seed[i], 0).accuracy;
                        }
                        kfolderror = this->cv->actual_error / this->cv->qtde;
                    }
                }

                /*leave one out*/
                if (this->leave_one_out) {
                    //   leave_oo = utils_leave_one_out(stmp, train, skip, 0);
                    std::cout << "LeaveOO -- Dim: " << (dim - level) << ", Margin: " << margin << ", LeaveOO: " << leave_oo
                         << ", SVs: " << svcount << std::endl;
                } else if (this->verbose) {
                    if (this->cv->qtde > 0 && level % this->cv->jump == 0)
                        std::cout << "Dim: " << (dim - level) << ", Margin: " << margin << ", SVs: " << svcount << ", Erro "
                             << this->cv->fold << "-fold: " << kfolderror << "%\n";
                    else
                        std::cout << "Dim: " << (dim - level) << ", Margin: " << margin << ", SVs: " << svcount << std::endl;
                    //printf("Dim: %d, Margem: %lf, Distancia entre os centros: %f, SVs: %d\n", (dim-level), data_get_dist_centers(stmp), margin, svcount);
                }

                w = this->classifier->getSolutionRef()->w.X();
                weight.resize(stmp->dim());
                auto fnames = stmp->getFeaturesNames();

                for (i = 0; i < stmp->dim(); ++i) {
                    weight[i].w = w[i];
                    weight[i].fname = fnames[i];
                }

                std::sort(weight.begin(), weight.end(), compare_weight_greater);

                std::cout << "---------------------\n";
                if (this->verbose > 1) {
                    for (i = 0; i < stmp->dim(); ++i)
                        std::cout << weight[i].fname << ": " << weight[i].w << std::endl;
                    std::cout << "---------------------\n";
                }

                /*stopping criterion*/
                if (level >= this->depth ||
                    (this->cv->qtde > 0 && (this->cv->actual_error - this->cv->initial_error) > this->cv->limit_error)) {
                    std::cout << "---------------\n :: FINAL :: \n---------------\n";
                    choosen_feats = stmp->getFeaturesNames();
                    std::cout << "Choosen Features: ";
                    for (i = 0; i < stmp->dim() - 1; ++i) std::cout << choosen_feats[i] << ",";
                    std::cout << choosen_feats[i] << std::endl;

                    std::cout << "---------------\nEliminated Features: ";
                    for (i = 0; i < leveljump - 1; ++i) std::cout << features[i] << ",";
                    std::cout << features[i] << std::endl;

                    if (this->cv->qtde > 0) {
                        if (level % this->cv->jump != 0) {
                            for (this->cv->actual_error = 0, i = 0; i < this->cv->qtde; i++) {
                                this->cv->actual_error += 100- validation::kfold(*stmp, *this->classifier, this->cv->fold,
                                                                                 this->cv->seed[i], 0).accuracy;
                            }
                            kfolderror = this->cv->actual_error / this->cv->qtde;
                        }
                        std::cout << "Dim: " << (dim - level) << ", Margin: " << margin << ", SVs: " << svcount << ", Erro "
                             << this->cv->fold << "-fold: " << kfolderror << "%\n";
                    } else
                        std::cout << "Dim: " << (dim - level) << ", Margem: " << margin << ", SVs: " << svcount << std::endl;

                    std::cout << "---------------\nTotal time: " << (100.0f * clock() / CLOCKS_PER_SEC - START_TIME) / 100.0f
                         << "\n\n";

                    //data_write(filename, stmp, 0);
                    weight.clear();
                    break;
                }

                if (level + this->jump > this->depth)
                    leveljump = this->depth;
                else
                    leveljump = level + this->jump;

                /*manutencao do w do pai para o IMA Primal*/
                if (this->classifier->getFormulationString() == "Primal") {
                    for (j = 0; j < stmp->dim(); ++j)
                        for (i = level; i < leveljump; ++i)
                            if (weight[i - level].w == w[j])
                                w[j] = 0;

                    new_w.resize(dim - leveljump);

                    for (i = 0, j = 0; j < stmp->dim(); ++j)
                        if (w[j] != 0)
                            new_w[i++] = w[j];
                    //novo_w[i] = w[j]; //bias nao copia mais
                    w.clear();
                    w = new_w;
                    sol.w.clear();
                    sol.w = w;
                } else //IMA Dual e SMO
                {
                    w.clear();
                }

                // if(*stmp != *this->samples){ stmp.reset(); }

                /*saving removed feature name*/
                for (i = level; i < leveljump; ++i) {
                    std::cout << "Removing w = " << weight[i - level].w << "\n";
                    features[i] = weight[i - level].fname;
                }
                std::cout << "---------------------\n";
                weight.clear();

                /*increment*/
                if (level + this->jump > this->depth) {
                    level = this->depth;
                    this->jump = 0;
                } else {
                    level += this->jump;

                }
                /*get temp data struct*/
                stmp->removeFeatures(features);
            }
            /*free stuff*/

            if (this->cv->qtde > 0) this->cv->seed.clear();

            //samples.reset();
            if (partial) {
                stmp.reset();
                return *stmp_partial;
            } else {
                stmp_partial.reset();
                return *stmp;
            }
        }

        /*----------------------------------------------------------*
        * Returns 1 for a > b, -1 a < b, 0 if a = b                *
        *----------------------------------------------------------*/
        template<typename T>
        int RFE<T>::compare_weight_greater(const select_weight &a, const select_weight &b) {
            /*                 V (greater)*/
            //printf("%d\n",(fabs(ia->w) > fabs(ib->w)) - (fabs(ia->w) < fabs(ib->w)));
            return fabs(a.w) < fabs(b.w);
            //return (fabs(a.w) > fabs(b.w)) - (fabs(a.w) < fabs(b.w));
        }
    }
}
