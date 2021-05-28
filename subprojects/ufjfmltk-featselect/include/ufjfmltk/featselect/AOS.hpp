//
// Created by Mateus Coutinho Mari on 8/20/2018.
//

#ifndef UFJF_MLTK_AOS_H
#define UFJF_MLTK_AOS_H

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
    }
}
#endif //UFJF_MLTK_AOS_H
