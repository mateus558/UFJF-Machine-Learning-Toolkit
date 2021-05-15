//
// Created by Mateus Coutinho Mari on 8/20/2018.
//

#include <algorithm>
#include <functional>
#include "ufjfmltk/featselect/AOS.hpp"
#include "ufjfmltk/classifier/PrimalClassifier.hpp"
#include "ufjfmltk/classifier/SMO.hpp"

#define MAX_HEAP 500000
#define NUM_ERROR_EPS 0.05
#define FIRST_DECAY 0.5

namespace mltk{
    namespace featselect {
        template<typename T>
        AOS<T>::AOS(std::shared_ptr<Data<T> > samples, classifier::Classifier<T> *classifier,
                    typename validation::CrossValidation *cv,
                    int breadth, int depth, double bonus, int cut, int look_ahead_depth, int skip,
                    int startover, double g_margin, bool doleave_oo, int sorting_shape, int choice_shape, int verbose) {
            if (cv == nullptr) {
                this->cv = new validation::CrossValidation;
            }
            this->samples = samples;
            this->classifier = classifier;
            this->cv = cv;
            this->breadth = breadth;
            this->depth = depth;
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
        std::shared_ptr<Data<T>> AOS<T>::selectFeatures() {
            int i = 0;
            int tbreadth = 0;
            int level = 0;
            int dim = this->samples->dim();
            int startdim = dim_orig = dim;
            std::vector<int> fnames;
            int ftime = 1;
            int lool = 0;    /*leave one out level*/
            double g_margin = 0;
            Data<T> stmp;
            max_time_orig = this->max_time;
            this->startdim = dim;

            int parcial = 0; //verifica se �ltima solu��o � uma solu��o recuperada (parcial)

            this->timer.Reset();
            this->initial_time = this->timer.Elapsed();

            /*checking arguments*/
            if (this->breadth > this->MAX_BREATH) { this->breadth = this->MAX_BREATH; }
            if (this->depth > dim - 1) { this->depth = dim - 1; }
            if (this->depth > this->MAX_DEPTH) { this->depth = this->MAX_DEPTH; }

            /*create a hash*/
            this->hash = std::make_unique<AOS<T>::Hash>(this->HASH_SIZE, this->HASH_WIDTH);
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
                    this->mainLoop();
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
                    stmp.copy(*this->samples);
                    stmp.removeFeatures(fnames);

                    if (level == 1)
                        this->n0 = this->max_time *= FIRST_DECAY;
                    else if (level > 1)
                        this->max_time =
                                this->n0 * std::exp(-stmp.getTime_mult() * ((double) startdim / (startdim - level)));

                    /*stop criterium*/
                    if (this->samples->dim() == dim - this->depth && this->heap->getElements()[1]->rgamma > 0) {
                        std::cout << "---------------\n :: FINAL :: \n---------------\n";
                        std::cout << "Chosen Features: ";
                        std::vector<int> fnamesp = stmp.getFeaturesNames();
                        for (i = 0; i < stmp.dim() - 1; ++i)
                            std::cout << fnamesp[i] << ",";
                        std::cout << fnamesp[i] << std::endl;

                        std::cout << "---------------\nRemoved Features: ";
                        for (i = 0; i < this->samples->dim() - stmp.dim() - 1; ++i)
                            std::cout << this->heap->getElements()[1]->fnames[i] << ", ";
                        std::cout << this->heap->getElements()[1]->fnames[i] << std::endl;

                        if (this->cv->qtde > 0) {
                            for (this->cv->actual_error = 0, i = 0; i < this->cv->qtde; i++)
                                this->cv->actual_error += 100-validation::kfold(stmp, *this->classifier, this->cv->fold,
                                                                            this->cv->seed[i], 0).accuracy;
                            this->kfolderror = this->cv->actual_error / this->cv->qtde;
                            std::cout << "Dim: " << stmp.dim() << ", Margin: "
                                      << this->heap->getElements()[1]->rgamma
                                      << ", SVs: " << this->heap->getElements()[1]->sv << ", Error " << this->cv->fold
                                      << "-fold: " << this->kfolderror << "%\n";
                        } else {
                            std::cout << "Dim: " << stmp.dim() << ", Margin: "
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
                        stmp.write(this->filename, "data");
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
                    if (tbreadth > stmp.dim()) tbreadth = stmp.dim();

                    /*run select*/
                    this->mainLoop();

                    /*free stuff*/
                    stmp.clear();
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
                stmp.clear();
                return this->stmp_partial;
            } else {
                this->stmp_partial.reset();
                return std::make_shared<Data<T> >(stmp);
            }
        }

        /*----------------------------------------------------------*
        * Run A* feature selection main loop                       *
        *----------------------------------------------------------*/

        template<typename T>
        void AOS<T>::mainLoop() {
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
            AOS<T>::select_gamma *gtmp = nullptr;
            AOS<T>::select_weight *weight = nullptr;
            bool isPrimal = this->classifier->getFormulationString() == "Primal";

            size_t dim = this->samples->dim();
            size_t size = this->samples->size();
            //double q = sample->q;

            int loolflag = 0; //fechar uma dimensao

            if (this->heap->getSize() == 0) {
                if (this->ftime && isPrimal) //primeira dimens�o -- solu��o exata primal
                {
                    auto *primal = dynamic_cast<classifier::PrimalClassifier<T> *>(this->classifier);

                    if (primal->getQ() == 2) {
                        Kernel K;

                        K.setType(0);
                        K.setParam(0);

                        classifier::SMO<T> smo(this->samples, &K, 0);

                        if (!smo.train()) {
                            //if (!smo_train(sample, &w, &margin, &svcount, 0)) {
                            if (this->verbose > 1) std::cout << "Training failed!\n";
                            return;
                        }

                        Solution sol = smo.getSolution();

                        w = sol.w;
                        margin = sol.margin;
                        svcount = sol.svs;

                    } else if (primal->getQ() == 1) {
                        /* Implementar solução exata por programação linear
                        if (!linear_programming(sample, &w, &margin, &svcount, 0)) {
                            if (this->verbose > 1) std::cout << "Treinamento falhou!\n";
                            return;
                        }*/
                    }
                } else if (this->ftime &&
                           this->samples->getTime_mult() == 2) //primeira dimens�o -- solu��o exata dual -- "gambiarra"
                {
                    Kernel K;

                    K.setParam(0);
                    K.setType(0);

                    classifier::SMO<T> smo(this->samples, &K, 0);
                    if (!smo.train()) {
                        //if (!smo_train(sample, &w, &margin, &svcount, 0)) {
                        if (this->verbose > 1) std::cout << "Training failed!\n";
                        return;
                    }

                    Solution sol = smo.getSolution();

                    w = sol.w;
                    margin = sol.margin;
                    svcount = sol.svs;
                } else {
                    if (!this->classifier->train()) {
                        if (this->verbose > 1) std::cout << "Training failed!\n";
                        return;
                    }

                    Solution sol = this->classifier->getSolution();

                    w = sol.w;
                    margin = sol.margin;
                    svcount = sol.svs;
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
                    this->classifier->setSamples(this->samples);

                    if (!this->classifier->train()) {   /*training failed, remove this option*/
                        gtmp = nullptr;
                        if (this->verbose > 1) std::cout << "Training failed!\n";
                        return;
                    }

                    if (this->choice_shape == 2)
                        gtmp->value = margin * mltk::stats::distCenters(*this->samples, -1);
                    else
                        gtmp->value = margin;
                    gtmp->rgamma = margin;
                    gtmp->train = 1;

                    this->contprojtrained++;

                    /*verifica se realmente eh o melhor ou se jah chegou na profundidade desejada*/
                    if (gtmp->value < this->heap->getElements()[1]->value ||
                        gtmp->level == this->depth) {   /*reinsert the node into heap*/
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
                            this->cv->initial_error += 100-validation::kfold(*this->samples, *this->classifier,
                                                                         this->cv->fold, this->cv->seed[i], 0).accuracy;
                        kfolderror = this->cv->initial_error / this->cv->qtde;
                    } else if (level % this->cv->jump == 0) {
                        for (this->cv->actual_error = 0, i = 0; i < this->cv->qtde; i++)
                            this->cv->actual_error += 100-validation::kfold(*this->samples, *this->classifier,
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
                    std::cout << this->startdim << " " << level << std::endl;
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
                this->stmp_partial.reset();
                this->stmp_partial->copy(*this->samples);
                if (look_ahead_depth > 0) {
                    if (isPrimal) {
                        /*criar um w de manutencao pra volta do look_ahead*/
                        w_manut.resize(dim);
                        w_manut = w;
                    }

                    /*get new look ahead margin*/
                    g_margin = this->lookAhead(ofnames, w, level);

                    if (isPrimal) {
                        w = w_manut;
                    }
                }
                /*cut heap based on its level and look ahead margin*/
                //this->heap->cut(this->hash, lool, cut, g_margin, this->verbose);

                if (this->verbose > 2) {
                    std::cout << " (" << this->heap->getSize() << ")\t";
                    for (j = 0; j < level; j++)
                        std::cout << ofnames[j] << " ";
                    std::cout << "\n";
                }
            }

        }

        /*----------------------------------------------------------*
        * look ahead for pruning value                             *
        *----------------------------------------------------------*/

        template<typename T>
        double AOS<T>::lookAhead(std::vector<int> fnames_orig, std::vector<double> w_orig, int level_orig) {
            size_t i = 0, j = 0;
            int level = level_orig;
            int svcount = 0;
            int count = 0;
            int feat = 0;
            bool isPrimal = this->classifier->getFormulationString() == "Primal";
            std::vector<int> features((unsigned int) look_ahead_depth + 1);
            double min = 0;
            double margin = 0;
            double g_margin = 0;
            std::vector<double> w = w_orig;
            std::vector<double> novo_w;
            auto stmp = this->samples;
            select_gamma *gtmp = nullptr;
            double distcents = 0;
            Solution sol;

            while (true) {
                /*stopping criterion*/
                if (count == this->look_ahead_depth || count == this->samples->dim() - 1 ||
                        stmp->dim() == this->samples->dim() - this->depth || level == this->depth)
                    break;
                if (this->choice_shape == 2) {
                    /*selecting one feature with least w / dist. centers*/
                    min = fabs(w[0]) / mltk::stats::distCenters(*stmp, stmp->getFeaturesNames()[0]);
                    feat = stmp->getFeaturesNames()[0];
                    for (i = 1; i < stmp->dim(); i++) {
                        distcents = mltk::stats::distCenters(*stmp, stmp->getFeaturesNames()[i]);
                        if (fabs(w[i]) / distcents < min) {
                            min = fabs(w[i]) / distcents;
                            feat = (int) mltk::stats::distCenters(*stmp, stmp->getFeaturesNames()[i]);
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
                    size_t dim = this->samples->dim() - 1;
                    std::vector<int> fnames = this->samples->getFeaturesNames();
                    novo_w.resize(dim - 1);
                    for (i = 0, j = 0; j < dim; ++j)
                        if (fnames[j] != feat)
                            novo_w[i++] = w[j];
                }

                /*saving removed feature name*/
                features[count] = feat;

                /*removing old data sample*/
                if (*stmp != *this->samples) stmp.reset();

                /*get temp data struct*/
                stmp->removeFeatures(features);

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
                gtmp->pgamma = sol.margin;
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
                    sol.margin = gtmp->pgamma;
                    sol.bias = gtmp->bias;
                    sol.w = novo_w;
                    this->classifier->setSolution(sol);
                    if (!this->classifier->train()) {
                        if (this->verbose) std::cout << "Training failed!\n";
                        break;
                    }

                    gtmp->w = this->classifier->getSolutionRef()->w;
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
                    w = this->classifier->getSolutionRef()->w;
                }

                /*increment*/
                level++;
                count++;
            }

            w_orig = w;

            /*free stuff*/
            if (*stmp != *this->samples) stmp.reset();

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

        /***********************************************************
         *              HASH FUNCTIONS                             *
         ***********************************************************/

        /*----------------------------------------------------------*
        * creates a fresh new hash                                 *
        *----------------------------------------------------------*/

        template<typename T>
        AOS<T>::Hash::Hash(size_t length, size_t width) {
            size_t i;

            this->length = length;
            this->width = width;

            elements = new AOS<T>::select_gamma **[length];

            for (i = 0; i < length; i++) {
                elements[i] = new AOS<T>::select_gamma *[width];
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
                if (*elmt == *(this->elements[index][i])) {
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
                    delete elmt;

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

        template<typename T>
        void AOS<T>::Hash::set_null(AOS<T>::select_gamma *elmt) {
            unsigned int i = 0, j = 0;
            int index = 0;
            double func = 0;

            /*error check*/
            if (elmt == nullptr || this->elements == nullptr) return;

            /*hashing function*/
            for (i = 0; i < elmt->level; ++i)
                func += std::pow(elmt->fnames[i], 2);

            index = (unsigned int) fmod(func, this->length);

            /*finding it*/
            i = 0;
            while (i < this->width && this->elements[index][i] != nullptr) {
                /*check equality between nodes*/
                if (*elmt == *(this->elements[index][i])) {
                    /*shift elements*/
                    j = i + 1;
                    while (j < this->width && this->elements[index][j] != nullptr) {
                        this->elements[index][j - 1] = this->elements[index][j];
                        j++;
                    }
                    /*setting last element as null*/
                    this->elements[index][j - 1] = nullptr;

                    break;
                }
                i++;
            }
        }

        /*----------------------------------------------------------*
        * print hash table                                         *
        *----------------------------------------------------------*/

        template<typename T>
        void AOS<T>::Hash::print(int dim) {
            size_t i = 0, j = 0, k = 0, d = 0;
            int cont = 0;

            for (i = 0; i < this->length; ++i)
                for (j = 0; j < this->width; ++j)
                    if (this->elements[i][j] != nullptr) {
                        cont++;
                    }
            std::cout << "Cont = " << cont << "\n";
            if (dim <= 10)
                for (d = 0; d < dim; d++) {
                    for (i = 0; i < this->length; ++i)
                        for (j = 0; j < this->width; ++j)
                            if (this->elements[i][j] != nullptr)
                                if (this->elements[i][j]->level == d) {
                                    for (k = 0; k < (this->elements[i][j]->level) - 1; ++k)
                                        std::cout << this->elements[i][j]->fnames[k] << ",";
                                    std::cout << this->elements[i][j]->fnames[k] << "\n";
                                }
                    std::cout << "\n";
                }
        }

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
            std::cout << this->size << std::endl;
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
        void AOS<T>::Heap::cut(std::unique_ptr<AOS<T>::Hash> hash, int levelat, int cut, double g_margin, int verbose) {
            size_t i = 0, count = 0;
            AOS<T>::select_gamma *curr = nullptr;

            for (i = this->size; i > 0; --i) {
                curr = this->elements[i];
                if (curr->level < levelat)
                    if (curr->value < (1 - NUM_ERROR_EPS) * g_margin || levelat - curr->level >= cut) {
                        /*errase it from hash*/
                        delete curr;

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
            size_t i;

            for (i = 0; i < this->size; i++) {
                delete[] this->elements[i];
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

        template
        class AOS<int>;

        template
        class AOS<double>;

        template
        class AOS<float>;

        template
        class AOS<int8_t>;

        template
        class AOS<char>;

        template
        class AOS<long long int>;

        template
        class AOS<short int>;

        template
        class AOS<long double>;

        template
        class AOS<unsigned char>;

        template
        class AOS<unsigned int>;

        template
        class AOS<unsigned short int>;
    }
}