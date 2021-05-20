/*
                       _oo0oo_
                      o8888888o
                      88" . "88
                      (| -_- |)
                      0\  =  /0
                    ___/`---'\___
                  .' \\|     |// '.
                 / \\|||  :  |||// \
                / _||||| -:- |||||- \
               |   | \\\  -  /// |   |
               | \_|  ''\---/''  |_/ |
               \  .-\__  '-'  ___/-. /
             ___'. .'  /--.--\  `. .'___
          ."" '<  `.___\_<|>_/___.' >' "".
         | | :  `- \`.;`\ _ /`;.`/ - ` : | |
         \  \ `_.   \_ __\ /__ _/   .-` /  /
     =====`-.____`.___ \_____/___.-`___.-'=====
                       `=---='


     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

           A Buddha statue to bless your
               code to be bug free
*/

/*! Data manipulation class
   \file Data.hpp
   \author Mateus Coutinho Marim
*/

#pragma once
#ifndef DATA_HPP_INCLUDED
#define DATA_HPP_INCLUDED
#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_CORE
    #define DLLData __declspec(dllexport)
  #else
    #define DLLData __declspec(dllimport)
  #endif
#else
  #ifdef BUILDING_CORE
      #define DLLData __attribute__ ((visibility ("default")))
  #else
      #define DLLData
  #endif
#endif

#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <memory>
#include <random>
#include <set>

#include "Point.hpp"
#include "Statistics.hpp"
#include "Utils.hpp"

namespace mltk{
    static const std::vector<std::string> types {"data", "csv", "arff", "txt", "plt"};
    enum  Type {TYPE_INVALID = -1, TYPE_DATA = 0, TYPE_CSV = 1, TYPE_ARFF = 2, TYPE_TXT = 3};

    template < typename T > 
    class Statistics;

    template < typename T > 
    class Data;

    template < class T > using DataPointer = std::shared_ptr<mltk::Data< T > >;
    template < class T > using SamplePointer = std::shared_ptr<mltk::Point< T > >;
    template < class T > using SampleIterator = typename std::vector<SamplePointer< T > >::iterator;
    template < class T > using ConstSampleIterator = typename std::vector<SamplePointer< T > >::const_iterator;

    /**
     * \brief Wrapper for the dataset data.
     */
    template < typename T = double >
    class DLLData Data {
        // Associations
        // Attributes
    private :
        std::string dataset_name{""};
        /// Set of points.
        std::vector<SamplePointer<T> > m_points;
        /// Features names.
        std::vector<int> fnames;
        /// Points indexes.
        std::vector<int> index;
        /// Names of the classes in the dataset.
        std::vector<std::string> class_names;
        /// Numeric values of the classes.
        std::vector<int> m_classes;
        /// Frequency of each class in the dataset.
        std::vector<size_t> class_distribution;
        /// Number of points in the dataset.
        size_t m_size = 0;
        /// Dataset points dimension.
        size_t m_dim = 0;
        /// Time multiplier
        double time_mult = 0.0;
        /// Positive and negative classes. (1, -1 are the default classes)
        std::string pos_class = std::string("1"), neg_class = std::string("-1");
        /// Verify if there's some data loaded.
        bool is_empty = true;
        /// Boolean informing if the classes are at the end of the dataset.
        bool atEnd = false;
        /// Verify if the data is normalized.
        bool normalized = false;
        /// Verify if classes distribution is computed
        bool cdist_computed = false;
        /// Dataset type.
        std::string type = "Classification";
    public:
        /**
         * \brief Returns the type of the dataset.
         * \return std::string
         **/
        const std::string &getType() const;

    private:
        // Private Operations
        /**
         * \brief Process a new class being added to the dataset.
         * \param item new class being added.
         * \return Id of the new class.
         **/
        int process_class(std::string& item);
        /**
         * \brief Returns the type of the file.
         * \param file (???) Path to the file.
         * \return int
         */
        Type identifyFileType (std::string file);
        /**
         * \brief Load a dataset from a csv file.
         * \param path (???) Path to csv file.
         * \return bool
         */
        bool load_csv (const std::string& path);
        /**
         * \brief Load a dataset from a arff file.
         * \param path (???) Path to arff file.
         * \return bool
         */
        bool load_arff (const std::string& path);
        /**
         * \brief Load a dataset from a data file.
         * \param path (???) Path to data file.
         * \return bool
         */
        bool load_data (const std::string& path);
        /**
         * \brief Load a dataset from a txt file.
         * \param path (???) Path to txt file.
         * \return bool
         */
        bool load_txt (const std::string& path);

        std::string discover_dataset_name(const std::string& path);

    public :
        void setType(const std::string &type);

        Data() = default;
        Data(const Data<T>& other);
        Data(const std::string &dataset, bool atEnd);
        /**
         * \brief Data constructor to load a dataset from a file.
         * \param dataset (???) Path to the dataset to be loaded.
         * \param pos_class String representing the positive class on the dataset.
         * \param neg_class String representing the negative class on the dataset.
         */
        explicit Data (const char* dataset, const char* datasetType, const char* pos_class = "1", const char* neg_class = "-1");
        /**
         * \brief Constructor for instantiation with an initial size and dimension.
         * \param size The initial size of the data.
         * \param dim The initial dimension of the data.
         * \param val The default value for the initialization, if not given, it'll call the default constructor of T.
         */
        Data (size_t size, size_t dim, T val = T());
        /**
         * \brief Constructor for empty data.
         * \param pos_class String representing the positive class on the dataset.
         * \param neg_class String representing the negative class on the dataset.
         */
        explicit Data (const char* dataset);

        /*********************************************
         *               Getters                     *
         *********************************************/
        /**
        * \brief Inform if the dataset is used for classification.
        **/
        [[nodiscard]] bool isClassification() const { return (type == "Classification" || type == "MultiClassification" || type == "BinClassification");}
        [[nodiscard]] std::string name() const{ return dataset_name; }
        /**
         * \brief Returns the size of the dataset.
         * \return int
         */
        [[nodiscard]] size_t size() const{ return m_size;};
        /**
         * \brief Returns the dimension of the dataset.
         * \return int
         */
        [[nodiscard]] size_t dim() const{ return (!m_points.empty()) ? m_points[0]->size() : 0; }
        /**
         * \brief Returns a shared pointer to the vector of Points of the sample.
         * \return std::vector<std::shared_ptr<Point< T > > >
         */
        std::vector<std::shared_ptr<Point< T > > > points ();
        /**
         * \brief Returns a shared pointer to the vector of Points of the sample.
         * \return std::vector<std::shared_ptr<Point< T > > >
         */
        std::vector<std::shared_ptr<Point< T > > > points () const;
        /**
         * \brief Returns a vector containing the numeric values of the classes.
         * \return std::vector<int>
         **/
        [[nodiscard]] const std::vector<int> classes() const;
        /**
         * \brief Returns a shared pointer to the point with the given index.
         * \param index    Position of a point in the points array.
         * \return std::vector<Point< T > >
         */
        PointPointer<T> point (int index) const;
        Point< T > getFeature(int index) const;
        [[nodiscard]] Point< double > getLabels() const;
        /**
         * \brief Returns a vector containing the frequency of the classes.
         * \return std::vector<size_t>
         **/
        [[nodiscard]] std::vector<size_t> classesDistribution() const ;
        /**
         * \brief Returns a vector containing the name of the classes.
         * \return std::vector<std::string>
         **/
        [[nodiscard]] std::vector<std::string> classesNames() const;
        /**
         * \brief Returns the features names.
         * \return std::vector<int>
         */
        [[nodiscard]] std::vector<int> getFeaturesNames() const;
        /**
         * \brief Returns the vector of indexes.
         * \return std::vector<int>
         */
        [[nodiscard]] std::vector<int> getIndex() const;
        /**
         * \brief Return the time multiplier.
         * \return double
         */
        [[nodiscard]] double getTime_mult() const;

        /*********************************************
         *               Setters                     *
         *********************************************/
        void setName(const std::string& name){ this->dataset_name = name; }
        inline void setClassesNames(const std::vector<std::string> class_names){ this->class_names = class_names; }
        /**
         * \brief Set the classes to use in the dataset.
         * \param classes Vector of classes that will be in the dataset.
         **/
        void setClasses(const std::vector<int> &classes);
        /**
         * \brief Inform if the classes of the dataset to be loaded are at begining or end of the attributes.
         * \param atEnd Boolean informing if classes are at end.
         **/
        void setClassesAtEnd(bool atEnd){ this->atEnd = atEnd; }
        /**
         * \brief setPoint Set the point in a position of the data.
         * \param index (???) Index of the point that will be set.
         * \param p (???) Point to be set.
         */
        void setPoint (int index, std::shared_ptr<Point< T > > p);
        /**
         * \brief setFeaturesNames Set the name of the features of the data.
         * \param fnames (???) Name of the features.
         */
        void setFeaturesNames(const std::vector<int>& fnames);
        /**
         * \brief Set the index vector for the data.
         * \param index Index vector.
         */
        void setIndex(std::vector<int> index);
        /**
         * \brief setDim Set the dimension of the points.
         * \param dim Dimension to be set.
         */
        void setDim(size_t dim);

        /*********************************************
         *              Other operations             *
         *********************************************/
        
        /**
         * \brief Compute the frequency of each class in the dataset.
         **/
        void computeClassesDistribution();
        /**
         * \brief Shuffle the data with a given seed
         * \param seed Seed given for randomization
         **/
        void shuffle(const size_t& seed = 42);
        /**
         * \brief Load a dataset from a file.
         * \param file (???) Path to dataset file.
         * \return bool
         */
        bool load (const std::string& file, bool _atEnd=false);
        /**
         * \brief write Write the data to a file with the given extention.
         * \param fname Name of the file.
         * \param ext   Extention of the file.
         */
        void write(const std::string& fname, std::string ext);
        /**
         * \brief Returns if there's a dataset loaded.
         * \return bool
         */
        bool isEmpty () const;
        /**
         * \brief Returns if the dataset is normalized.
         * \return bool
         */
        bool isNormalized() const{ return normalized; };
        /**
         * \brief clear Clear the data.
         */
        void clear();
        /**
         * \brief Makes a deep copy from another data object.
         * \return Data
         */
        void classesCopy (const Data< T > &_data, std::vector<int> &classes);
        /**
         * \brief Returns a copy of itself.
         * \return Data
         */
        mltk::Data<T> copy() const;
        /**
         * \brief Makes a deep copy from another data object.
         * \return Data
         */
        void copy (const Data< T > &_data);
        /**
         * \brief Returns a copy of the data with zero points.
         * \return Data
         */
        void copyZero (const Data< T >& other);
        std::vector< Data< T > > splitByClasses();
        std::vector< Data< T > > splitSample(const std::size_t &split_size, bool stratified = true, size_t seed = 0);
        Data< T > selectFeatures(std::vector<size_t> feats);
        Data< T > sampling(const size_t& samp_size, bool with_replacement = true, const int &seed=0);
        void apply(std::function<void(mltk::PointPointer<T> point)> f);
        /**
         * \brief Merge one dataset with another.
         * \param data (???) Dataset to be joined.
         * \return bool
         */
        void join(const Data< T >& data);
        /**
         * \brief Insert a point to the data from another sample.
         * \param sample (???) Sample with the point to be added.
         * \param id (???) Index of the point to be added.
         * \return bool
         */
        bool insertPoint (Data< T > sample, int id);
        /**
         * \brief Insert a point to the end of points vector.
         * \param p  Point to be inserted.
         * \return bool
         */
        bool insertPoint (std::shared_ptr<Point< T > > p);
        bool insertPoint (Point< T > &p);
        /**
         * \brief Remove several points from the sample.
         * \param ids (???) Ids of the points to be removed (must be sorted).
         * \return booleans informing which points were removed succesfully.
         */
        std::vector<bool> removePoints (std::vector<int> ids);
        /**
         * \brief Remove a point from the data.
         * \param pid (???) Index of the point to be removed.
         * \return bool
         */
        bool removePoint (int pid);
        /**
         * @brief insertFeatures Returns Data object with only features in array.
         * @param ins_feat (???) Array with features that will be in the Data object.
         * @return Data If the object is empty something wrong happened.
         */
        Data< T >* insertFeatures(std::vector<int> ins_feat);
        /**
         * \brief Remove several features from the sample.
         * \param feats (???) Names of the features to be removed (must be sorted).
         * \return boolean informing if all features were succesfully removed.
         */
        bool removeFeatures (std::vector<int> feats);
        bool updatePointValue(const size_t &idx, double value);
        /**
         * \brief Change the x vector of a sample.
         * \param index (???) Indexes of the change to be made.
         * \return void
         */
        void changeXVector(std::vector<int> index);
        /**
         * \brief normalize Normalize the dataset using a Lp-norm.
         * \param p Norm to be utilized.
         */
        void normalize(double p = 2);
        /**
         * \brief Reset the index vector.
         */
        void resetIndex();

        /*********************************************
         *  Overloaded operators for the Data class. *
         *********************************************/

        SampleIterator<T> begin() { return m_points.begin(); }
        
        SampleIterator<T> end() { return m_points.end(); }

        ConstSampleIterator<T>  begin() const { return m_points.cbegin(); }

        ConstSampleIterator<T> end() const { return m_points.cend(); }

        std::shared_ptr<Point< T > > operator[](size_t i) const {return m_points[i];}

        std::shared_ptr<Point< T > > & operator[](size_t i) {return m_points[i];}

        Point< T > operator()(size_t i) const {return *(m_points[i]);}
        Point< T >& operator()(size_t i) {return *m_points[i];}

        Data< T >& operator=(const Data< T >&);

        bool operator==(const Data< T > &rhs) const;

        bool operator!=(const Data< T > &rhs) const;

        template< typename U >
        friend std::ostream &operator<<( std::ostream &output, const Data< U > &data );

        ~Data();
    };

    template < typename T, typename... Types >
    DataPointer< T > make_data(Types... args) {
        return std::make_shared< Data < T > >(args...);
    }

    template < typename T >
    std::ostream &operator<<( std::ostream &output, const Data< T > &data ){
        for(auto p : data.m_points){
            output << *p << std::endl;
        }

        return output;
    }
}

#endif