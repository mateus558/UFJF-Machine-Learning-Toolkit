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
#include <utility>
#include <cmath>
#include <vector>
#include <cstring>
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
    class Data {
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
        Data< T > selectFeatures(std::vector<size_t> feats, int size=-1);
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
        Data< T > insertFeatures(std::vector<int> ins_feat);
        /**
         * \brief Remove several features from the sample.
         * \param feats (???) Names of the features to be removed (must be sorted).
         * \return boolean informing if all features were succesfully removed.
         */
        Data<T> removeFeatures (std::vector<int> feats, int fsize) const;
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

    template<typename T>
    mltk::Data< T >::Data(const char* dataset){
        if(!load(std::string(dataset))){
            std::cerr << "Couldn't read the dataset." << std::endl;
        }
    }

    template<typename T>
    Data<T>::Data(const std::string &dataset, bool atEnd): atEnd(atEnd) {
        if(!load(std::string(dataset), atEnd)){
            std::cerr << "Couldn't read the dataset." << std::endl;
        }
    }

    template<typename T>
    mltk::Data<T>::Data(size_t size, size_t dim, T val) {
        size_t i;

        this->m_points.resize(size);
        this->fnames.resize(dim);
        this->index.resize(size);

        for(i = 0; i < size; i++){
            this->m_points[i] = std::make_shared<Point< T > >(dim, val);
            this->m_points[i]->Id() = i+1;
            this->m_points[i]->Y() = 0;
        }

        std::iota(fnames.begin(), fnames.end(), 1);
        std::iota(index.begin(), index.end(), 0);
        this->m_size = size;
        this->m_dim = dim;
        if(size > 0) this->is_empty = false;
    }

    template<typename T>
    mltk::Data< T >::Data(const char* dataset, const char* datasetType, const char* pos_class, const char* neg_class){
        this->pos_class = std::string(pos_class);
        this->neg_class = std::string(neg_class);
        this->type = std::string(datasetType);

        if(!load(std::string(dataset))) {
            std::cerr << "Couldn't read the dataset." << std::endl;
        }
    }

    template<typename T>
    Type mltk::Data< T >::identifyFileType(std::string file){
        size_t l , k, i, j, tsize = types.size();

        for(k = file.size() - 1; (k >= 0 && file[k] != '.') ; k--){
            for(i = 0; i < tsize; ++i){
                if(file[k] == types[i][types[i].size()-1]){
                    for(j = types[i].size() - 1, l = 0; (j >= 0 && file[k-l] != '.') ; j--, l++){
                        if(j == 0 && types[i][j] == file[k-l]){
                            switch(i){
                                case 0:
                                    return Type::TYPE_DATA;
                                    break;
                                case 1:
                                    return Type::TYPE_CSV;
                                    break;
                                case 2:
                                    return Type::TYPE_ARFF;
                                    break;
                                case 3:
                                    return Type::TYPE_TXT;
                                    break;
                                default:
                                    return Type::TYPE_INVALID;
                                    break;
                            }
                        }else if(types[i][j] != file[k-l]){
                            break;
                        }
                    }
                }
            }
        }

        return Type::TYPE_INVALID;
    }

    template<typename T>
    bool mltk::Data< T >::load(const std::string& file, bool _atEnd){
        Type t = identifyFileType(file);
        this->dataset_name = discover_dataset_name(file);
        this->cdist_computed = true;

        this->atEnd = _atEnd;

        switch (t) {
            case TYPE_ARFF:
                return load_arff(file);
            case TYPE_CSV:
                return load_csv(file);
            case TYPE_DATA:
                return load_data(file);
            case TYPE_TXT:
                return load_txt(file);
            default:
                std::cerr << "Invalid file type." << std::endl;
                return false;
        }
        fnames.assign(this->dim(), 0);
        std::iota(fnames.begin(), fnames.end(), 1);
        return true;
    }

    template<typename T>
    bool mltk::Data< T >::load_csv(const std::string& path){
        std::ifstream input(path.c_str());
        std::stringstream ss;
        std::string str, item;
        int _dim, ldim, _size, ssize;
        char deli = ',';
        bool cond, flag;

        flag = false;
        _dim = ldim = _size = 0;

        if(!input){
            std::cout << "File could not be opened!" << std::endl;
            return false;
        }

        //Verify if the class is at the begining or at the end and error check
        while(std::getline(input, str)){
            ssize = str.size();
            _dim = -1;
            ss.str(str);

            //Define csv file delimitator
            if(_size == 0){
                for(int i = 0; i < ssize; ++i){
                    if(str[i] == ','){
                        deli = ',';
                        break;
                    }else if(str[i] == ';'){
                        deli = ';';
                        break;
                    }
                }
            }

            while(getline(ss, item, deli)){
                //check for invalid feature or class
                if(this->isClassification()) {
                    if (_dim == -1 && !flag) {
                        if (!((item == pos_class) || (item == neg_class))) {
                            flag = true;
                        }
                    } else if (ss.eof() && !flag) {
                        if (!((item == pos_class) || (item == neg_class))) {
                            flag = true;
                        }
                    }
                }
                _dim++;
            }

            if(ldim != _dim && ldim != 0){
                std::cerr << "All the samples must have the same dimension!" << std::endl;
                return false;
            }

            ldim = _dim;
            _size++;
            ss.clear();
        }

        input.clear();
        input.seekg(0, std::ios::beg);

        //initialize _dim and _size
        this->m_dim = _dim;
        this->m_size = _size;

        //reserve memory for fnames array and set feature names
        fnames.assign(_dim, 0);
        index.assign(_size, 0);
        std::iota(fnames.begin(), fnames.end(), 1);
        std::iota(index.begin(), index.end(), 0);

        //reserve memory for points array
        m_points.resize(_size);
        _size = 0;
        //Read sample (line) from file
        while(std::getline(input, str)){
            auto new_point = std::make_shared<Point< T > >();

            ss.str(str);
            _dim = -1;

            //reserve memory for x array
            new_point->X().resize(this->m_dim, 0.0);

            //Read features from line
            while(std::getline(ss, item, deli)){
                if(atEnd)
                    cond = !ss.eof();
                else
                    cond = _dim != -1;

                if(cond){
                    if(utils::is_number(item))
                        new_point->X()[(!atEnd) ? _dim : _dim + 1] = utils::atod(item.c_str());
                }else{
                    double c;

                    if(this->isClassification()) {
                        c = process_class(item);
                    }else{
                        c = utils::atod(item.c_str());
                    }
                    new_point->Y() = c;
                }
                _dim++;
            }

            m_points[_size++] = std::move(new_point);
            m_points[_size - 1]->Id() = _size;
            ss.clear();
        }

        is_empty = false;

        return true;
    }

    template<typename T>
    bool mltk::Data< T >::load_data(const std::string& path){
        std::ifstream input(path.c_str());
        std::string str, item, buffer;
        std::stringstream ss, ss1;
        int _dim, ldim, _size;
        double c;
        bool is_feature, type_detect = false;

        if(!input){
            std::cout << "File could not be opened!" << std::endl;
            return false;
        }
        _dim = ldim = _size = c = 0;
        //get dimension of the points and do error check
        while(std::getline(input, str)){
            _dim = -1;

            ss.str(str);
            ss.clear();

            while(std::getline(ss, item, ' ')){
                const char * pch = std::strchr(item.c_str(), ':');
                _dim++;
                /*if(_size > 0 && _dim < ldim && pch == nullptr){
                    std::cerr << "Error (line: " << _size << ", _dim: " << _dim << "): file isn't in the .data format." << std::endl;
                    return false;
                }*/

                ss1.str(item);
                ss1.clear();
                while(_size > 0 && std::getline(ss1, item, ':')) {
                    if(_dim == ldim && !type_detect){
                        if(strchr(item.c_str(), '.')){
                            this->type = "Regression";
                        }else{
                            this->type = "Classification";
                        }
                        type_detect = true;
                    }else if(_dim < ldim && !utils::is_number(item)){
                        std::clog << "Warning (line: " << _size << "): feature " << _dim << " is not a number." << std::endl;
                    }
                }
            }
            if(ldim != _dim && ldim != 0){
                std::cerr << "Error (line: " << _size << "): all the samples must have the same dimension! (_dim: " << _dim << ", last_dim: " << ldim << ")" << std::endl;
                return false;
            }

            ldim = _dim;
            _size++;
        }

        input.clear();
        input.seekg(0, std::ios::beg);

        //initialize _dim and _size
        this->m_dim = _dim;
        this->m_size = _size;

        //reserve memory for fnames array and set feature names
        fnames.assign(_dim, 0);
        index.assign(_size, 0);
        std::iota(fnames.begin(), fnames.end(), 1);
        std::iota(index.begin(), index.end(), 0);

        //reserve memory for points array
        m_points.resize(_size);
        _size = 0;

        //get lines from file
        while(std::getline(input, str)){
            auto new_point = std::make_shared<Point< T > >();

            ss.str(str);
            ss.clear();
            _dim = 0;
            new_point->X().resize(this->m_dim, 0.0);

            //Read features from line
            while(std::getline(ss, item, ' ')){
                const char * pch = std::strchr(item.c_str(), ':');
                if(!pch){
                    if(this->isClassification()) {
                        c = process_class(item);
                    }else{
                        c = utils::atod(item.c_str());
                    }
                    new_point->Y() = c;
                }
                //Verify if the class is at the beggining or at the end
                if(!ss.eof()){
                    is_feature = false; //Verify if it's including value or fname
                    ss1.str(item);
                    ss1.clear();
                    //Get feature name and value
                    while(std::getline(ss1, item, ':')){
                        if(!is_feature){
                            fnames[_dim] = utils::stoin(item);
                            is_feature = true;
                        }else{
                            if(utils::is_number(item)){
                                new_point->X()[_dim] = utils::atod(item.c_str());
                                _dim++;
                            }
                        }
                    }
                }
            }
            m_points[_size++] = std::move(new_point);
            m_points[_size - 1]->Id() = _size;
        }

        if(m_classes.size() == 2){
            type = "BinClassification";
        }else{
            type = "MultiClassification";
        }

        input.close();
        is_empty = false;
        return true;
    }

    template<typename T>
    bool mltk::Data< T >::load_arff(const std::string& path){
        std::ifstream input(path.c_str());
        std::istringstream ss;
        std::string str, item;
        int _dim, ldim, _size, c;
        bool atBegin, flag, cond;

        if(!input){
            std::cout << "File could not be opened! \nError: " << std::strerror(errno) << std::endl;
            return false;
        }

        _dim = ldim = _size = c = 0;
        atBegin = flag = cond = false;

        //Verify if the class is at the begining or at the end and error check
        while(std::getline(input, str)){
            _dim = 0;
            ss.str(str);

            while(std::getline(ss, item, ',')){
                //check for invalid feature or class
                auto found_class = std::find(this->class_names.begin(), this->class_names.end(), item);

                if(!utils::is_number(item) && (found_class != this->class_names.end())){
                    std::clog << "Warning: point[" << _size << "] " << _dim + 1 << " feature is not a number. (" << item << ")" << std::endl;
                    _dim--;
                }
                if(this->isClassification()) {
                    if (_dim == 0 && !flag) {
                        if (!((item == pos_class) || (item == neg_class))) {
                            //atEnd = true;
                            flag = true;
                        }
                    } else if (ss.eof() && !flag) {
                        if (!((item == pos_class) || (item == neg_class))) {
                            atBegin = true;
                            flag = true;
                        }
                    }
                }
                _dim++;
            }

            if(ldim != _dim && ldim != 0){
                std::cerr << "All the samples must have the same dimension!" << std::endl;
                return false;
            }

            ldim = _dim;
            _size++;
            ss.clear();
        }
        input.clear();
        input.seekg(0, std::ios::beg);

        //initialize _dim and _size
        _dim--;
        this->m_dim = _dim;
        this->m_size = _size;

        //reserve memory for fnames array and set feature names
        fnames.assign(_dim, 0);
        index.assign(_size, 0);
        std::iota(fnames.begin(), fnames.end(), 1);
        std::iota(index.begin(), index.end(), 0);

        //reserve memory for points array
        m_points.resize(_size);

        _size = 0;

        //Read line (sample) from file
        while(std::getline(input, str)){
            auto new_point = std::make_shared<Point< T > >();
            _dim = -1;
            ss.str(str);

            //reserve memory for features
            new_point->X().assign(this->m_dim, 0.0);

            //Read features from line
            while(std::getline(ss, item, ',')){
                if(atEnd)
                    cond = (!ss.eof() && atEnd);
                else
                    cond = _dim != 0;

                if(cond){
                    if(utils::is_number(item)){
                        new_point->X()[_dim + 1] = utils::atod(item.c_str());
                    }
                }else{
                    if(this->isClassification()){
                        c = process_class(item);
                    }
                    new_point->Y() = c;
                }
                _dim++;
            }

            m_points[_size++] = std::move(new_point);
            m_points[_size - 1]->Id() = _size;
            ss.clear();
        }

        is_empty = false;

        return true;
    }

    template<typename T>
    bool mltk::Data< T >::load_txt(const std::string& path){
        std::ifstream input(path.c_str());
        std::istringstream ss;
        std::string str, item;
        int n, _dim = 0, n1, d, _size;

        if(!input){
            std::cout << "File could not be opened!" << std::endl;
            return false;
        }

        n1 = d = _size = 0;

        //error check
        while(std::getline(input, str)){
            ss.str(str);
            n1 = 0;
            _dim = 0;

            while(std::getline(ss, item, ' ')){
                if(n1 >= 2){
                    _dim++;
                }
                n1++;
            }

            if(_size != 0 && _dim != d ){
                std::cerr << _dim << " " << this->m_dim << std::endl;
                std::cerr << "All the samples must have the same dimension!" << std::endl;
                return false;
            }

            d = _dim;
            _size++;
            ss.clear();
        }

        //Initialize _size and _dim
        this->m_size = _size;
        this->m_dim = _dim;

        //Set features names
        fnames.assign(_dim, 0);
        index.assign(_size, 0);
        std::iota(fnames.begin(), fnames.end(), 1);
        std::iota(index.begin(), index.end(), 0);

        input.clear();
        input.seekg(0, std::ios::beg);

        //Allocate memory for points array
        m_points.resize(_size);

        _size = 0;

        //get line from file (sample)
        while(std::getline(input, str)){
            auto new_point = std::make_shared<Point< T > >();

            //Allocate memory for features
            new_point->X().resize(_dim, 0.0);
            ss.str(str);
            n = 0;

            //read features from line
            while(std::getline(ss, item, ' ')){
                if(n >= 2){
                    if(utils::is_number(item))
                        new_point->X()[n - 2] = utils::atod(item.c_str());
                    else{ std::clog << "Warning: point[" << _size << "] " << n - 2 << " feature is not a number." << std::endl; }
                    new_point->Y() = 0;
                }else if(n == 1){
                    new_point->Id() = utils::atod(item.c_str());
                }
                n++;
            }

            m_points[_size++] = std::move(new_point);
            m_points[_size - 1]->Id() = _size;
            ss.clear();
        }

        is_empty = false;

        return true;
    }

    template<typename T>
    int mltk::Data<T>::process_class(std::string& item) {
        int c = 0;
        auto class_name_it = std::find(this->class_names.begin(), this->class_names.end(), item);

        item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());
        if(item.empty()) return std::numeric_limits<int>::infinity();

        if(class_name_it == class_names.end()){
            this->class_names.push_back(item);
            auto has_mone = std::find(class_names.begin(), class_names.end(), "-1");
            if(has_mone == class_names.end()){
                if(utils::is_number(item)) {
                    c = std::stoi(item);
                }else{
                    auto pos = std::find(class_names.begin(), class_names.end(), item);
                    c = (int)(pos - this->class_names.begin())+1;
                }
            }else{
                c = 1;
            }
            if(item == "-1"){
                c = -1;
            }

        }else{
            if(utils::is_number(item)) {
                c = std::stoi(item);
            }else{
                c = (int)(class_name_it - this->class_names.begin())+1;
            }
        }
        auto class_it = std::find(this->m_classes.begin(), this->m_classes.end(), c);
        if(class_it == this->m_classes.end()){
            this->m_classes.push_back(c);
            this->class_distribution.push_back(0);
        }
        auto it_pos = std::find_if(this->m_classes.begin(), this->m_classes.end(), [&c](const int &_class){
            return (_class == c);
        });
        this->class_distribution[(it_pos-this->m_classes.begin())]++;

        return c;
    }

    template<typename T>
    bool mltk::Data< T >::removePoint(int pid){
        int i;

        if(m_size == 1){ std::cout << "Error: RemovePoint, only one point left\n"; return false; }
        //Ids bound verification
        if(pid > m_points[m_size - 1]->Id() || pid <= 0) return false;
        int y;
        if(!index.empty()){
            index.resize(m_size);

            for(i = 0; i < m_size; ++i){
                if(i >= pid){
                    index[i-1] = index[i] - 1;
                }
            }
        }

        //Find the point by its id and erase it
        for(i = 0; i < m_size; i++){
            if(m_points[i]->Id() == pid){
                y = m_points[i]->Y();
                m_points.erase(m_points.begin() + i);
                break;
            }
        }
        m_size--;
        auto pos = std::find(m_classes.begin(), m_classes.end(), y) - m_classes.begin();
        class_distribution[pos]--;
        return true;
    }

    template<typename T>
    void mltk::Data< T >::write(const std::string& fname, std::string ext){
        int i, j;
        std::string path = fname + "." + ext;
        std::ofstream outstream(path.c_str(), std::ios::out);
        m_dim =(this->m_points.size()>0)?this->m_points[0]->size():0;

        if(!outstream.is_open()){
            std::cerr << "Can't write in file." << std::endl;
            return;
        }

        for(i = 0; i < m_size; i++){
            if(ext == "plt"){
                outstream << m_points[i]->Y() << " ";
                for(j = 0; j < m_dim-1; j++){
                    outstream << m_points[i]->X()[j] << " ";
                }
                outstream << m_points[i]->X()[j] << std::endl;
            }else if(ext == "data"){
                outstream << m_points[i]->Y() << " ";
                for(j = 0; j < m_dim-1; j++){
                    outstream << fnames[j] << ":" << m_points[i]->X()[j] << " ";
                }
                outstream << fnames[j] << ":" << m_points[i]->X()[j] << "\n";
            }else if(ext == "csv"){
                outstream << m_points[i]->Y() << ",";
                for(j = 0; j < m_dim-1; j++){
                    outstream << m_points[i]->X()[j] << ",";
                }
                outstream << m_points[i]->X()[j] << "\n";
            }
        }

        outstream.close();
    }

    template<typename T>
    std::vector<bool> mltk::Data< T >::removePoints(std::vector<int> ids){
        int idsize = ids.size(), i, j;
        bool save;
        std::shared_ptr<Point< T > > po;
        auto p = m_points.begin();
        std::vector<bool> notFound(idsize, true);

        std::sort(ids.begin(), ids.end());

        for(; p != m_points.end();){
            save = true;
            po = (*p);
            for(i = 0; i < idsize; i++){
                if(po->Id() == ids[i]){
                    save = false;
                    notFound[i] = false;
                    break;
                }
            }

            if(save) p++;
            else{
                p = m_points.erase(p);
                int c = po->Y();
                if(!index.empty()){
                    index.resize(m_size);

                    for(j = 0; j < m_size; ++j){
                        if(j >= ids[i]){
                            index[j-1] = index[j] - 1;
                        }
                    }
                }
                //Size verification.
                if(m_size == 1){ std::clog << "Error: RemovePoint, only one point left." << std::endl; break;}
                auto class_pos = std::find(m_classes.begin(), m_classes.end(), c) - m_classes.begin();
                class_distribution[class_pos]--;
                m_size--;
            }
        }

        return notFound;
    }

    template<typename T>
    mltk::Data< T > mltk::Data< T >::insertFeatures(std::vector<int> ins_feat){
        size_t i, j, s, offset = 0, fsize = ins_feat.size();
        bool saveflag = false;
        std::vector<int> new_fnames(fsize, 0);
        std::shared_ptr<Point< T > > p;
        auto *smout = new mltk::Data< T >;

        if(fsize == 0) return this->copy();
        sort(ins_feat.begin(), ins_feat.end());

        //error check
        if(fsize > m_dim){ std::cerr << "Error: InsertFeature, fsize(" << ins_feat.size() << ")>dim(" << m_dim << ")\n"; return *smout; }
        smout->setDim(fsize);

        //Copying information to new data array
        for(i = 0; i < m_size; i++){
            p = std::make_shared<Point< T > >();
            p->X().resize(fsize);
            p->Alpha() = m_points[i]->Alpha();
            p->Id() = m_points[i]->Id();
            p->Y() = m_points[i]->Y();

            //Copying features
            s = 0, offset = 0;
            for(j = 0; j < m_dim; j++){
                if(offset < fsize && fnames[j] == ins_feat[offset]){
                    saveflag = true;
                    offset++;
                }

                if(saveflag){
                    p->X()[s] = m_points[i]->X()[j];
                    new_fnames[s] = fnames[j];
                    s++;
                    saveflag = false;
                }
            }
            //error check
            if(s != fsize){
                std::cerr << "Error: Something went wrong on InsertFeature\n";
                std::cerr << "s = " << s << ", dim = " << m_dim << ", fsize = " << fsize << std::endl;
                smout->clear();
                return *smout;
            }
            smout->insertPoint(p);
        }
        smout->setFeaturesNames(new_fnames);

        return *smout;
    }

    template<typename T>
    void mltk::Data< T >::shuffle(const size_t &seed){
        std::mt19937 gen(seed);
        std::uniform_int_distribution<size_t> dist(0, m_size - 1);

        for(auto it = m_points.begin(); it != m_points.end(); it++){
            auto pos = m_points.begin() + dist(gen);
            auto temp = (*it)->Id();
            (*it)->Id() = (*pos)->Id();
            (*pos)->Id() = temp;
            std::iter_swap(it, pos);
        }
    }
    template < typename T >
    Data<T> Data<T>::removeFeatures(std::vector<int> feats, int fsize) const {
        auto data_copy = this->copy();
        size_t feats_size = (fsize==-1)?feats.size():fsize;
        std::vector<int> to_remove(feats.begin(), feats.begin()+feats_size);
        data_copy.removeFeatures(to_remove);
        return data_copy;
    }

    template<typename T>
    bool mltk::Data< T >::removeFeatures(std::vector<int> feats){
        size_t i, j, k, psize = m_points.size(), rsize = feats.size();
        typename std::vector< T >::iterator itr;
        std::vector<int>::iterator fitr;
        std::vector<bool> exist(rsize, true);

        if(feats.empty()) return true;

        if(fnames.size() == 1){
            std::cerr << "Error: RemoveFeature, only one feature left.\n";
            return false;
        }
        /*if(feats.size() >= fnames.size()){
            cerr << "Error: RemoveFeature, more or equal features to remove than exist.\n";
            return false;
        }*/

        //Sort feats for remove features easily
        sort(feats.begin(), feats.end());

        //Check the existence of the features to be removed
        for(i = 0; i < rsize; i++){
            for(j = 0; j < m_dim; j++){
                if((feats[i] > -1) && feats[i] == fnames[j]){
                    break;
                }
            }
            if(j == m_dim){
                exist[i] = false;
            }
        }

        //Remove features from each point
        for(i = 0; i < psize; i++){
            if(m_points[i] == nullptr) std::clog << "WARNING: point is null." << std::endl;

            // Iterate through the point features
            for(itr = m_points[i]->X().begin(),k = 0, j = 0; itr != m_points[i]->X().end();){
                while(k < rsize && !exist[k]) k++; // go to next existent feature
                if(k == rsize) break;              // Verify if is in the end of the feats vector

                // Feature to remove found, remove it from the point and go to the next feat to remove
                if(fnames[j] == feats[k]){
                    itr = m_points[i]->X().erase(itr);
                    k++;
                }else{
                    itr++;
                }
                j++;
            }
        }

        //remove names of removed features
        for(k = 0; k < rsize; k++){
            for(fitr = fnames.begin(); fitr != fnames.end();){
                if((*fitr) == feats[k]){
                    fitr = fnames.erase(fitr);
                    m_dim--;
                    break;
                }else{
                    fitr++;
                }
            }
        }

        return true;
    }

    template<typename T>
    bool mltk::Data< T >::insertPoint(mltk::Data< T > sample, int _index){
        if(_index > sample.size() - 1){
            std::cerr << "Index out of bounds. (insertPoint)" << std::endl;
            return false;
        }

        insertPoint(sample.point(_index));

        return true;
    }

    template<typename T>
    bool mltk::Data< T >::insertPoint(std::shared_ptr<Point< T > > p){
        //Dimension verification
        if(m_size > 0 && int(p->X().size()) > this->dim()){
            std::cerr << "Point with dimension different from the data. (insertPoint)" << std::endl;
            std::cerr << "Point dim = " << p->X().size() << " dim = " << m_dim << std::endl;
            return false;
        }

        if(p->size() > 0) {
            this->fnames = std::vector<int>(p->size(), 0);
            std::iota(this->fnames.begin(), this->fnames.end(), 1);
        }
        //Insert the point p at the end of the points vector
        m_points.insert(m_points.end(), p);
        m_size++;
        if(is_empty) is_empty = false;

        if(this->isClassification()){
            auto class_pos = std::find(this->m_classes.begin(), this->m_classes.end(), p->Y());

            if(class_pos == this->m_classes.end()){
                this->class_names.push_back(std::to_string(int(m_points[m_size - 1]->Y())));
                this->m_classes.push_back(m_points[m_size - 1]->Y());
                this->class_distribution.push_back(1);
            }else{
                this->class_distribution[int(class_pos - this->m_classes.begin())]++;
            }
        }
        //Give a new id to the point equal to the previous point id plus 1
        m_points[m_size - 1]->Id() = m_size;
        index.push_back(m_size - 1);

        return true;
    }

    template<typename T>
    bool mltk::Data< T >::insertPoint(Point< T > &p){
        return this->insertPoint(std::make_shared<Point< T > >(p));
    }

    template<typename T>
    void mltk::Data< T >::changeXVector(std::vector<int> _index){
        int i;
        std::vector<std::shared_ptr<Point< T > > > nPoints(m_size);

        //Copy features and classes of the points making the changes
        for(i = 0; i < m_size; i++){
            nPoints[i]->X() = m_points[_index[i]]->X();
            nPoints[i]->X() = m_points[_index[i]]->X();
        }

        //Save changes in the class
        m_points = nPoints;
    }

    template<typename T>
    PointPointer<T> mltk::Data< T >::point(int _index) const{
        return m_points[_index];
    }

    template<typename T>
    void mltk::Data< T >::setPoint(int _index, std::shared_ptr<Point< T > > p){
        m_points[_index] = p;
    }

    template<typename T>
    void mltk::Data< T >::classesCopy(const mltk::Data< T > &_data, std::vector<int> &classes){
        size_t _size = 0;
        std::set<int> _classes;
        for(auto c: classes){
            _classes.insert(c);
        }
        for(size_t i = 0; i < _data.size(); i++){
            if(_classes.find(_data[i]->Y()) != _classes.end()){
                this->m_points.push_back(std::make_shared<Point< T > >());
                size_t curr = this->m_points.size()-1;
                this->m_points[curr]->X() = _data[i]->X();
                this->m_points[curr]->Y() = _data[i]->Y();
                this->m_points[curr]->Alpha() = _data[i]->Alpha();
                this->m_points[curr]->Id() = _data[i]->Id();
                _size++;
            }
        }

        this->fnames = _data.getFeaturesNames();
        this->m_size = _size;
        this->m_classes = classes;
        this->m_dim = _data.dim();
        this->type = _data.getType();
        this->index = _data.getIndex();
        this->is_empty = _data.isEmpty();
        this->normalized = _data.isNormalized();
        this->time_mult = _data.getTime_mult();
        this->cdist_computed = false;
    }

    template<typename T>
    Data<T> mltk::Data< T >::copy() const{
        mltk::Data<T> cp;
        cp.copy(*this);
        return cp;
    }

    template<typename T>
    void mltk::Data< T >::copy(const mltk::Data<T> &_data){
        size_t _size = _data.size();
        if(this->m_points.size() > 0) this->m_points.clear();
        //this->m_points.reserve(_size);
        for(size_t i = 0; i < _size; i++){
            auto point = std::make_shared<Point< T > >();
            point->X().clear();
            point->X().resize(_data[i]->X().size());
            std::copy(_data[i]->X().begin(), _data[i]->X().end(), point->X().begin());
            point->Y() = _data[i]->Y();
            point->Alpha() = _data[i]->Alpha();
            point->Id() = _data[i]->Id();
            this->m_points.push_back(point);
        }
        this->fnames = _data.getFeaturesNames();
        this->m_size = _data.size();
        this->m_classes = _data.classes();
        this->class_names = _data.classesNames();
        this->class_distribution = _data.classesDistribution();
        this->m_dim = _data.dim();
        this->type = _data.getType();
        this->index = _data.getIndex();
        this->is_empty = _data.isEmpty();
        this->normalized = _data.isNormalized();
        this->time_mult = _data.getTime_mult();
        this->cdist_computed = _data.cdist_computed;
        this->dataset_name = _data.dataset_name;
    }

    template<typename T>
    void mltk::Data< T >::copyZero(const mltk::Data< T >& other){
        fnames = other.fnames;
        m_dim = other.m_dim;
        m_size = 0;
        is_empty = other.is_empty;
        normalized = other.normalized;
        cdist_computed = false;
    }

    template<typename T>
    void mltk::Data< T >::join(const Data< T >& data){
        size_t i, j, dim1 = data.dim(), antsize = m_size, size1 = data.size();
        std::vector<int> index1 = data.getIndex(), antindex = index;
        auto points1 = data.points();

        if(m_dim > dim1){
            std::cerr << "Error: sample1 dimension must be less or equal to sample2\n";
            exit(1);
        }

        m_size += size1;

        if(!index.empty() && !index1.empty()){
            index.resize(m_size, 0);
            for(i = 0; i < antsize; i++) index[i] = antindex[i];
            for(i = 0; i < size1; i++) index[i + antsize] = index1[i];
        }

        if(index.empty()){
            index.resize(m_size);
            iota(index.begin(), index.end(), 0);
        }

        m_points.resize(m_size);

        for(i = antsize, j = 0; i < m_size && j < size1; i++, j++){
            m_points[i] = mltk::make_point<T>();
            m_points[i]->X() = points1[j]->X();
            m_points[i]->Y() = points1[j]->Y();
            m_points[i]->Alpha() = points1[j]->Alpha();
            m_points[i]->Id() = m_points[j]->Id();
        }
        std::vector<int> diff, classes1 = data.classes(), classes = m_classes;
        std::sort(classes.begin(), classes.end());
        std::sort(classes1.begin(), classes1.end());
        std::set_difference(classes1.begin(), classes1.end(), classes.begin(), classes.end(),
                            std::inserter(diff, diff.begin()));
        m_classes.reserve(m_classes.size()+diff.size());
        m_classes.insert(m_classes.end(), diff.begin(), diff.end());

        std::vector<std::string> diff_names, classes_names1 = data.classesNames(), classes_names = class_names;
        std::sort(classes_names.begin(), classes_names.end());
        std::sort(classes_names1.begin(), classes_names1.end());
        std::set_difference(classes_names1.begin(), classes_names1.end(), classes_names.begin(), classes_names.end(),
                            std::inserter(diff_names, diff_names.begin()));
        class_names.reserve(class_names.size()+diff_names.size());
        class_names.insert(class_names.end(), diff_names.begin(), diff_names.end());

        class_distribution.assign(m_classes.size(), 0);
        computeClassesDistribution();
        if(fnames.empty()){
            fnames = data.getFeaturesNames();
        }
    }

    template<typename T>
    void mltk::Data< T >::normalize(double p){
        int i = 0, j = 0;
        double norm = 0.0;

        for(i = 0; i < m_size; ++i){
            for(norm = 0, j = 0; j < m_dim; ++j){
                norm += std::pow(fabs(m_points[i]->X()[j]),p);
            }
            m_points[i]->X().resize(m_dim+1);
            m_points[i]->X()[j] = 1;
            norm += std::pow(fabs(m_points[i]->X()[j]),p);
            norm = std::pow(norm, 1.0/p);
            for(j = 0; j < m_dim+1; ++j){
                m_points[i]->X()[j] /= norm;
            }
        }
        m_dim++;
        fnames.push_back(m_dim);

        normalized = true;
    }

    template<typename T>
    void mltk::Data< T >::setDim(size_t _dim){
        this->m_dim = _dim;
    }

    template<typename T>
    std::vector<int> mltk::Data< T >::getFeaturesNames() const{
        return fnames;
    }

    template<typename T>
    void mltk::Data< T >::setFeaturesNames(const std::vector<int>& _fnames){
        this->fnames = _fnames;
    }

    template<typename T>
    std::vector<std::shared_ptr<Point< T > > > mltk::Data< T >::points(){
        return m_points;
    }

    template<typename T>
    std::vector<std::shared_ptr<Point< T > > > mltk::Data< T >::points() const {
        return m_points;
    }

    template<typename T>
    std::vector<int> mltk::Data< T >::getIndex() const{
        return index;
    }

    template<typename T>
    bool mltk::Data< T >::isEmpty() const{
        return is_empty;
    }

    template<typename T>
    mltk::Data< T >& mltk::Data< T >::operator=(const mltk::Data< T >& data){
        m_points = data.m_points;
        fnames = data.fnames;
        index = data.index;
        m_size = data.m_size;
        m_dim = data.m_dim;
        m_classes = data.m_classes;
        class_distribution = data.class_distribution;
        class_names = data.class_names;
        time_mult = data.time_mult;
        pos_class = data.pos_class;
        neg_class = data.neg_class;
        is_empty = data.is_empty;
        normalized = data.normalized;
        dataset_name = data.dataset_name;
        type = data.type;

        return *this;
    }


    template<typename T>
    void mltk::Data< T >::clear(){
        m_points.clear();
        fnames.clear();
        index.clear();
        m_classes.clear();
        class_names.clear();
        m_size = 0;
        m_dim = 0;
        normalized = false;
        is_empty = true;
        cdist_computed = false;
    }

    template<typename T>
    mltk::Data< T >::~Data(){
        fnames.clear();
        index.clear();
        m_classes.clear();
        class_names.clear();
        m_size = 0;
        m_dim = 0;
        normalized = false;
        is_empty = true;
        cdist_computed = false;
    }

    template<typename T>
    void mltk::Data< T >::setIndex(std::vector<int> _index) {
        this->index = _index;
    }

    template<typename T>
    void mltk::Data< T >::resetIndex(){
        index.assign(m_size, 0);
        iota(index.begin(), index.end(), 0);
    }

    template<typename T>
    double mltk::Data< T >::getTime_mult() const {
        return time_mult;
    }

    template<typename T>
    bool mltk::Data< T >::operator==(const mltk::Data< T > &rhs) const {
        if(m_points.size() != rhs.m_points.size()) return false;

        size_t i, _size = m_points.size();

        for(i = 0; i < _size; i++){
            if(*m_points[i] != *rhs.m_points[i]){
                return false;
            }
        }

        if(m_classes.size() != rhs.classes().size()) return false;
        for(i = 0; i < m_classes.size(); i++){
            if(m_classes[i] != rhs.classes()[i]){
                return false;
            }
        }

        for(i = 0; i < class_distribution.size(); i++){
            if(class_distribution[i] != rhs.class_distribution[i]){
                return false;
            }
        }

        return fnames == rhs.fnames &&
               index == rhs.index &&
               _size == rhs.m_size &&
               m_dim == rhs.m_dim &&
               time_mult == rhs.time_mult &&
               pos_class == rhs.pos_class &&
               neg_class == rhs.neg_class &&
               is_empty == rhs.is_empty &&
               normalized == rhs.normalized;
    }

    template<typename T>
    bool mltk::Data< T >::operator!=(const mltk::Data< T > &rhs) const {
        return !(rhs == *this);
    }

    template<typename T>
    void mltk::Data<T>::setType(const std::string &_type) {
        this->type = _type;
    }

    template<typename T>
    const std::string &mltk::Data<T>::getType() const {
        return type;
    }

    template<typename T>
    std::vector<std::string> mltk::Data<T>::classesNames() const{
        return this->class_names;
    }

    template<typename T>
    std::vector<size_t> mltk::Data<T>::classesDistribution() const{
        return this->class_distribution;
    }

    template<typename T>
    const std::vector<int> mltk::Data<T>::classes() const {
        return this->m_classes;
    }

    template<typename T>
    void mltk::Data<T>::setClasses(const std::vector<int> &_classes) {
        this->m_classes = _classes;
        this->class_distribution.resize(_classes.size());
        this->cdist_computed = false;
    }

    template <typename T>
    void mltk::Data< T >::computeClassesDistribution(){
        if(cdist_computed) return;
        this->class_distribution = std::vector<size_t>(this->m_classes.size(), 0);
        for(auto p: m_points){
            int c = p->Y();
            auto class_it = std::find(this->m_classes.begin(), this->m_classes.end(), c);
            size_t class_pos = class_it - this->m_classes.begin();
            class_distribution[class_pos]++;
        }
    }

    template<typename T>
    std::vector<Data<T>> Data<T>::splitSample(const std::size_t &split_size, bool stratified, const size_t seed) {
        std::vector<Data<T>> split(split_size);
        auto new_size = size_t(size() / split_size);
        if(this->isClassification() && stratified){
            this->computeClassesDistribution();
            Point< double > dist(class_distribution.size());
            dist = class_distribution;
            auto classes_split = this->splitByClasses();
            std::vector<size_t> marker(m_classes.size(), 0);
            dist = (dist / size()) * new_size;

            for(size_t i = 0; i < dist.size(); i++){
                dist[i] = (dist[i] < 1.0)?1.0:std::ceil(dist[i]);
            }

            for(size_t i = 0; i < split.size(); i++){
                for(size_t j = 0; j < classes_split.size(); j++){
                    for(size_t k = 0; k < int(dist[j]); k++){
                        if(marker[j] == classes_split[j].size()) break;
                        split[i].insertPoint(classes_split[j][marker[j]]);
                        marker[j]++;
                    }
                }
                split[i].shuffle(seed+i);
            }
        }else{
            auto data = this->copy();
            data.shuffle(seed);
            size_t counter = 0;
            for(size_t i = 0; i < split.size(); i++){
                for(size_t j = 0; j < new_size; j++){
                    split[i].insertPoint(data[counter]);
                    counter++;
                    if(counter == this->size()){
                        return split;
                    }
                }
            }
        }
        return split;
    }

    template<typename T>
    bool Data<T>::updatePointValue(const size_t &idx, const double value) {
        if(idx >= m_size){
            std::cerr << "Error [Data]: idx bigger than data size.\n";
            return false;
        }
        double old_value = m_points[idx]->Y();

        if(isClassification()){
            int _c = int(value);
            auto class_pos = std::find(m_classes.begin(), m_classes.end(), _c);
            if(class_pos == m_classes.end()){
                m_classes.push_back(_c);
                class_names.push_back(std::to_string(_c));
                class_distribution.push_back(1);
            }
            auto oldclass_pos = std::find(m_classes.begin(), m_classes.end(), int(old_value))- m_classes.begin();
            class_distribution[class_pos - m_classes.begin()]++;
            class_distribution[oldclass_pos]--;
            if(class_distribution[oldclass_pos] == 0){
                class_distribution.erase(class_distribution.begin() + oldclass_pos);
                m_classes.erase(m_classes.begin() + oldclass_pos);
                class_names.erase(class_names.begin() + oldclass_pos);
            }

        }
        m_points[idx]->Y() = value;
        return true;
    }

    template<typename T>
    std::vector<Data<T>> Data<T>::splitByClasses() {
        int last_c = std::numeric_limits<int>::max();
        std::vector<Data<T>> class_split(m_classes.size());

        for(auto it = m_points.begin(); it != m_points.end(); it++){
            auto point = *(*it);
            auto class_pos = std::find(m_classes.begin(), m_classes.end(), int(point.Y())) - m_classes.begin();
            class_split[class_pos].insertPoint(*it);
        }
        for(auto &data: class_split){
            data.computeClassesDistribution();
        }

        return class_split;
    }

    template<typename T>
    Data<T> Data<T>::sampling(const size_t &samp_size, bool with_replacement, const int &seed) {
        assert(samp_size <= size());
        std::random_device rd;
        std::mt19937 gen((seed == -1)?rd():seed);
        Data< T > sample;
        std::set<std::size_t> ids;
        auto classes_split = splitByClasses();
        Point<double> class_dist(m_classes.size());
        std::vector<std::uniform_int_distribution<size_t>> dist;

        class_dist = classesDistribution();
        for(int i = 0; i < class_dist.size(); i++){
            dist.emplace_back(0, class_dist[i]-1);
        }
        class_dist = (class_dist / size()) * samp_size;

        for(size_t i = 0; i < class_dist.size(); i++){
            class_dist[i] = (class_dist[i] < 1)?1:std::floor(class_dist[i]);
        }

        for(size_t i = 0; i < class_dist.size(); i++){
            for(size_t j = 0; j < class_dist[i]; j++){
                std::size_t idx = dist[i](gen);
                if(!with_replacement) {
                    while (ids.find(idx) != ids.end()) {
                        idx = dist[i](gen);
                    }
                    ids.insert(idx);
                }
                sample.insertPoint(classes_split[i][idx]);
            }
        }

        return sample;
    }

    template<typename T>
    Data<T> Data<T>::selectFeatures(std::vector<size_t> feats, int size) {
        std::sort(feats.begin(), feats.end());
        size_t _size = (size == -1)?feats.size():size;
        Data<T> new_data;
        std::vector<size_t> feats_pos(_size);
        int i, j, invalid;
        for(i = 0, j = 0, invalid=0; (i < _size) && (invalid < _size) && (j < _size); ){
            if(feats[j] < 1) { invalid++; j++; continue; }
            if(fnames[i] == feats[j]){
                feats_pos[j] = i;
                j++;
            }
            i++;
        }
        assert((j == _size) && "There are non-existing features on remove set.");
        for(auto const& point: this->m_points){
            auto new_point = make_point<T>(_size-invalid);
            for(i = 0; i < (_size-invalid); i++){
                (*new_point)[i] = (*point)[feats_pos[i]];
                (*new_point).Y() = (*point).Y();
                (*new_point).Id() = (*point).Id();
                (*new_point).Alpha() = (*point).Alpha();
            }
            new_data.setName(this->name());
            new_data.insertPoint(new_point);
        }
        return new_data;
    }

    template<typename T>
    Data<T>::Data(const Data<T> &other) {
        this->copy(other);
    }

    template<typename T>
    Point<T> Data<T>::getFeature(int idx) const{
        Point<T> feat(m_size, T());
        for(int i = 0; i < m_size; i++){
            feat[i] = (*m_points[i])[idx];
        }
        return feat;
    }

    template<typename T>
    Point<double> Data<T>::getLabels() const{
        Point<double> labels(m_size, double());
        for(int i = 0; i < m_size; i++){
            labels[i] = m_points[i]->Y();
        }
        return labels;
    }

    template<typename T>
    void Data<T>::apply(std::function<void(mltk::PointPointer<T> point)> f) {
        std::for_each(this->m_points.begin(),this->m_points.end(), f);
    }

    template<typename T>
    std::string Data<T>::discover_dataset_name(const std::string &path) {
        auto tokens = mltk::utils::tokenize(path, '/');
        tokens = mltk::utils::tokenize(tokens.back(), '.');
        return (tokens.empty())?std::string():tokens[0];
    }
}

#endif