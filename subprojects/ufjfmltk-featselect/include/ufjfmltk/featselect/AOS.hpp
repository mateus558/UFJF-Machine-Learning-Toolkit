//
// Created by Mateus Coutinho Mari on 8/20/2018.
//

#ifndef UFJF_MLTK_AOS_H
#define UFJF_MLTK_AOS_H
#define MAX_HEAP 500000
#define NUM_ERROR_EPS 0.05
#define FIRST_DECAY 0.5

#include <algorithm>
#include <functional>
#include "FeatureSelection.hpp"
#include "ufjfmltk/Core.hpp"

namespace mltk{
    namespace featselect {
        template<typename T = double>
        class AOS : public FeatureSelection<T> {
        public:
            struct select_weight {
                int fname{0};
                int indice{0};
                double w{0};
                double val{0};
                double pmargin{0};
                double radius{-1};
                double dcents{-1};
                double golub{-1};
                double fisher{-1};

                bool operator==(AOS::select_weight other) const;
            };

            struct select_gamma {
                std::vector<int> fnames;
                int level{0};
                int sv{0};
                int train{0};
                double value{0}; /*valor usado como criterio de escolha*/
                double pgamma{0}; /*projected gamma*/
                double rgamma{0}; /*real gamma p/ display*/
                double radius{-1}; /*raio*/
                double dcents{-1}; /*distancia entre os centros*/
                double golub{-1}; /*golub - estatistica*/
                double fisher{-1}; /*fisher - estatistica*/
                std::vector<double> w;
                double bias{0};

                bool operator==(AOS::select_gamma other) const;
            };


            class Hash {
            private:
                size_t length = 0, width = 0, conthash = 0;
                AOS::select_gamma ***elements;

            public:
                Hash(size_t length, size_t width);

                bool add(AOS::select_gamma *elmt);

                //void set_null(AOS::select_gamma *elmt);

                //void print(int dim);

                size_t getLength() const;

                void setLength(size_t length);

                unsigned int getWidth() const;

                void setWidth(size_t width);

                unsigned int getConthash() const;

                void setConthash(size_t conthash);

                ~Hash();
            };

            class Heap {
            private:
                size_t size = 0, contheap = 0, contheapreins = 0, maxheapsize = 0, contprooning = 0;
                AOS::select_gamma **elements;

            public:
                Heap();

                select_gamma **getElements() const;

                size_t getMaxheapsize() const;

                size_t getContheap() const;

                size_t getContheapreins() const;

                size_t getContprooning() const;

                size_t getSize() const;

                bool insert(AOS::select_gamma *tok, int cont);

                AOS::select_gamma *pop();

                void print();

                size_t projected();

                void percolate(size_t i);

                void cut(std::shared_ptr<AOS::Hash> hash, int levelat, int cut, double g_margin, int verbose);

                ~Heap();
            };

            void setBreadth(int breadth);

            void setCut(int cut);

            void setSortingShape(int sortingShape);

            void setChoiceShape(int choiceShape);

            void setLookAheadDepth(int lookAheadDepth);

        private:
            const size_t HASH_SIZE = 161387, HASH_WIDTH = 100;
            int breadth = 0, cut = 0, sorting_shape = 0, choice_shape = 0, startover = 0, look_ahead_depth = 0, ftime = 0, lool = 0;
            int dim_orig = 1, startdim;
            int contheap_parcial, contheapreins_parcial, conthash_parcial, contprooning_parcial, maxheapsize_parcial,
                    contnaoheap_parcial, conthashnaoheap_parcial, contexpandidos_parcial, contprojetados_parcial,
                    contprojtreinados_parcial, sobraprojecoes_parcial;
            int contexpanded = 0, contnotheap = 0, contprojected = 0, contprojtrained = 0, conthashnotheap = 0;
            double bonus = 0.0, n0 = 1, max_time = 0, kfolderror = 0, g_margin = 0;
            double partial_margin, partial_svs, partial_time, partial_dim;
            double START_TIME, initial_time, max_time_orig;
            bool doleave_oo = false;
            double mult_tempo = 2;
        public:
            void setMultTempo(double multTempo) {
                mult_tempo = multTempo;
            }

        private:
            int q = 2;
            mltk::KernelType kernel_type = INNER_PRODUCT;

            std::string filename;
            std::unique_ptr<Heap> heap;
            std::shared_ptr<Hash> hash;
            std::shared_ptr<Data<T> > stmp_partial{nullptr};

        private:
            static bool select_gamma_equal(const select_gamma *a, const select_gamma *b);

        public:
            AOS() = default;
            AOS(const Data<T>& samples, classifier::Classifier<T> *classifier, int final_dim,
                typename validation::CrossValidation *cv = nullptr,
                int breadth = 3, int sorting_shape = 1,
                int choice_shape = 1, int look_ahead_depth = 0, int cut = 0, int skip = 1, double bonus = 0,
                int startover = 999999, double g_margin = 0, bool doleave_oo = 0, int verbose = 0);

            Data<T> selectFeatures() override;

            void mainLoop(Data<T>& sample);

            double lookAhead(Data<T>& sample, std::vector<int> fnames_orig, std::vector<double> w_orig, int level_orig);

            void setQ(int q);
        };

        template<typename T>
        AOS<T>::AOS(const Data<T>& samples, classifier::Classifier<T> *classifier, int final_dim,
                    typename validation::CrossValidation *cv, int breadth,
                    int sorting_shape, int choice_shape, int look_ahead_depth, int cut, int skip, double bonus,
                    int startover, double g_margin, bool doleave_oo, int verbose) {
            if (cv == nullptr) {
                this->cv = new validation::CrossValidation;
            }
            this->samples = mltk::make_data<T>(samples);
            this->classifier = classifier;
            this->cv = cv;
            if(!this->cv){
                this->cv = new validation::CrossValidation();
                this->cv->seed = std::vector<unsigned int>(1, 0);
                this->cv->fold = 10;
                this->cv->qtde = 0;
                this->cv->jump = this->jump;
            }
            this->breadth = breadth;
            this->depth = this->samples->dim()-final_dim;
            this->bonus = bonus;
            this->cut = cut;
            this->look_ahead_depth = look_ahead_depth;
            this->skip = skip;
            this->startover = startover;
            this->g_margin = g_margin;
            this->leave_one_out = (bool) doleave_oo;
            this->sorting_shape = sorting_shape;
            this->choice_shape = choice_shape;
            this->verbose = verbose;
            if (classifier)
                classifier->setVerbose(0);
        }


        template<typename T>
        Data<T> AOS<T>::selectFeatures() {
            int i = 0;
            int tbreadth = 0;
            int level = 0;
            int dim = this->samples->dim();
            int startdim = dim_orig = dim;
            std::vector<int> fnames;
            int ftime = 1;
            int lool = 0;    /*leave one out level*/
            double g_margin = 0;
            std::shared_ptr<Data<T>> stmp = make_data<T>();
            max_time_orig = this->max_time;
            this->startdim = dim;

            int parcial = 0; //verifica se �ltima solu��o � uma solu��o recuperada (parcial)

            this->timer.reset();
            this->initial_time = this->timer.elapsed();

            /*checking arguments*/
            if (this->breadth > this->MAX_BREATH) { this->breadth = this->MAX_BREATH; }
            if (this->depth > dim - 1) { this->depth = dim - 1; }
            if (this->depth > this->MAX_DEPTH) { this->depth = this->MAX_DEPTH; }

            /*create a hash*/
            this->hash = std::make_shared<AOS<T>::Hash>(this->HASH_SIZE, this->HASH_WIDTH);
            this->heap = std::make_unique<AOS<T>::Heap>();

            /*inicializando o cross-validation*/
            if (this->cv->qtde > 0) {
                //utils_initialize_random();
                this->cv->seed.resize(this->cv->qtde);
                for (i = 0; i < this->cv->qtde; i++)
                    this->cv->seed[i] = (size_t) i; //rand();
                this->cv->initial_error = 0;
                this->cv->actual_error = 0;
            }

            stmp_partial = mltk::make_data<T>(this->samples->copy());
            this->ftime = ftime;

            /*do this while my depth permits*/
            while (1) {
                /*first problem to solve, when heap is empty*/
                if (this->heap->getSize() == 0) {
                    /*quit end of heap found*/
                    if (ftime == 0) /*first time = false -- nao eh a primeira vez, o processo falhou!*/
                    {
                        std::cout << "End of Heap, recovering last dimension...\n\n";

                        /*pegar os dados do ultimo lool, uma vez que foi a ultima dimensao fechada*/
                        std::cout << "---------------\n :: FINAL :: \n---------------\n";
                        std::cout << "Chosen Features: ";
                        std::vector<int> fnamesp = this->stmp_partial->getFeaturesNames();
                        for (i = 0; i < this->stmp_partial->dim() - 1; ++i)
                            std::cout << fnamesp[i] << ", ";
                        std::cout << fnamesp[i] << std::endl;


                        if (this->cv->qtde > 0) {
                            if (level % this->cv->jump != 0) {
                                for (this->cv->actual_error = 0, i = 0; i < this->cv->qtde; i++) {
                                    this->cv->actual_error += 100-validation::kfold(*this->stmp_partial, *this->classifier,
                                                                                    this->cv->fold, this->cv->seed[i], 0).accuracy;
                                }
                                this->kfolderror = this->cv->actual_error / this->cv->qtde;
                            }
                            std::cout << "Dim: " << this->partial_dim << ", Margin: " << this->partial_margin
                                      << ", SVs: " << this->partial_svs
                                      << ", Error " << this->cv->fold << "-fold: " << this->kfolderror << "%\n";
                        } else {
                            std::cout << "Dim: " << this->partial_dim << ", Margin: -" << this->partial_margin
                                      << ", SVs: " << this->partial_svs
                                      << std::endl;
                        }
                        std::cout << "Total insertions in Heap: " << this->contheap_parcial << std::endl;
                        std::cout << "Total reinsertions in Heap: " << this->contheapreins_parcial << std::endl;
                        std::cout << "Max size of the Heap: " << this->maxheapsize_parcial << std::endl;
                        std::cout << "Total prunes in the Heap: " << this->contprooning_parcial << std::endl;
                        std::cout << "Expanded nodes: " << this->contexpandidos_parcial << std::endl;
                        std::cout << "Not inserted in Heap: " << this->contnaoheap_parcial << std::endl;
                        std::cout << "Total of projections: " << this->contprojetados_parcial << std::endl;
                        std::cout << "Total of trained projections: " << this->contprojtreinados_parcial << std::endl;
                        std::cout << "Total of non-trained projections: " << this->contprojetados_parcial
                                                                             - this->contprojtreinados_parcial
                                  << std::endl;
                        //printf("Sobra de projecoes no Heap: %d\n", sobraprojecoes_parcial);
                        std::cout << "Equal nodes in Hash that didn't entered in the Heap: "
                                  << this->conthashnaoheap_parcial
                                  << std::endl;
                        std::cout << "Hash size: " << this->conthash_parcial << std::endl;
                        std::cout << "Total time: " << this->partial_time << std::endl;
                        parcial = 1;
                        /*Save data to file*/

                        //data_write(filename, stmp_partial, 0);
                        break;
                    }
                    /*check breadth*/
                    if (this->breadth > dim) this->breadth = dim;

                    /*run select*/
                    this->mainLoop(*this->samples);
                    std::cout << this->heap->getSize() << std::endl;
                    if (this->heap->getSize() == 0) {
                        std::cout << "Initial training failed!!!\n\n";
                        break;
                    }
                    ftime = 0;
                }
                    /*subsequent problems (heap not empty)*/
                else {
                    /*create new data struct*/
                    level = this->heap->getElements()[1]->level;
                    fnames = this->heap->getElements()[1]->fnames;
                    stmp->copy(*this->samples);
                    stmp->removeFeatures(fnames);

                    if (level == 1)
                        this->n0 = this->max_time *= FIRST_DECAY;
                    else if (level > 1)
                        this->max_time =
                                this->n0 * std::exp(-stmp->getTime_mult() * ((double) startdim / (startdim - level)));

                    /*stop criterium*/
                    if (this->samples->dim() == dim - this->depth && this->heap->getElements()[1]->rgamma > 0) {
                        std::cout << "---------------\n :: FINAL :: \n---------------\n";
                        std::cout << "Chosen Features: ";
                        std::vector<int> fnamesp = stmp->getFeaturesNames();
                        for (i = 0; i < stmp->dim() - 1; ++i)
                            std::cout << fnamesp[i] << ",";
                        std::cout << fnamesp[i] << std::endl;

                        std::cout << "---------------\nRemoved Features: ";
                        for (i = 0; i < this->samples->dim() - stmp->dim() - 1; ++i)
                            std::cout << this->heap->getElements()[1]->fnames[i] << ", ";
                        std::cout << this->heap->getElements()[1]->fnames[i] << std::endl;

                        if (this->cv->qtde > 0) {
                            for (this->cv->actual_error = 0, i = 0; i < this->cv->qtde; i++)
                                this->cv->actual_error += 100-validation::kfold(*stmp, *this->classifier, this->cv->fold,
                                                                                this->cv->seed[i], 0).accuracy;
                            this->kfolderror = this->cv->actual_error / this->cv->qtde;
                            std::cout << "Dim: " << stmp->dim() << ", Margin: "
                                      << this->heap->getElements()[1]->rgamma
                                      << ", SVs: " << this->heap->getElements()[1]->sv << ", Error " << this->cv->fold
                                      << "-fold: " << this->kfolderror << "%\n";
                        } else {
                            std::cout << "Dim: " << stmp->dim() << ", Margin: "
                                      << this->heap->getElements()[1]->rgamma
                                      << ", SVs: " << this->heap->getElements()[1]->sv << "\n";
                        }
                        std::cout << "Total insertions in the Heap: " << this->heap->getContheap() << std::endl;
                        std::cout << "Total reinsertions in the  Heap: " << this->heap->getContheapreins() << std::endl;
                        std::cout << "Max size of the Heap: " << this->heap->getMaxheapsize() << std::endl;
                        std::cout << "Total prune in the Heap: " << this->heap->getContprooning() << std::endl;
                        std::cout << "Expanded nodes: " << this->contexpanded << std::endl;
                        std::cout << "Not inserted in Heap: " << this->contnotheap << std::endl;
                        std::cout << "Total of projections: " << this->contprojected << std::endl;
                        std::cout << "Total of trained projections: " << this->contprojtrained << std::endl;
                        std::cout << "Total of trained projections: " << this->contprojected - this->contprojtrained
                                  << std::endl;
                        //printf("Sobra de projecoes no Heap: %d\n", aos_select_heap_projected(heap));
                        std::cout << "Equal nodes in Hash that didn't entered in the Heap: " << this->conthashnotheap
                                  << std::endl;
                        std::cout << "Hash size: " << this->hash->getConthash() << std::endl;
                        std::cout << "Total time: "
                                  << (((100.0f * clock() / CLOCKS_PER_SEC) - this->initial_time) / 100.0f) << std::endl;

                        /*Save data to file*/
                        stmp->write(this->filename, "data");
                        break;
                    }

                    /*some verbose*/
                    if (this->verbose && level <= 50) {
                        std::cout << "-- Testing node - Features (";
                        for (i = 0; i < level - 1; ++i)
                            std::cout << fnames[i] << ",";
                        std::cout << fnames[i] << ")\n";
                        std::cout << "------------------------------------------\n";
                    }

                    /*check breadth*/
                    tbreadth = this->breadth;
                    if (tbreadth > stmp->dim()) tbreadth = stmp->dim();

                    /*run select*/
                    auto stmp_copy = stmp->copy();
                    this->mainLoop(stmp_copy);

                    /*free stuff*/
                    stmp.reset(new Data<T>());
                }

                /*verbose*/
                if (this->verbose) {
                    if (this->verbose > 1) {
                        this->heap->print();
                        std::cout.flush();
                    } else
                        std::cout << "-- Heap Size: " << this->heap->getSize();
                    std::cout << "--------\n";
                }
            }

            /*free stuff*/
            if (parcial) {
                stmp.reset();
                return *this->stmp_partial;
            } else {
                this->stmp_partial.reset();
                return *stmp;
            }
        }

        /*----------------------------------------------------------*
        * Run A* feature selection main loop                       *
        *----------------------------------------------------------*/

        template<typename T>
        void AOS<T>::mainLoop(Data<T>& sample) {
            std::vector<int> ofnames;
            std::vector<double> w, w_manut;
            size_t i = 0, j = 0, k = 0;
            int level = 0;
            int svcount = 0;
            //int trained = 0;
            double margin = 0;
            double omargin = 0;
            double wnorm = 0;
            double tpmargin = 0;
            double sumnorm = 0;
            double leave_oo = -1;
            size_t dim = sample.dim();
            size_t size = sample.size();
            AOS<T>::select_gamma *gtmp = nullptr;
            std::vector<AOS<T>::select_weight> weight(dim);
            bool isPrimal = this->classifier->getFormulationString() == "Primal";
            auto data_copy = sample.copy();

            //double q = sample->q;

            int loolflag = 0; //fechar uma dimensao

            this->classifier->setSamples(sample);

            if (this->heap->getSize() == 0) {
                if(!this->classifier->train()){
                    std::cerr << "Training failed!" << std::endl;
                }
                auto solution = this->classifier->getSolution();
                w = solution.w.X();
                margin = solution.margin;
                svcount = solution.svs;
                if(ftime){
                    max_time /= mult_tempo;
                }
            } else {
                gtmp = this->heap->pop();

                if (gtmp->level > this->depth) {   /*eliminar nodo com nivel maior que a profundidade desejada*/
                    gtmp = nullptr;
                    return;
                }

                w = gtmp->w;
                if (gtmp->rgamma > 0) {
                    //trained = heap->elements[1]->train;
                    margin = gtmp->rgamma;
                    svcount = gtmp->sv;
                } else {   /*Training*/
                    margin = gtmp->pgamma;

                    this->classifier->getSolutionRef()->bias = gtmp->bias;
                    this->classifier->setSamples(sample);

                    if (!this->classifier->train()) {   /*training failed, remove this option*/
                        gtmp = nullptr;
                        if (this->verbose > 1) std::cout << "Training failed!\n";
                        return;
                    }

                    auto solution = this->classifier->getSolution();
                    w = solution.w.X();
                    margin = solution.margin;
                    svcount = solution.svs;

                    if (this->choice_shape == 2)
                        gtmp->value = margin * mltk::stats::distCenters(sample, -1);
                    else
                        gtmp->value = margin;
                    gtmp->rgamma = margin;
                    gtmp->train = 1;

                    this->contprojtrained++;

                    /*verifica se realmente eh o melhor ou se jah chegou na profundidade desejada*/
                    if ((gtmp->value < this->heap->getElements()[1]->value) ||
                        (gtmp->level == this->depth)) {   /*reinsert the node into heap*/
                        gtmp->w = w;
                        gtmp->sv = svcount;
                        this->heap->insert(gtmp, 0);
                        return;
                    }
                }
                /*this is the best solution, continue*/
                this->contexpanded++;
                //ofnames = gtmp->fnames;
                ofnames.resize((size_t) gtmp->level);
                ofnames = gtmp->fnames;
                level = gtmp->level;
                omargin = gtmp->pgamma;

                gtmp = nullptr;
                if (level > this->lool) {
                    this->lool = level;
                    loolflag = 1; //fechou a dimensao
                }
            }

            /*some verbose*/
            if (this->verbose)
                if (level > 0) {
                    std::cout << "-> Expanding features (";
                    for (i = 0; i < level - 1; ++i)
                        std::cout << ofnames[i] << ",";
                    std::cout << ofnames[i];
                    std::cout << ") -- Margin: " << margin << ", pMargin: " << omargin << ", Level: " << level << "\n";
                }

            /*calculating leave one out, if it's the first to hit this level*/
            if ((this->lool) == level && (loolflag == 1 || level == 0)) {
                loolflag = 0;
                if (this->cv->qtde > 0) {
                    if (level == 0) {
                        for (this->cv->initial_error = 0, i = 0; i < this->cv->qtde; i++)
                            this->cv->initial_error += 100-validation::kfold(sample, *this->classifier,
                                                                             this->cv->fold, this->cv->seed[i], 0).accuracy;
                        kfolderror = this->cv->initial_error / this->cv->qtde;
                    } else if (level % this->cv->jump == 0) {
                        for (this->cv->actual_error = 0, i = 0; i < this->cv->qtde; i++)
                            this->cv->actual_error += 100-validation::kfold(sample, *this->classifier,
                                                                            this->cv->fold, this->cv->seed[i], 0).accuracy;
                        kfolderror = this->cv->actual_error / this->cv->qtde;
                    }
                }

                if (this->doleave_oo) {
                    //leave_oo = utils_leave_one_out(sample, train, skip, 0);
                    std::cout << "Leave One Out -- Dim: " << this->startdim - level << ", Margin: " << margin
                              << ", LeaveOO: "
                              << leave_oo << ", SVs: " << svcount << ", Time: "
                              << ((100.0f * clock() / CLOCKS_PER_SEC) - this->initial_time) / 100.0f << "\n";
                } else {
                    leave_oo = -1;
                    std::cout << "--- --- --- --- --- --- --- ---\n";
                    if (this->cv->qtde > 0 && level % this->cv->jump == 0)
                        std::cout << "Dim: " << (this->startdim - level) << ", Margin: " << margin << ", SVs: "
                                  << svcount
                                  << ", Error " << this->cv->fold << "-fold: " << this->kfolderror << ", Time: "
                                  << ((100.0f * clock() / CLOCKS_PER_SEC) - this->initial_time) / 100.0f;
                    else
                        std::cout << "Dim: " << (this->startdim - level) << ", Margin: " << margin << ", SVs: "
                                  << svcount
                                  << ", Time: " << ((100.0f * clock() / CLOCKS_PER_SEC) - this->initial_time) / 100.0f;
                    if (level > 0) {
                        std::cout << " - Removed features: ";
                        for (j = 0; j < level - 1; j++)
                            std::cout << ofnames[j] << ",";
                        std::cout << ofnames[j] << std::endl;
                    }
                    std::cout << "\nIns: " << this->heap->getContheap() << " / ";
                    std::cout << "ReIns: " << this->heap->getContheapreins() << " / ";
                    std::cout << "Max: " << this->heap->getMaxheapsize() << " / ";
                    std::cout << "Proonings: " << this->heap->getContprooning() << " / ";
                    std::cout << "Hash: " << this->hash->getConthash() << " / ";
                    std::cout << "Expanded: " << this->contexpanded << " / ";
                    std::cout << "Not trained: " << this->contprojected - this->contprojtrained << " / ";
                    std::cout << "\n--- --- --- --- --- --- --- ---\n";
                }

                /*salvando os dados da ultima dimensao fechada*/
                this->partial_margin = margin;
                this->partial_svs = svcount;
                this->partial_time = ((100.0f * clock() / CLOCKS_PER_SEC) - this->initial_time) / 100.0f;
                this->partial_dim = startdim - level;
                this->contheap_parcial = this->heap->getContheap();
                this->contheapreins_parcial = this->heap->getContheapreins();
                this->conthash_parcial = this->hash->getConthash();
                this->contprooning_parcial = this->heap->getContprooning();
                this->maxheapsize_parcial = this->heap->getMaxheapsize();
                this->contnaoheap_parcial = this->contnotheap;
                this->conthashnaoheap_parcial = this->conthashnotheap;
                this->contexpandidos_parcial = this->contexpanded;
                this->contprojetados_parcial = this->contexpanded;
                this->contprojtreinados_parcial = this->contprojtrained;
                //sobraprojecoes_parcial = aos_select_heap_projected(heap);
                this->stmp_partial = mltk::make_data<T>();
                this->stmp_partial->copy(data_copy);
                if (look_ahead_depth > 0) {
                    if (isPrimal) {
                        /*criar um w de manutencao pra volta do look_ahead*/
                        w_manut.resize(dim);
                        w_manut = w;
                    }

                    /*get new look ahead margin*/
                    g_margin = this->lookAhead(sample, ofnames, w, level);

                    if (isPrimal) {
                        w = w_manut;
                    }
                }
                /*cut heap based on its level and look ahead margin*/
                this->heap->cut(this->hash, lool, cut, g_margin, this->verbose);

                if (this->verbose > 2) {
                    std::cout << " (" << this->heap->getSize() << ")\t";
                    for (j = 0; j < level; j++)
                        std::cout << ofnames[j] << " ";
                    std::cout << "\n";
                }
            }
            for(i = 0; i < weight.size(); i++){
                weight[i].w = w[i];
                weight[i].indice = i;
                weight[i].fname = sample.getFeaturesNames()[i];
                weight[i].radius = -1;
                weight[i].dcents = -1;
                weight[i].golub = -1;
                weight[i].fisher = -1;
            }

            if(sorting_shape == 2){
                for(i = 0; i < dim; i++){
                    weight[i].dcents = mltk::stats::distCenters(sample, weight[i].fname);
                }
            }else if(sorting_shape == 3){
                for(i = 0; i < dim; i++){
                    weight[i].radius = mltk::stats::radius(sample, weight[i].fname, q);
                    weight[i].dcents = mltk::stats::distCenters(sample, weight[i].fname);
                }
            }else if(sorting_shape == 4){
                for(i = 0; i < dim; i++){
                    weight[i].radius = mltk::stats::radius(sample, weight[i].fname, q);;
                }
            }else if(sorting_shape == 5 || sorting_shape == 6){
                int num_pos = 0, num_neg = 0;

                auto classes = sample.classes();
                auto neg_id = std::find(classes.begin(), classes.end(), -1) - classes.begin();
                auto pos_id = std::find(classes.begin(), classes.end(), 1) - classes.begin();

                num_pos = sample.classesDistribution()[pos_id];
                num_neg = sample.classesDistribution()[neg_id];

                std::vector<double> avg_pos(dim), avg_neg(dim), sd_pos(dim), sd_neg(dim);

                // calc average
                for(i = 0; i < dim; i++){
                    avg_neg[i] = 0; avg_pos[i] = 0;
                    for(j = 0; j < size; j++){
                        if(sample.point(j)->Y() == -1){
                            avg_neg[i] += sample.point(j)->X()[i];
                        }else{
                            avg_pos[i] += sample.point(j)->X()[i];
                        }
                    }
                    avg_neg[i] /= num_neg;
                    avg_pos[i] /= num_pos;
                }
                // calc variance
                for(i = 0; i < dim; i++){
                    sd_neg[i] = 0; sd_pos[i] = 0;
                    for(j = 0; j < size; j++){
                        if(sample.point(j)->Y() == -1){
                            sd_neg[i] += std::pow(sample.point(j)->X()[i]-avg_neg[i], 2);
                        }else{
                            sd_pos[i] += std::pow(sample.point(j)->X()[i]-avg_pos[i], 2);
                        }
                    }
                }

                for(i = 0; i < dim; i++){
                    weight[i].golub = std::fabs(avg_pos[i] - avg_pos[i])/(std::sqrt(sd_pos[i]/num_pos-1)) +
                                      std::sqrt(sd_neg[i]/num_neg-1);
                    weight[i].fisher = std::pow(avg_pos[i] - avg_neg[i], 2)/(sd_pos[i] + sd_neg[i]);
                }
            }

            // sorting shape
            if(sorting_shape == 6){ // w* fisher
                std::sort(weight.begin(), weight.end(), [](const auto& a, const auto& b){
                    return (((std::fabs(a.w * a.fisher) > std::fabs(b.w * b.fisher)) -
                             (std::fabs(a.w * a.fisher) < std::fabs(b.w * b.fisher)))<=0)?false:true;
                });
            }else if(sorting_shape == 5){ // w * golub
                std::sort(weight.begin(), weight.end(), [](const auto& a, const auto& b){
                    return (((std::fabs(a.w * a.golub) > std::fabs(b.w * b.golub)) -
                             (std::fabs(a.w * a.golub) < std::fabs(b.w * b.golub))) <= 0)?false:true;
                });
            }else if(sorting_shape == 4){  // w * radius
                std::sort(weight.begin(), weight.end(), [](const auto& a, const auto& b){
                    return (((std::fabs(a.w * a.radius) > std::fabs(b.w * b.radius)) -
                             (std::fabs(a.w * a.radius) < std::fabs(b.w * b.radius))) <= 0)?false:true;
                });
            }else if(sorting_shape == 3){ // w * radius / distcenter
                std::sort(weight.begin(), weight.end(), [](const auto& a, const auto& b){
                    return ((std::fabs((a.w * a.radius)/a.dcents) > std::fabs((b.w * b.radius)/b.dcents)) -
                            (std::fabs((a.w * a.radius)/a.dcents) < std::fabs((b.w * b.radius)/b.dcents))<=0)?false:true;
                });
            }else if(sorting_shape == 2){ // w / distcenter
                std::sort(weight.begin(), weight.end(), [](const auto& a, const auto& b){
                    return ((std::fabs(a.w / a.dcents) > std::fabs(b.w / b.dcents)) -
                            (std::fabs(a.w / a.dcents) < std::fabs(b.w / b.dcents)) <= 0)?false:true;
                });
            }else{
                std::sort(weight.begin(), weight.end(), [](const auto& a, const auto& b){
                    return (((std::fabs(a.w)>std::fabs(b.w)) - (std::fabs(a.w)<std::fabs(b.w))) <= 0)?false:true;
                });
            }

            if(kernel_type == INNER_PRODUCT){
                wnorm = mltk::norm(mltk::Point<double>(w), q);
            }else{
                mltk::Kernel<T> kernel(INNER_PRODUCT);
                kernel.compute(make_data<T>(sample));
                wnorm = kernel.featureSpaceNorm(make_data<T>(sample));
            }

            // branching nodes
            for(i = 0; i < breadth; i++){
                if(kernel_type == INNER_PRODUCT){
                    // projected margin computation on IMA primal
                    if(dim > 1){
                        for(sumnorm = 0, j = 0; j < dim; j++){
                            if(i != j){
                                sumnorm += std::pow(std::fabs(weight[i].w/wnorm)*margin, q);
                            }
                        }
                        tpmargin = std::pow(sumnorm, 1.0/q);
                    }
                }else{ // projected margin computation on IMA dual and SMO
                    mltk::Kernel<T> kernel;
                    auto Hk = kernel.generateMatrixHwithoutDim(make_data<T>(sample), weight[i].indice);
                    std::vector<double> alphaaux(size);

                    for(k = 0; k < size; ++k){
                        for(alphaaux[k] = 0, j = 0; j < size; ++j){
                            alphaaux[k] += sample.point(j)->Alpha() * (*Hk)[k][j];
                        }
                    }

                    for(sumnorm = 0, k = 0; k < size; k++){
                        sumnorm += alphaaux[k] * sample.point(k)->Alpha();
                    }

                    tpmargin = std::sqrt(sumnorm)/wnorm*margin;
                }
                // creating nodes for heap
                gtmp = new select_gamma;

                // setting values
                if(choice_shape == 2){
                    gtmp->value = tpmargin*weight[i].dcents;
                }else{
                    gtmp->value = tpmargin;
                }
                gtmp->pgamma = tpmargin;
                gtmp->rgamma = -1;
                gtmp->train = 0;
                gtmp->sv = 0;
                gtmp->level = level+1;
                gtmp->radius = weight[i].radius;
                gtmp->dcents = weight[i].dcents;
                gtmp->golub = weight[i].golub;
                gtmp->fisher = weight[i].fisher;

                // father node W maintenance
                if(kernel_type == INNER_PRODUCT){
                    gtmp->w.resize(dim);

                    for(k = 0, j = 0; k < dim; k++){
                        if(sample.getFeaturesNames()[j] != weight[i].fname){
                            gtmp->w[k++] = w[j];
                        }
                    }
                }

                // resizing fnames array
                gtmp->fnames.resize(level+1);

                for(j = 0; j < level; j++){
                    gtmp->fnames[j] = ofnames[j];
                }
                gtmp->fnames[j] = weight[i].fname;

                // sorting new feature array
                std::sort(gtmp->fnames.begin(), gtmp->fnames.end(), [](const auto& a, const auto& b){
                    return (((a>b)-(a < b)) <= 0)?false:true;
                });
                if(choice_shape == 2){
                    if(i != 0 && tpmargin*weight[i].dcents < (1-NUM_ERROR_EPS)*(g_margin)){
                        hash->add(gtmp);
                        contnotheap++; continue;
                    }
                }else{
                    if(i != 0 && tpmargin < (1-NUM_ERROR_EPS)*g_margin){
                        hash->add(gtmp);
                        contnotheap++; continue;
                    }
                }

                if(this->verbose){
                    if(sorting_shape == 5 || sorting_shape == 6){
                        std::cout << "  -- New Node - Feature " << weight[i].fname << ", Value: " << gtmp->value <<
                                  ", pMargin: " << gtmp->pgamma << ", DCent: " << gtmp->dcents << ", Radius: " << gtmp->radius <<
                                  ", Golub: " << gtmp->golub << ", Fisher: " << gtmp->fisher << ", Level: " << gtmp->level <<
                                  std::endl;
                    }else{
                        std::cout << "  -- New Node - Feature " << weight[i].fname << ", Value: " << gtmp->value <<
                                  ", pMargin: " << gtmp->pgamma << ", DCent: " << gtmp->dcents << ", Radius: " << gtmp->radius <<
                                  gtmp->level << std::endl;
                    }
                }

                // push node into heap if it is not redundant in hash
                if(hash->add(gtmp)){
                    heap->insert(gtmp, 1);
                    contprojected++;
                }else{
                    conthashnotheap++;
                }
            }
        }

        /*----------------------------------------------------------*
        * look ahead for pruning value                             *
        *----------------------------------------------------------*/

        template<typename T>
        double AOS<T>::lookAhead(Data<T>& sample, std::vector<int> fnames_orig, std::vector<double> w_orig, int level_orig) {
            size_t i = 0, j = 0;
            int level = level_orig;
            int svcount = 0;
            int count = 0;
            int feat = 0;
            bool isPrimal = this->classifier->getFormulationString() == "Primal";
            std::vector<int> features((unsigned int) look_ahead_depth + 1, 0);
            double min = 0;
            double margin = 0;
            double g_margin = 0;
            std::vector<double> w = w_orig;
            std::vector<double> novo_w;
            auto stmp = make_data<T>(sample.copy());
            select_gamma *gtmp = nullptr;
            double distcents = 0;
            Solution solution;

            while (true) {
                /*stopping criterion*/
                if (count == this->look_ahead_depth || count == sample.dim() - 1 ||
                    stmp->dim() == sample.dim() - this->depth || level == this->depth)
                    break;
                if (this->choice_shape == 2) {
                    /*selecting one feature with least w / dist. centers*/
                    min = fabs(w[0]) / mltk::stats::distCenters(*stmp, stmp->getFeaturesNames()[0]);
                    feat = stmp->getFeaturesNames()[0];
                    for (i = 1; i < stmp->dim(); i++) {
                        distcents = mltk::stats::distCenters(*stmp, stmp->getFeaturesNames()[i]);
//                        std::cout << "feat: " << feat << " feati: " << stmp->getFeaturesNames()[i] << " w[i]: " << w[i] << std::endl;
//                        std::cout << "distcenters: " << distcents << std::endl;
                        if (fabs(w[i]) / distcents < min) {
                            min = fabs(w[i]) / distcents;
                            feat = stmp->getFeaturesNames()[i];
                        }
                    }
                } else {
                    /*selecting one feature with least w*/
                    min = fabs(w[0]);
                    feat = stmp->getFeaturesNames()[0];
                    for (i = 1; i < stmp->dim(); i++)
                        if (fabs(w[i]) < min) {
                            min = fabs(w[i]);
                            feat = stmp->getFeaturesNames()[i];
                        }
                }

                /*manutencao do w do pai para o IMA Primal*/
                if (isPrimal) {
                    size_t dim = sample.dim() - 1;
                    std::vector<int> fnames = sample.getFeaturesNames();
                    novo_w.reserve(dim - 1);
                    for (i = 0, j = 0; j < dim; ++j)
                        if (fnames[j] != feat)
                            novo_w.push_back(w[j]);
                }

                /*saving removed feature name*/
                features[count] = feat;

                /*removing old data sample*/
                if (*stmp != sample) stmp.reset();

                /*get temp data struct*/
                stmp = make_data<T>(sample.removeFeatures(features, count+1));

                if (level == 0)
                    this->n0 = this->max_time = max_time_orig * FIRST_DECAY;
                else
                    this->max_time = n0 * std::exp(
                            -stmp->getTime_mult() * ((double) this->dim_orig / (this->dim_orig - level - 1)));

                /*creating new nodes for heap*/
                gtmp = new AOS<T>::select_gamma;
                if (gtmp == nullptr) {
                    std::cerr << "Error: Out of memory 9\n" << std::endl;
                    exit(1);
                }

                /*setting values*/
                gtmp->value = -1;
                gtmp->pgamma = solution.margin;
                gtmp->rgamma = -1;
                gtmp->train = 0;
                gtmp->sv = 0;
                gtmp->w.clear();
                gtmp->bias = 0;
                gtmp->level = level + 1;
                gtmp->radius = -1; //data_get_radius(stmp, -1, stmp->q);
                if (this->choice_shape == 2)
                    gtmp->dcents = mltk::stats::distCenters(*stmp, -1);
                else
                    gtmp->dcents = -1;
                gtmp->fisher = -1;
                gtmp->golub = -1;

                /*creating new fnames array*/
                gtmp->fnames.resize((size_t) level + 1);

                for (j = 0; j < level_orig; ++j) gtmp->fnames[j] = fnames_orig[j];
                for (j = 0; j < count + 1; ++j) gtmp->fnames[level_orig + j] = features[j];

                /*sorting new feature array*/
                std::sort(gtmp->fnames.begin(), gtmp->fnames.end(), std::greater<int>());

                if (this->verbose) {
                    std::cout << "  -- New look-ahead node - Features: " << std::endl;
                    for (i = 0; i < count; i++)
                        std::cout << features[i] << ", ";
                    std::cout << features[i] << std::endl;
                }

                /*push node into heap if it is not redundant in hash*/
                if (hash->add(gtmp)) {
                    /*training sample*/
                    svcount = 0;
                    solution.margin = gtmp->pgamma;
                    solution.bias = gtmp->bias;
                    solution.w = novo_w;
                    this->classifier->setSolution(solution);
                    if (!this->classifier->train()) {
                        if (this->verbose) std::cout << "Training failed!\n";
                        break;
                    }

                    gtmp->w = this->classifier->getSolutionRef()->w.X();
                    gtmp->bias = this->classifier->getSolutionRef()->bias;
                    gtmp->sv = this->classifier->getSolutionRef()->svs;
                    gtmp->rgamma = this->classifier->getSolutionRef()->margin;
                    gtmp->train = 1;
                    if (this->choice_shape == 2)
                        gtmp->value = this->classifier->getSolutionRef()->margin * gtmp->dcents;
                    else
                        gtmp->value = this->classifier->getSolutionRef()->margin;
                    g_margin = gtmp->value;
                    this->heap->insert(gtmp, 1);
                }
                if (isPrimal) {
                    w.resize(stmp->dim());
                    w = this->classifier->getSolutionRef()->w.X();
                }

                /*increment*/
                level++;
                count++;
            }

            w_orig = w;

            /*free stuff*/
            if (*stmp != sample) stmp.reset();

            return 0;
        }

        template<typename T>
        void AOS<T>::setBreadth(int breadth) {
            AOS::breadth = breadth;
        }

        template<typename T>
        void AOS<T>::setCut(int cut) {
            AOS::cut = cut;
        }

        template<typename T>
        void AOS<T>::setSortingShape(int sortingShape) {
            sorting_shape = sortingShape;
        }

        template<typename T>
        void AOS<T>::setChoiceShape(int choiceShape) {
            choice_shape = choiceShape;
        }

        template<typename T>
        void AOS<T>::setLookAheadDepth(int lookAheadDepth) {
            look_ahead_depth = lookAheadDepth;
        }

        template<typename T>
        void AOS<T>::setQ(int q) {
            AOS::q = q;
        }

        template<typename T>
        bool AOS<T>::select_gamma_equal(const AOS::select_gamma *a, const AOS::select_gamma *b) {
            int i = 0;
            int eq = 0;

            if(a->level != b->level){
                return false;
            }
            for(i = 0; i < a->level; i++){
                if(a->fnames[i] == b->fnames[i]) eq++;
                else break;
            }
            return (eq == a->level);
        }

        /***********************************************************
         *              HASH FUNCTIONS                             *
         ***********************************************************/

        /*----------------------------------------------------------*
        * creates a fresh new hash                                 *
        *----------------------------------------------------------*/

        template<typename T>
        AOS<T>::Hash::Hash(size_t length, size_t width) {
            size_t i, j;

            this->length = length;
            this->width = width;

            elements = new AOS<T>::select_gamma **[length];

            for (i = 0; i < length; i++) {
                elements[i] = new AOS<T>::select_gamma *[width];

                for(j = 0; j < width; j++){
                    elements[i][j] = nullptr;
                }
            }
        }

        /*----------------------------------------------------------*
        * insert an element into my hash                           *
        *----------------------------------------------------------*/

        template<typename T>
        bool AOS<T>::Hash::add(AOS<T>::select_gamma *elmt) {
            unsigned int i = 0;
            int index = 0;
            double func = 0;

            /*error check*/
            if (elmt == nullptr || this->elements == nullptr)
                return false;

            /*hashing function*/
            func = 0;
            for (i = 0; i < elmt->level; ++i)
                func += std::pow(elmt->fnames[i], 2);

            index = (int) fmod(func, this->length);

            /*skiping equals*/
            i = 0;
            while (i < this->width && this->elements[index][i] != nullptr) {
                /*check equality between nodes*/
                if (select_gamma_equal(elmt, this->elements[index][i])) {
                    /*this node is identical to some other node*/
                    /*check if this node has real gamma*/
                    if (this->elements[index][i]->rgamma < 0) {
                        /*keep node with highest projected value*/
                        if (this->elements[index][i]->value < elmt->value) {
                            this->elements[index][i]->pgamma = elmt->pgamma;
                            this->elements[index][i]->value = elmt->value;
                        }
                    }

                    /*free stuff*/
                    if(elmt) delete elmt;
                    elmt = nullptr;
                    return false;
                } else {
                    /*
                    printf("CRASH! (%d) ",index);
                    printf("[");
                    for(j = 0; j< hash->elements[index][i]->level; ++j)
                        printf("%d,",hash->elements[index][i]->fnames[j]);
                    printf("]\n");
                    */
                }
                /*increment*/
                i++;
            }

            /*adding element*/
            if (i >= this->width) {
                int filled = 0;
                for (i = 0; i < this->length; ++i)
                    if (this->elements[i][0] == nullptr) filled++;

                std::cerr << "NEED RE-HASH! (just failed) (" << filled << "/" << this->length << ") = "
                          << ((double) filled) /
                             (this->length) * 100.0 << "%\n";
                exit(1);
            } else {
                this->elements[index][i] = elmt;
                conthash++;
            }
            return true;
        }

        /*----------------------------------------------------------*
        * erase an element from my hash                            *
        *----------------------------------------------------------*/

//        template<typename T>
//        void AOS<T>::Hash::set_null(AOS<T>::select_gamma *elmt) {
//            unsigned int i = 0, j = 0;
//            int index = 0;
//            double func = 0;
//
//            /*error check*/
//            if (elmt == nullptr || this->elements == nullptr) return;
//
//            /*hashing function*/
//            for (i = 0; i < elmt->level; ++i)
//                func += std::pow(elmt->fnames[i], 2);
//
//            index = (unsigned int) fmod(func, this->length);
//
//            /*finding it*/
//            i = 0;
//            while (i < this->width && this->elements[index][i] != nullptr) {
//                /*check equality between nodes*/
//                if (select_gamma_equal(elmt, this->elements[index][i])) {
//                    /*shift elements*/
//                    j = i + 1;
//                    while (j < this->width && this->elements[index][j] != nullptr) {
//                        this->elements[index][j - 1] = this->elements[index][j];
//                        j++;
//                    }
//                    /*setting last element as null*/
//                    this->elements[index][j - 1] = nullptr;
//
//                    break;
//                }
//                i++;
//            }
//        }

        /*----------------------------------------------------------*
        * print hash table                                         *
        *----------------------------------------------------------*/

//        template<typename T>
//        void AOS<T>::Hash::print(int dim) {
//            size_t i = 0, j = 0, k = 0, d = 0;
//            int cont = 0;
//
//            for (i = 0; i < this->length; ++i)
//                for (j = 0; j < this->width; ++j)
//                    if (this->elements[i][j] != nullptr) {
//                        cont++;
//                    }
//            std::cout << "Cont = " << cont << "\n";
//            if (dim <= 10)
//                for (d = 0; d < dim; d++) {
//                    for (i = 0; i < this->length; ++i)
//                        for (j = 0; j < this->width; ++j)
//                            if (this->elements[i][j] != nullptr)
//                                if (this->elements[i][j]->level == d) {
//                                    for (k = 0; k < (this->elements[i][j]->level) - 1; ++k)
//                                        std::cout << this->elements[i][j]->fnames[k] << ",";
//                                    std::cout << this->elements[i][j]->fnames[k] << "\n";
//                                }
//                    std::cout << "\n";
//                }
//        }

        /*---------------------------------q-------------------------*
        * frees hash table                                         *
        *----------------------------------------------------------*/

        template<typename T>
        AOS<T>::Hash::~Hash() {
            size_t i = 0;

            for (i = 0; i < this->length; i++) {
                delete[] this->elements[i];
            }

            delete[] this->elements;
            this->elements = nullptr;
        }

        template<typename T>
        size_t AOS<T>::Hash::getLength() const {
            return length;
        }

        template<typename T>
        void AOS<T>::Hash::setLength(size_t length) {
            AOS<T>::Hash::length = length;
        }

        template<typename T>
        unsigned int AOS<T>::Hash::getWidth() const {
            return width;
        }

        template<typename T>
        void AOS<T>::Hash::setWidth(size_t width) {
            AOS<T>::Hash::width = width;
        }

        template<typename T>
        unsigned int AOS<T>::Hash::getConthash() const {
            return conthash;
        }

        template<typename T>
        void AOS<T>::Hash::setConthash(size_t conthash) {
            AOS<T>::Hash::conthash = conthash;
        }

        template<typename T>
        bool AOS<T>::select_gamma::operator==(const AOS<T>::select_gamma other) const {
            unsigned int i = 0;
            int eq = 0;

            if (this->level != other.level)
                return false;

            for (i = 0; i < this->level; ++i) {
                if (this->fnames[i] == other.fnames[i]) eq++;
                else break;
            }
            return (eq == this->level);
        }

        /***********************************************************
         *               HEAP FUNCTIONS                            *
         ***********************************************************/

        /*----------------------------------------------------------*
        * creates a heap                                           *
        *----------------------------------------------------------*/

        template<typename T>
        AOS<T>::Heap::Heap() {
            size_t i = 0;

            this->elements = new AOS<T>::select_gamma *[MAX_HEAP + 1];

            if (this->elements == nullptr) {
                std::cerr << "Out of mem! Heap 2\n";
                exit(1);
            }

            for (i = 0; i < MAX_HEAP + 1; ++i) this->elements[i] = nullptr;
            this->size = 0;
        }

        /*----------------------------------------------------------*
        * Insert into heap                                         *
        *----------------------------------------------------------*/

        template<typename T>
        bool AOS<T>::Heap::insert(AOS<T>::select_gamma *tok, int cont) {
            int i = 0;
            double val = 0;

            if (this->size == MAX_HEAP) {
                if (tok->value > this->elements[MAX_HEAP]->value)
                    i = MAX_HEAP;
                else return false;
            } else i = ++(this->size);

            val = (this->elements[i / 2] != nullptr) ? this->elements[i / 2]->value : 0;

            while (i > 1 && val < tok->value) {
                this->elements[i] = this->elements[i / 2];
                i /= 2;
                val = (this->elements[i / 2] != nullptr) ? this->elements[i / 2]->value : 0;
            }
            this->elements[i] = tok;

            if (this->size > this->maxheapsize)
                this->maxheapsize = this->size;

            if (cont) this->contheap++;
            else this->contheapreins++;
            return true;
        }

        /*----------------------------------------------------------*
        * Returns top element                                      *
        *----------------------------------------------------------*/

        template<typename T>
        typename AOS<T>::select_gamma *AOS<T>::Heap::pop() {
            AOS<T>::select_gamma *min_element = nullptr;

            if (this->size == 0) {
                std::cerr << "Tried to pop an empty heap!\n";
                return nullptr;
            }

            min_element = this->elements[1];

            this->size--;
            this->percolate(1);

            return min_element;
        }

        /*----------------------------------------------------------*
        * Prints heap                                              *
        *----------------------------------------------------------*/

        template<typename T>
        void AOS<T>::Heap::print() {
            int i = 0, j = 0;
            std::vector<int> fnames;
            AOS<T>::select_gamma *curr = nullptr;

            if (this->elements == nullptr) return;

            for (i = 1; i <= this->size; ++i) {
                curr = this->elements[i];
                fnames = curr->fnames;
                std::cout << "Heap[" << i << "] --";
                if (curr->level <= 50) {
                    std::cout << " (";
                    for (j = 0; j < curr->level - 1; ++j)
                        std::cout << fnames[j] << ",";
                    std::cout << fnames[j] << ")";
                }
                std::cout << " Value: " << curr->value << ",";
                std::cout << " rMargem: " << curr->rgamma << ",";
                std::cout << " pMargem: " << curr->pgamma << ",";
                std::cout << " Nivel: " << curr->level << "\n";
            }
        }

        /*----------------------------------------------------------*
        * Cont projected margin nodes in heap                      *
        *----------------------------------------------------------*/

        template<typename T>
        size_t AOS<T>::Heap::projected() {
            size_t i = 0, projected = 0;
            AOS<T>::select_gamma *curr = nullptr;

            if (this->elements == nullptr) return 0;

            for (i = 1; i <= this->size; ++i) {
                curr = this->elements[i];
                if (curr->rgamma < 0)
                    projected++;
            }
            return projected;
        }

        /*----------------------------------------------------------*
        * percolates                                               *
        *----------------------------------------------------------*/

        template<typename T>
        void AOS<T>::Heap::percolate(size_t i) {
            size_t child = i * 2;
            AOS<T>::select_gamma *last_element = nullptr;
            last_element = this->elements[this->size + 1];

            if (child > this->size) {
                this->elements[i] = last_element;
                return;
            }

            if (child != this->size && this->elements[child + 1]->value > this->elements[child]->value)
                child++;

            /*percolate one level*/
            if (last_element->value < this->elements[child]->value) {
                this->elements[i] = this->elements[child];
                this->percolate(child);
            } else this->elements[i] = last_element;
        }

        /*----------------------------------------------------------*
        * removes old levels                                       *
        *----------------------------------------------------------*/

        template<typename T>
        void AOS<T>::Heap::cut(std::shared_ptr<AOS<T>::Hash> hash, int levelat, int cut, double g_margin, int verbose) {
            size_t i = 0, count = 0;
            AOS<T>::select_gamma *curr = nullptr;

            for (i = this->size; i > 0; --i) {
                curr = this->elements[i];
                if (curr->level < levelat)
                    if (curr->value < (1 - NUM_ERROR_EPS) * g_margin || levelat - curr->level >= cut) {
                        /*errase it from hash*/
                        if(curr) delete curr;
                        curr = nullptr;
                        /*percolate heap*/
                        this->size--;
                        this->percolate(i);
                        count++;
                    }
            }
            if (verbose) std::cout << "  [Removed nodes from pruning: " << count << "]\n";

            contprooning += count;
        }

        /*----------------------------------------------------------*
        * Frees heap                                               *
        *----------------------------------------------------------*/

        template<typename T>
        AOS<T>::Heap::~Heap() {
            size_t i = 0;
            for (i = 1; i <= this->size; ++i) {
                if(this->elements[i])delete this->elements[i];
                this->elements[i] = nullptr;
            }
            delete[] this->elements;
            this->elements = nullptr;
        }

        template<typename T>
        size_t AOS<T>::Heap::getSize() const {
            return size;
        }

        template<typename T>
        typename AOS<T>::select_gamma **AOS<T>::Heap::getElements() const {
            return elements;
        }

        template<typename T>
        size_t AOS<T>::Heap::getContheap() const {
            return contheap;
        }

        template<typename T>
        size_t AOS<T>::Heap::getContheapreins() const {
            return contheapreins;
        }

        template<typename T>
        size_t AOS<T>::Heap::getContprooning() const {
            return contprooning;
        }

        template<typename T>
        size_t AOS<T>::Heap::getMaxheapsize() const {
            return maxheapsize;
        }

        template<typename T>
        bool AOS<T>::select_weight::operator==(AOS::select_weight other) const {
            return (fabs(other.w) > fabs(other.w)) - (fabs(other.w) < fabs(other.w));
        }
    }
}
#endif //UFJF_MLTK_AOS_H
