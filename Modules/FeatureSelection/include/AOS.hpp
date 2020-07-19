//
// Created by Mateus Coutinho Mari on 8/20/2018.
//

#ifndef UFJF_MLTK_AOS_H
#define UFJF_MLTK_AOS_H

#include "FeatureSelection.hpp"
#include "../../Validation/include/Validation.hpp"

template < typename T >
class AOS: public FeatureSelection< T >{
public:
    struct select_weight
    {
        int fname;
        int indice;
        double w;
        double val;
        double pmargin;
        double radius;
        double dcents;
        double golub;
        double fisher;

        bool operator==(AOS::select_weight other) const;
    };

    struct select_gamma
    {
        std::vector<int> fnames;
        int level;
        int sv;
        int train;
        double value; /*valor usado como criterio de escolha*/
        double pgamma; /*projected gamma*/
        double rgamma; /*real gamma p/ display*/
        double radius; /*raio*/
        double dcents; /*distancia entre os centros*/
        double golub; /*golub - estatistica*/
        double fisher; /*fisher - estatistica*/
        std::vector<double> w;
        double bias;

        bool operator==(AOS::select_gamma other) const;
    };


    class Hash {
        private:
            size_t length = 0, width = 0, conthash = 0;
            AOS::select_gamma ***elements;

        public:
            Hash(size_t length, size_t  width);

            bool add(AOS::select_gamma *elmt);

            void set_null(AOS::select_gamma *elmt);

            void print(int dim);

            size_t getLength() const;

            void setLength(size_t length);

            unsigned int getWidth() const;

            void setWidth(size_t width);

            unsigned int getConthash() const;

            void setConthash(size_t conthash);

            ~Hash();
    };

    class Heap{
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

            AOS::select_gamma* pop();

            void print();

            size_t projected();

            void percolate(size_t i);

            void cut(std::unique_ptr<AOS::Hash> hash, int levelat, int cut, double g_margin, int verbose);

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

    std::string filename;
    std::unique_ptr<Heap> heap;
    std::unique_ptr<Hash> hash;
    std::shared_ptr<Data< T > > stmp_partial;

public:
    AOS(std::shared_ptr<Data< T > > samples = nullptr, Classifier< T > *classifier = nullptr, typename Validation< T >::CrossValidation *cv = nullptr,
        int breadth = 0, int depth = 0, double bonus = 0, int cut = 0, int look_ahead_depth = 0, int skip = 0,
        int startover = 0, double g_margin = 0, bool doleave_oo = 0, int sorting_shape = 0, int choice_shape = 0, int verbose = 0);
    std::shared_ptr<Data< T > > selectFeatures() override;
    void mainLoop();
    double lookAhead(std::vector<int> fnames_orig, std::vector<double> w_orig, int level_orig);
};


#endif //UFJF_MLTK_AOS_H
