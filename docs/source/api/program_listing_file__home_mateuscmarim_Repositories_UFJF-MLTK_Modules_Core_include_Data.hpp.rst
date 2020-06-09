
.. _program_listing_file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Core_include_Data.hpp:

Program Listing for File Data.hpp
=================================

|exhale_lsh| :ref:`Return to documentation for file <file__home_mateuscmarim_Repositories_UFJF-MLTK_Modules_Core_include_Data.hpp>` (``/home/mateuscmarim/Repositories/UFJF-MLTK/Modules/Core/include/Data.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

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
   
   template < typename T >
   class Data {
       // Associations
       // Attributes
   private :
       std::vector<std::shared_ptr<Point< T > > > points;
       std::vector<int> fnames;
       std::vector<int> index;
       std::vector<std::string> class_names;
       std::vector<int> classes;
       std::vector<size_t> class_distribution;
       size_t size = 0;
       size_t dim = 0;
       double time_mult = 0.0;
       std::string pos_class = std::string("1"), neg_class = std::string("-1");
       bool is_empty = true;
       bool normalized = false;
       Statistics< T > stats;
       std::string type = "Classification";
   public:
       const std::string &getType() const;
   
   private:
   
       // Private Operations
       int process_class(std::string item);
       Type identifyFileType (std::string file);
       bool load_csv (const std::string& path);
       bool load_arff (const std::string& path);
       bool load_data (const std::string& path);
       bool load_txt (const std::string& path);
   
   public :
       void setType(const std::string &type);
   
       Data() {}
       explicit Data (const char* dataset, const char* datasetType, const char* pos_class = "1", const char* neg_class = "-1");
       Data (size_t size, size_t dim, T val = T());
       Data (const char* dataset);
   
       /*********************************************
        *               Getters                     *
        *********************************************/
       void setClasses(const std::vector<int> &classes);
   
       size_t getSize() const{ return size;};
       size_t getDim () const{ return dim; }
       std::vector<std::shared_ptr<Point< T > > > getPoints ();
   
       const std::vector<int> &getClasses() const;
   
       std::shared_ptr<Point< T > > getPoint (int index);
       std::vector<size_t> getClassesDistribution() const ;
       std::vector<std::string> getClassNames() const;
       std::vector<int> getFeaturesNames() const;
       Statistics< T > getStatistics () const;
       std::vector<int> getIndex() const;
       int getNumberPositivePoints ();
       int getNumberNegativePoints ();
       double getTime_mult() const;
   
       /*********************************************
        *               Setters                     *
        *********************************************/
   
       void setPoint (int index, std::shared_ptr<Point< T > > p);
       void setFeaturesNames(const std::vector<int>& fnames);
       void setIndex(std::vector<int> index);
       void setDim(size_t dim);
   
       /*********************************************
        *              Other operations             *
        *********************************************/
   
       bool load (const std::string& file);
       void write(const std::string& fname, std::string ext);
       bool isEmpty () const;
       bool isNormalized() const{ return normalized; };
       void clear();
       void copy (const Data< T > &_data);
       void copyZero (const Data< T >& other);
       void join(std::shared_ptr<Data< T > > data);
       bool insertPoint (Data< T > sample, int id);
       bool insertPoint (std::shared_ptr<Point< T > > p);
       std::vector<bool> removePoints (std::vector<int> ids);
       bool removePoint (int pid);
       Data< T >* insertFeatures(std::vector<int> ins_feat);
       bool removeFeatures (std::vector<int> feats);
       void changeXVector(std::vector<int> index);
       void normalize(double p = 2);
       static void normalize(std::vector<double> &p, double q);
       void resetIndex();
   
       /*********************************************
        *  Overloaded operators for the Data class. *
        *********************************************/
   
       std::shared_ptr<Point< T > > operator[](size_t i) const {return points[i];}
   
       std::shared_ptr<Point< T > > & operator[](size_t i) {return points[i];}
   
       Data< T >& operator=(const Data< T >&);
   
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
