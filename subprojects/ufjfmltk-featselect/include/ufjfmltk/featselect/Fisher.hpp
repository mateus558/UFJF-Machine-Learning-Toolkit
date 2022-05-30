//
// Created by Mateus Coutinho Mari on 7/27/2018.
//

#pragma once

#include "FeatureSelection.hpp"
#include "ufjfmltk/valid/Validation.hpp"

namespace mltk{
    namespace featselect {
        template<typename T = double>
        class Fisher : public FeatureSelection<T> {
        private:
            int number = 0;

            struct fisher_select_score {
                int fname;
                double score;
            };

            static int fisher_select_compare_score_greater(const fisher_select_score &a, const fisher_select_score &b);

        public:
            Fisher() = default;
            explicit Fisher(const Data<T>& samples,
                            classifier::Classifier<T> *classifier = nullptr, int number = 0);

            Data<T> selectFeatures() override;
        };

        template<typename T>
        int Fisher<T>::fisher_select_compare_score_greater(const Fisher::fisher_select_score &a,
                                                           const Fisher::fisher_select_score &b) {
            return a.score < b.score;
        }

        template<typename T>
        Fisher<T>::Fisher(const Data<T>& samples, classifier::Classifier<T> *classifier, int final_dim) {
            this->samples = mltk::make_data<T>(samples);
            this->classifier = classifier;
            this->final_dim = final_dim;
            this->number = final_dim;
        }

        template<typename T>
        Data<T> Fisher<T>::selectFeatures() {
            size_t i, j;
            size_t num_pos = 0, num_neg = 0, svs = 0, dim = this->samples->dim(), size = this->samples->size();
            int partial = 0;
            double margin = 0.0;
            std::vector<int> remove(dim - number, -1), fnames;
            std::vector<double> avg_neg(dim), avg_pos(dim), sd_neg(dim), sd_pos(dim), w;
            std::vector<fisher_select_score> scores(dim);
            std::shared_ptr<Data<T> > stmp(std::make_shared<Data<T> >()), stmp_partial(std::make_shared<Data<T> >());
            Solution sol;

            /*calc average*/
            for (i = 0; i < dim; ++i) {
                num_neg = 0;
                num_pos = 0;
                avg_neg[i] = 0;
                avg_pos[i] = 0;
                for (j = 0; j < size; ++j) {
                    if ((*this->samples)[j]->Y() == -1) {
                        avg_neg[i] += (*this->samples)[j]->X()[i];
                        ++num_neg;
                    } else {
                        avg_pos[i] += (*this->samples)[j]->X()[i];
                        ++num_pos;
                    }
                }
                avg_neg[i] /= num_neg;
                avg_pos[i] /= num_pos;
            }

            /*calc standard deviation*/
            for (i = 0; i < dim; ++i) {
                sd_neg[i] = 0;
                sd_pos[i] = 0;
                for (j = 0; j < size; ++j) {
                    if ((*this->samples)[j]->Y() == -1)
                        sd_neg[i] += std::pow((*this->samples)[j]->X()[i] - avg_neg[i], 2);
                    else sd_pos[i] += std::pow((*this->samples)[j]->X()[i] - avg_pos[i], 2);
                }
                sd_neg[i] = sqrt(sd_neg[i] / (num_neg - 1));
                sd_pos[i] = sqrt(sd_pos[i] / (num_pos - 1));
            }

            fnames = this->samples->getFeaturesNames();

            /*calc scores*/
            for (i = 0; i < dim; ++i) {
                scores[i].score = std::pow(avg_pos[i] - avg_neg[i], 2) / (sd_pos[i] + sd_neg[i]);
                scores[i].fname = fnames[i];
                if (this->verbose)
                    std::cout << "Score: " << scores[i].score << ", Fname: " << scores[i].fname << std::endl;
            }
            if (this->verbose) std::cout << "----------------------------\n";

            if (this->verbose) std::cout << "Dim: " << dim << " -- ";

            /*training sample*/
            this->classifier->setVerbose(0);
            this->classifier->setGamma(margin);
            this->classifier->setSamples(this->samples);
            if (!this->classifier->train()) {
                w.erase(w.begin(), w.end());
                if (this->verbose) std::cout << "Training failed!\n";
                //break;
            } else {
                sol = this->classifier->getSolution();
                std::cout << "Training sucessful...\n";
                std::cout << "Margin = " << sol.margin << ", Support Vectors = " << sol.svs << "\n";
                std::cout << "----------------------------\n";
            }

            std::sort(scores.begin(), scores.end(), fisher_select_compare_score_greater);

            stmp_partial->copy(*this->samples);
            stmp->copy(*this->samples);

            for (i = 0; i < (dim - this->number); ++i) {
                if (this->verbose) std::cout << "Score: " << scores[i].score << ", Fname: " << scores[i].fname << "\n";
                remove[i] = scores[i].fname;
                stmp->removeFeatures(remove);

                if (this->verbose)
                    std::cout << "Dim: " << dim - i - 1 << " -- ";

                /*training sample*/
                w.erase(w.begin(), w.end());
                this->classifier->setGamma(margin);
                this->classifier->setSamples(stmp);
                if (!this->classifier->train()) {
                    if (this->verbose) std::cout << "Training failed!\n";
                    partial = 1;
                    break;
                } else {
                    sol = this->classifier->getSolution();
                    std::cout << "Training sucessful...\n";
                    std::cout << "Margin = " << sol.margin << ", Support Vectors = " << sol.svs << "\n";
                    std::cout << "----------------------------\n";
                }
                stmp_partial.reset();
                stmp_partial = std::make_shared<Data<T> >();
                stmp_partial->copy(*stmp);
            }

            if (partial) {
                stmp.reset();
                return *stmp_partial;
            } else {
                stmp_partial.reset();
                return *stmp;
            }
        }
    }
}
