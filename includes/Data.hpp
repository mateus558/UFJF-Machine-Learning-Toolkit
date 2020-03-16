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

#ifndef DATA__HPP
#define DATA__HPP

#include <vector>
#include <string>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <memory>

#include "Point.hpp"
#include "Statistics.hpp"
#include "Utils.hpp"

static const std::vector<std::string> types {"data", "csv", "arff", "txt", "plt"};
enum  Type {TYPE_INVALID = -1, TYPE_DATA = 0, TYPE_CSV = 1, TYPE_ARFF = 2, TYPE_TXT = 3};

template < typename T > 
class Statistics;

/**
 * \brief Wrapper for the dataset data.
 */

template < typename T >
class Data {
    // Associations
    // Attributes
private :
    /// Set of points.
    std::vector<std::shared_ptr<Point< T > > > points;
    /// Features names.
    std::vector<int> fnames;
    /// Points indexes.
    std::vector<int> index;
    std::vector<std::string> class_names;
    std::vector<int> classes;
    std::vector<size_t> class_distribution;
    /// Number of points in the dataset.
    size_t size = 0;
    /// Dataset points dimension.
    size_t dim = 0;
    /// Time multiplier
    double time_mult;
    /// Positive and negative classes. (1, -1 are the default classes)
    std::string pos_class = std::string("1"), neg_class = std::string("-1");
    /// Verify if there's some data loaded.
    bool is_empty = true;
    /// Verify if the data is normalized.
    bool normalized = false;
    /// Values for statistical methods.
    Statistics< T > stats;
    /// Dataset type.
    std::string type = "Classification";
public:
    const std::string &getType() const;

private:

    // Private Operations
    int process_class(std::string item);
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
    bool load_csv (std::string path);
    /**
     * \brief Load a dataset from a arff file.
     * \param path (???) Path to arff file.
     * \return bool
     */
    bool load_arff (std::string path);
    /**
     * \brief Load a dataset from a data file.
     * \param path (???) Path to data file.
     * \return bool
     */
    bool load_data (std::string path);
    /**
     * \brief Load a dataset from a txt file.
     * \param path (???) Path to txt file.
     * \return bool
     */
    bool load_txt (std::string path);

public :
    void setType(const std::string &type);

    Data() {}
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
    Data (const char* dataset);
    
    /*********************************************
     *               Getters                     *
     *********************************************/

    /**
     * \brief Returns the size of the dataset.
     * \return int
     */
    inline size_t getSize (){ return size;}
    /**
     * \brief Returns the dimension of the dataset.
     * \return int
     */
    inline size_t getDim (){ return dim; }
    /**
     * \brief Returns a shared pointer to the vector of Points of the sample.
     * \return std::vector<std::shared_ptr<Point< T > > >
     */
    std::vector<std::shared_ptr<Point< T > > > getPoints ();
    /**
     * \brief Returns a shared pointer to the point with the given index.
     * \param index    Position of a point in the points array.
     * \return std::vector<Point< T > >
     */
    std::shared_ptr<Point< T > > getPoint (int index);
    std::vector<size_t> getClassesDistribution();
    std::vector<std::string> getClassNames();
    /**
     * \brief Returns the features names.
     * \return std::vector<int>
     */
    std::vector<int> getFeaturesNames ();
    /**
     * \brief Returns a class with the statistics info of the sample.
     * \return Statistics
     */
    Statistics< T > getStatistics ();
    /**
     * \brief Returns the vector of indexes.
     * \return std::vector<int>
     */
    std::vector<int> getIndex();
    /**
     * \brief Return the number of positive points.
     * \return int
     */
    int getNumberPositivePoints ();
    /**
     * \brief Return the number of negative points.
     * \return int
     */
    int getNumberNegativePoints ();
    /**
     * \brief Return the time multiplier.
     * \return double
     */
    double getTime_mult() const;

    /*********************************************
     *               Setters                     *
     *********************************************/

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
    void setFeaturesNames(std::vector<int> fnames);
    /**
     * \brief Set the index vector for the data.
     * \param index Index vector.
     */
    void setIndex(std::vector<int> index);
    /**
     * \brief setDim Set the dimension of the points.
     * \param dim (???) Dimension to be set.
     */
    void setDim(size_t dim);
    /**
     * \brief setClasses Set the classes of the dataset.
     * \param pos   Positive class.
     * \param neg   Negative class.
     */
    void setClasses(std::string pos, std::string neg);

    /*********************************************
     *              Other operations             *
     *********************************************/

    /**
     * \brief Load a dataset from a file.
     * \param file (???) Path to dataset file.
     * \return bool
     */
    bool load (std::string file);
    /**
     * \brief write Write the data to a file with the given extention.
     * \param fname Name of the file.
     * \param ext   Extention of the file.
     */
    void write(std::string fname, std::string ext);
    /**
     * \brief Returns if there's a dataset loaded.
     * \return bool
     */
    bool isEmpty ();
    /**
     * \brief Returns if the dataset is normalized.
     * \return bool
     */
    inline bool isNormalized(){ return normalized; }
    /**
     * \brief clear Clear the data.
     */
    void clear();
    /**
     * \brief Returns a copy of the data.
     * \return Data
     */
    Data< T > copy ();
    /**
     * \brief Returns a copy of the data with zero points.
     * \return Data
     */
    void copyZero (const Data< T >& other);
    /**
     * \brief Merge one dataset with another.
     * \param data (???) Dataset to be joined.
     * \return bool
     */
    void join(std::shared_ptr<Data< T > > data);
    /**
     * \brief Insert a point to the data from another sample.
     * \param sample (???) Sample with the point to be added.
     * \param id (???) Index of the point to be added.
     * \return bool
     */
    bool insertPoint (Data< T > sample, int id);
    /**
     * \brief Insert a point to the end of vector points.
     * \param p (???) Point to be inserted.
     * \return bool
     */
    bool insertPoint (std::shared_ptr<Point< T > > p);
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
     * \brief normalize Normalize a vector using a Lp-norm.
     * \param q Norm to be utilized.
     * \param p Vector to be normalized.
     */
    static void normalize(std::vector<double> &p, double q);
    /**
     * \brief Reset the index vector.
     */
    void resetIndex();

    /*********************************************
     *  Overloaded operators for the Data class. *
     *********************************************/

    std::shared_ptr<Point< T > > operator[](size_t i) const {return points[i];}

    std::shared_ptr<Point< T > > & operator[](size_t i) {return points[i];}

    void operator=(const Data< T >&);

    bool operator==(const Data< T > &rhs) const;

    bool operator!=(const Data< T > &rhs) const;

    template< typename U >
    friend std::ostream &operator<<( std::ostream &output, const Data< U > &data );

    ~Data();
};

template < typename T >
std::ostream &operator<<( std::ostream &output, const Data< T > &data ){
    for(auto p : data.points){
        output << *p << std::endl;
    }

    return output;
}

#endif
