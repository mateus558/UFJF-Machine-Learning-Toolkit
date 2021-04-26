/**
    \brief Implementation of the Data class methods.
    \file Data.cpp
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <utility>
#include <cmath>
#include <cstring>

#include "ufjfmltk/core/Data.hpp"

namespace mltk{
    using namespace std;

    template < typename T >
    mltk::Data< T >::Data(const char* dataset){
        if(!load(string(dataset))){
            cerr << "Couldn't read the dataset." << endl;
        }
    }

    template < typename T >
    mltk::Data< T >::Data(const char* dataset, const char* datasetType, const char* pos_class, const char* neg_class){
        this->pos_class = string(pos_class);
        this->neg_class = string(neg_class);
        this->type = string(datasetType);

        if(!load(string(dataset))) {
            cerr << "Couldn't read the dataset." << endl;
        }
    }

    template < typename T >
    Type mltk::Data< T >::identifyFileType(string file){
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

    template < typename T >
    bool mltk::Data< T >::load(const string& file, bool _atEnd){
        Type t = identifyFileType(file);
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
                cerr << "Invalid file type." << endl;
                return false;
        }

        return true;
    }

    template < typename T >
    bool mltk::Data< T >::load_csv(const string& path){
        ifstream input(path.c_str());
        stringstream ss;
        string str, item;
        int _dim, ldim, _size, ssize;
        char deli = ',';
        bool cond, flag;

        flag = false;
        _dim = ldim = _size = 0;

        if(!input){
            cout << "File could not be opened!" << endl;
            return false;
        }

        //Verify if the class is at the begining or at the end and error check
        while(getline(input, str)){
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
                cerr << "All the samples must have the same dimension!" << endl;
                return false;
            }

            ldim = _dim;
            _size++;
            ss.clear();
        }

        input.clear();
        input.seekg(0, ios::beg);

        //initialize _dim and _size
        this->m_dim = _dim;
        this->m_size = _size;

        //reserve memory for fnames array and set feature names
        fnames.assign(_dim, 0);
        index.assign(_size, 0);
        iota(fnames.begin(), fnames.end(), 1);
        iota(index.begin(), index.end(), 0);

        //reserve memory for points array
        m_points.resize(_size);
        _size = 0;
        //Read sample (line) from file
        while(getline(input, str)){
            auto new_point = make_shared<Point< T > >();

            ss.str(str);
            _dim = -1;

            //reserve memory for x array
            new_point->X().resize(this->m_dim, 0.0);

            //Read features from line
            while(getline(ss, item, deli)){
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
                    if(this->isClassification())
                        if(c == -1){
                            stats.n_neg++;
                        }else{
                            stats.n_pos++;
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

    template < typename T >
    bool mltk::Data< T >::load_data(const string& path){
        ifstream input(path.c_str());
        string str, item, buffer;
        stringstream ss, ss1;
        int _dim, ldim, _size;
        double c;
        bool is_feature, type_detect = false;

        if(!input){
            cout << "File could not be opened!" << endl;
            return false;
        }
        _dim = ldim = _size = c = 0;
        //get dimension of the points and do error check
        while(getline(input, str)){
            _dim = -1;

            ss.str(str);
            ss.clear();

            while(getline(ss, item, ' ')){
                const char * pch = strchr(item.c_str(), ':');
                _dim++;
                /*if(_size > 0 && _dim < ldim && pch == nullptr){
                    std::cerr << "Error (line: " << _size << ", _dim: " << _dim << "): file isn't in the .data format." << std::endl;
                    return false;
                }*/

                ss1.str(item);
                ss1.clear();
                while(_size > 0 && getline(ss1, item, ':')) {
                    if(_dim == ldim && !type_detect){
                        if(strchr(item.c_str(), '.')){
                            this->type = "Regression";
                        }else{
                            this->type = "Classification";
                        }
                        type_detect = true;
                    }else if(_dim < ldim && !utils::is_number(item)){
                        clog << "Warning (line: " << _size << "): feature " << _dim << " is not a number." << endl;
                    }
                }
            }
            if(ldim != _dim && ldim != 0){
                cerr << "Error (line: " << _size << "): all the samples must have the same dimension! (_dim: " << _dim << ", last_dim: " << ldim << ")" << endl;
                return false;
            }

            ldim = _dim;
            _size++;
        }

        input.clear();
        input.seekg(0, ios::beg);

        //initialize _dim and _size
        this->m_dim = _dim;
        this->m_size = _size;

        //reserve memory for fnames array and set feature names
        fnames.assign(_dim, 0);
        index.assign(_size, 0);
        iota(fnames.begin(), fnames.end(), 1);
        iota(index.begin(), index.end(), 0);

        //reserve memory for points array
        m_points.resize(_size);
        _size = 0;

        //get lines from file
        while(getline(input, str)){
            auto new_point = make_shared<Point< T > >();

            ss.str(str);
            ss.clear();
            _dim = 0;
            new_point->X().resize(this->m_dim, 0.0);

            //Read features from line
            while(getline(ss, item, ' ')){
                const char * pch = strchr(item.c_str(), ':');
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
                    while(getline(ss1, item, ':')){
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

            for(auto it = class_names.begin(); it != class_names.end(); it++){
                if((*it) == "-1"){
                    this->stats.n_neg = this->class_distribution[0];
                }else{
                    this->stats.n_pos = this->class_distribution[1];
                }
            }
            type = "BinClassification";
        }else{
            type = "MultiClassification";
        }

        input.close();
        is_empty = false;
        return true;
    }

    template < typename T >
    bool mltk::Data< T >::load_arff(const string& path){
        ifstream input(path.c_str());
        istringstream ss;
        string str, item;
        int _dim, ldim, _size, c;
        bool atBegin, flag, cond;

        if(!input){
            cout << "File could not be opened! \nError: " << strerror(errno) << endl;
            return false;
        }

        _dim = ldim = _size = c = 0;
        atBegin = flag = cond = false;

        //Verify if the class is at the begining or at the end and error check
        while(getline(input, str)){
            _dim = 0;
            ss.str(str);

            while(getline(ss, item, ',')){
                //check for invalid feature or class
                auto found_class = std::find_if(this->class_names.begin(), this->class_names.end(), [&item](const std::string &_class){
                    return (_class == item);
                });
                
                if(!utils::is_number(item) && (found_class != this->class_names.end())){
                    clog << "Warning: point[" << _size << "] " << _dim + 1 << " feature is not a number. (" << item << ")" << endl;
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
                cerr << "All the samples must have the same dimension!" << endl;
                return false;
            }

            ldim = _dim;
            _size++;
            ss.clear();
        }
        input.clear();
        input.seekg(0, ios::beg);

        //initialize _dim and _size
        _dim--;
        this->m_dim = _dim;
        this->m_size = _size;

        //reserve memory for fnames array and set feature names
        fnames.assign(_dim, 0);
        index.assign(_size, 0);
        iota(fnames.begin(), fnames.end(), 1);
        iota(index.begin(), index.end(), 0);

        //reserve memory for points array
        m_points.resize(_size);

        _size = 0;

        //Read line (sample) from file
        while(getline(input, str)){
            auto new_point = make_shared<Point< T > >();
            _dim = -1;
            ss.str(str);

            //reserve memory for features
            new_point->X().assign(this->m_dim, 0.0);

            //Read features from line
            while(getline(ss, item, ',')){
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

    template < typename T >
    bool mltk::Data< T >::load_txt(const string& path){
        ifstream input(path.c_str());
        istringstream ss;
        string str, item;
        int n, _dim = 0, n1, d, _size;

        if(!input){
            cout << "File could not be opened!" << endl;
            return false;
        }

        n1 = d = _size = 0;

        //error check
        while(getline(input, str)){
            ss.str(str);
            n1 = 0;
            _dim = 0;

            while(getline(ss, item, ' ')){
                if(n1 >= 2){
                    _dim++;
                }
                n1++;
            }

            if(_size != 0 && _dim != d ){
                cerr << _dim << " " << this->m_dim << endl;
                cerr << "All the samples must have the same dimension!" << endl;
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
        iota(fnames.begin(), fnames.end(), 1);
        iota(index.begin(), index.end(), 0);

        input.clear();
        input.seekg(0, ios::beg);

        //Allocate memory for points array
        m_points.resize(_size);

        _size = 0;

        //get line from file (sample)
        while(getline(input, str)){
            auto new_point = make_shared<Point< T > >();

            //Allocate memory for features
            new_point->X().resize(_dim, 0.0);
            ss.str(str);
            n = 0;

            //read features from line
            while(getline(ss, item, ' ')){
                if(n >= 2){
                    if(utils::is_number(item))
                        new_point->X()[n - 2] = utils::atod(item.c_str());
                    else{ clog << "Warning: point[" << _size << "] " << n - 2 << " feature is not a number." << endl; }
                    new_point->Y() = 0;
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

    template < typename T >
    bool mltk::Data< T >::removePoint(int pid){
        int i;

        if(m_size == 1){ cout << "Error: RemovePoint, only one point left\n"; return false; }
        //Ids bound verification
        if(pid > m_points[m_size - 1]->Id() || pid <= 0) return false;

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
                if(stats.n_pos > 0 || stats.n_neg > 0){
                    if(m_points[i]->Y() == 1) stats.n_pos--;
                    else if(m_points[i]->Y() == -1) stats.n_neg--;
                }
                m_points.erase(m_points.begin() + i);
                break;
            }
        }
        m_size--;

        return true;
    }

    template < typename T >
    void mltk::Data< T >::write(const string& fname, string ext){
        int i, j;
        string path = fname + "." + ext;
        ofstream outstream(path.c_str(), ios::out);

        if(!outstream.is_open()){
            cerr << "Can't write in file." << endl;
            return;
        }

        for(i = 0; i < m_size; i++){
            if(ext == "plt"){
                outstream << m_points[i]->Y() << " ";
                for(j = 0; j < m_dim-1; j++){
                    outstream << m_points[i]->X()[j] << " ";
                }
                outstream << m_points[i]->X()[j] << endl;
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

    template < typename T >
    vector<bool> mltk::Data< T >::removePoints(vector<int> ids){
        int idsize = ids.size(), i;
        bool save;
        std::shared_ptr<Point< T > > po;
        auto p = m_points.begin();
        vector<bool> notFound(idsize, true);

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
                //Size verification.
                if(m_size == 1){ clog << "Error: RemovePoint, only one point left." << endl; break;}
                if(c == 1) stats.n_pos--;
                else if(c == -1) stats.n_neg--;
                auto class_pos = std::find_if(m_classes.begin(), m_classes.end(), [&c](auto &_c){
                   return (c == _c);
                }) - m_classes.begin();
                class_distribution[class_pos]--;
                m_size--;
            }
        }

        return notFound;
    }

    template < typename T >
    mltk::Data< T >* mltk::Data< T >::insertFeatures(std::vector<int> ins_feat){
        size_t i, j, s, offset = 0, fsize = ins_feat.size();
        bool saveflag = false;
        vector<int> new_fnames(fsize, 0);
        std::shared_ptr<Point< T > > p;
        auto *smout = new mltk::Data< T >;

        if(fsize == 0) return this;
        sort(ins_feat.begin(), ins_feat.end());

        //error check
        if(fsize > m_dim){ cerr << "Error: InsertFeature, fsize(" << ins_feat.size() << ")>dim(" << m_dim << ")\n"; return smout; }
        smout->setDim(fsize);

        //Copying information to new data array
        for(i = 0; i < m_size; i++){
            p = make_shared<Point< T > >();
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
                cerr << "Error: Something went wrong on InsertFeature\n";
                cerr << "s = " << s << ", dim = " << m_dim << ", fsize = " << fsize << endl;
                smout->clear();
                return smout;
            }
            smout->insertPoint(p);
        }
        smout->setFeaturesNames(new_fnames);

        return smout;
    }

    template < typename T >
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
    bool mltk::Data< T >::removeFeatures(std::vector<int> feats){
        size_t i, j, k, psize = m_points.size(), rsize = feats.size();
        typename vector< T >::iterator itr;
        vector<int>::iterator fitr;
        vector<bool> exist(rsize, true);

        if(feats.empty()) return true;

        if(fnames.size() == 1){
            cerr << "Error: RemoveFeature, only one feature left.\n";
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
                if(feats[i] == fnames[j]){
                    break;
                }
            }
            if(j == m_dim){
                exist[i] = false;
            }
        }

        //Remove features from each point
        for(i = 0; i < psize; i++){
            if(m_points[i] == nullptr) clog << "WARNING: point is null." << endl;

            // Iterate through the point features
            for(itr = m_points[i]->X().begin(),k = 0, j = 0; itr != m_points[i]->X().end();){
                while(!exist[k] && k < rsize) k++; // go to next existent feature
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

    template < typename T >
    bool mltk::Data< T >::insertPoint(mltk::Data< T > sample, int _index){
        if(_index > sample.size() - 1){
            cerr << "Index out of bounds. (insertPoint)" << endl;
            return false;
        }

        insertPoint(sample.point(_index));

        return true;
    }

    template < typename T >
    bool mltk::Data< T >::insertPoint(std::shared_ptr<Point< T > > p){
        //Dimension verification
        if(m_size > 0 && int(p->X().size()) >m_dim){
            cerr << "Point with dimension different from the data. (insertPoint)" << endl;
            cerr << "Point dim = " << p->X().size() << " dim = " << m_dim << endl;
            return false;
        }

        if(m_size == 0) m_dim = p->X().size();
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

    template < typename T >
    bool mltk::Data< T >::insertPoint(Point< T > &p){
        return this->insertPoint(std::make_shared<Point< T > >(p));
    }

    template < typename T >
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

    template < typename T >
    std::shared_ptr<Point< T > > mltk::Data< T >::point(int _index){
        return m_points[_index];
    }

    template < typename T >
    void mltk::Data< T >::setPoint(int _index, std::shared_ptr<Point< T > > p){
        m_points[_index] = p;
    }

    template < typename T >
    void mltk::Data< T >::classesCopy(const mltk::Data< T > &_data, std::vector<int> &classes){
        size_t _size = 0;
        std::set<int> _classes(m_classes.begin(), m_classes.end());
        
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
        this->stats = _data.getStatistics();
        this->m_dim = _data.dim();
        this->type = _data.getType();
        this->index = _data.getIndex();
        this->is_empty = _data.isEmpty();
        this->normalized = _data.isNormalized();
        this->time_mult = _data.getTime_mult();
        this->cdist_computed = false;
    }

    template < typename T >
    void mltk::Data< T >::copy(const mltk::Data<T> &_data){
        size_t _size = _data.size();
        this->m_points.resize(_size);
        for(size_t i = 0; i < _size; i++){
            this->m_points[i] = std::make_shared<Point< T > >();
            this->m_points[i]->X() = _data[i]->X();
            this->m_points[i]->Y() = _data[i]->Y();
            this->m_points[i]->Alpha() = _data[i]->Alpha();
            this->m_points[i]->Id() = _data[i]->Id();
        }
        this->fnames = _data.getFeaturesNames();
        this->m_size = _data.size();
        this->m_classes = _data.classes();
        this->class_names = _data.classesNames();
        this->class_distribution = _data.classesDistribution();
        this->stats = _data.getStatistics();
        this->m_dim = _data.dim();
        this->type = _data.getType();
        this->index = _data.getIndex();
        this->is_empty = _data.isEmpty();
        this->normalized = _data.isNormalized();
        this->time_mult = _data.getTime_mult();
        this->cdist_computed = _data.cdist_computed;
    }

    template < typename T >
    void mltk::Data< T >::copyZero(const mltk::Data< T >& other){
        fnames = other.fnames;
        m_dim = other.m_dim;
        m_size = 0;
        is_empty = other.is_empty;
        normalized = other.normalized;
        cdist_computed = false;
    }

    template < typename T >
    void mltk::Data< T >::join(std::shared_ptr<mltk::Data< T > > data){
        size_t i, j, dim1 = data->dim(), antsize = m_size, size1 = data->size();
        std::vector<int> index1 = data->getIndex(), antindex = index;
        std::vector<std::shared_ptr<Point< T > > > points1 = data->points();

        if(m_dim > dim1){
            cerr << "Error: sample1 dimension must be less or equal to sample2\n";
            exit(1);
        }

        m_size += size1;

        if(!index.empty() && !index1.empty()){
            index.resize(m_size, 0);
            for(i = 0; i < antsize; i++) index[i] = antindex[i];
            for(i = 0; i < size1; i++) index[i + antsize] = index1[i];
        }

        m_points.resize(m_size);

        for(i = antsize, j = 0; i < m_size && j < size1; i++, j++){
            m_points[i] = points1[j];
            if(points1[j]->Y() == 1) stats.n_pos++;
            else if(points1[j]->Y() == -1) stats.n_neg++;
        }

    }

    template < typename T >
    void mltk::Data< T >::normalize(double p){
        int i = 0, j = 0;
        double norm = 0.0;

        for(i = 0; i < m_size; ++i){
            for(norm = 0, j = 0; j < m_dim; ++j){
                norm += std::pow(fabs(m_points[i]->X()[j]),p);
            }
            m_points[i]->X().resize(m_dim+1);
            m_points[i]->X()[j] = 1;
            fnames[j] = j+1;
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

    template < typename T >
    void mltk::Data< T >::normalize(std::vector<double> &v, double q){
        size_t i = 0, dim = v.size();
        double norm = 0.0;

        for(i = 0; i < dim; ++i)
            norm += std::pow(fabs(v[i]), q);

        norm = std::pow(norm, 1.0/q);

        for(i = 0; i < dim; ++i)
            v[i] /= norm;
    }

    template < typename T >
    void mltk::Data< T >::setDim(size_t _dim){
        this->m_dim = _dim;
    }

    template < typename T >
    std::vector<int> mltk::Data< T >::getFeaturesNames() const{
        return fnames;
    }

    template < typename T >
    void mltk::Data< T >::setFeaturesNames(const std::vector<int>& _fnames){
        this->fnames = _fnames;
    }

    template < typename T >
    std::vector<std::shared_ptr<Point< T > > > mltk::Data< T >::points(){
        return m_points;
    }

    template < typename T >
    std::vector<int> mltk::Data< T >::getIndex() const{
        return index;
    }

    template < typename T >
    int mltk::Data< T >::getNumberNegativePoints(){
        return stats.n_neg;
    }

    template < typename T >
    int mltk::Data< T >::getNumberPositivePoints(){
        return stats.n_pos;
    }

    template < typename T >
    Statistics< T > mltk::Data< T >::getStatistics() const{
        return stats;
    }

    template < typename T >
    bool mltk::Data< T >::isEmpty() const{
        return is_empty;
    }

    template < typename T >
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
        stats = data.stats;

        return *this;
    }


    template < typename T >
    void mltk::Data< T >::clear(){
        m_points.clear();
        fnames.clear();
        index.clear();
        m_classes.clear();
        class_names.clear();
        m_size = 0;
        m_dim = 0;
        stats.n_neg = 0;
        stats.n_pos = 0;
        stats.centroid = Point< T >();
        stats.neg_centroid = Point< T >();
        stats.pos_centroid = Point< T >();
        normalized = false;
        is_empty = true;
        cdist_computed = false;
    }

    template < typename T >
    mltk::Data< T >::~Data(){
        this->clear();
    }

    template < typename T >
    void mltk::Data< T >::setIndex(std::vector<int> _index) {
        this->index = _index;
    }

    template < typename T >
    void mltk::Data< T >::resetIndex(){
        index.assign(m_size, 0);
        iota(index.begin(), index.end(), 0);
    }

    template < typename T >
    double mltk::Data< T >::getTime_mult() const {
        return time_mult;
    }

    template < typename T >
    bool mltk::Data< T >::operator==(const mltk::Data< T > &rhs) const {
        if(m_points.size() != rhs.m_points.size()) return false;

        size_t i, _size = m_points.size();

        for(i = 0; i < _size; i++){
            if(*m_points[i] != *rhs.m_points[i]){
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

    template < typename T >
    bool mltk::Data< T >::operator!=(const mltk::Data< T > &rhs) const {
        return !(rhs == *this);
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
        }

        std::iota(fnames.begin(), fnames.end(), 1);
        std::iota(index.begin(), index.end(), 0);
        this->m_size = size;
        this->m_dim = dim;
        this->is_empty = false;
    }

    template<typename T>
    void mltk::Data<T>::setType(const string &_type) {
        this->type = _type;
    }

    template<typename T>
    const string &mltk::Data<T>::getType() const {
        return type;
    }

    template<typename T>
    int mltk::Data<T>::process_class(std::string item) {
        int c = 0;
        auto class_name_it = std::find(this->class_names.begin(), this->class_names.end(), item);
        
        if(class_name_it == class_names.end()){
            this->class_names.push_back(item);
            auto has_mone = std::find(class_names.begin(), class_names.end(), "-1");
            if(has_mone == class_names.end()){
                c = this->class_names.size();
            }else{
                c = this->class_names.size()-1;
            }
            if(item == "-1"){
                c = -1;
            }
        }else{
            item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());
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
            auto class_it = std::find_if(this->m_classes.begin(), this->m_classes.end(), [&c](int _c){
                return c == _c;
            });
            size_t class_pos = class_it - this->m_classes.begin();
            class_distribution[class_pos]++;
        }
    }

    template<typename T>
    std::vector<Data<T>> Data<T>::splitSample(const std::size_t &split_size, const size_t seed) {
        this->computeClassesDistribution();
        Point< double > dist(class_distribution.size());
        dist = class_distribution;
        auto new_size = size_t(size() / split_size);
        auto classes_split = this->splitByClasses();
        std::vector<size_t> marker(m_classes.size(), 0);
        std::vector<Data<T>> split(split_size);
        dist = (dist / size()) * new_size;

        for(size_t i = 0; i < dist.size(); i++){
            dist[i] = (dist[i] < 1.0)?1.0:std::round(dist[i]);
        }

        for(size_t i = 0; i < split.size(); i++){
            for(size_t j = 0; j < classes_split.size(); j++){
                for(size_t k = 0; k < dist[j]; k++){
                    if(marker[j] == classes_split[j].size()) break;
                    split[i].insertPoint(classes_split[j][marker[j]]);
                    marker[j]++;
                }
            }
            split[i].shuffle(seed+i);
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
            }else {
                class_distribution[class_pos - m_classes.begin()]++;
            }
        }
        m_points[idx]->Y() = value;
        return true;
    }

    template<typename T>
    std::vector<Data<T>> Data<T>::splitByClasses() {
        int last_c = std::numeric_limits<int>::max();
        size_t class_pos = 0;
        std::vector<Data<T>> class_split(m_classes.size());

        for(auto it = m_points.begin(); it != m_points.end(); it++){
            auto point = *(*it);
            if(last_c != point.Y()) {
                class_pos = std::find(m_classes.begin(), m_classes.end(), int(point.Y())) - m_classes.begin();
            }
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
        std::uniform_int_distribution<size_t> dist(0, size() - 1);
        Data< T > sample;
        std::set<std::size_t> ids;
        auto classes_split = splitByClasses();
        Point<double> class_dist(m_classes.size());

        class_dist = classesDistribution();
        class_dist = (class_dist / size()) * samp_size;

        for(size_t i = 0; i < class_dist.size(); i++){
            class_dist[i] = (class_dist[i] < 1)?1:std::floor(class_dist[i]);
        }
        for(size_t i = 0; i < class_dist.size(); i++){
            for(size_t j = 0; j < class_dist[i]; j++){
                std::size_t idx = dist(gen);
                if(!with_replacement) {
                    while (ids.find(idx) != ids.end()) {
                        idx = dist(gen);
                    }
                    ids.insert(idx);
                }
                sample.insertPoint(m_points[idx]);
            }
        }

        return sample;
    }

    template<typename T>
    Data<T>::Data(const string &dataset, bool atEnd): atEnd(atEnd) {
        if(!load(string(dataset), atEnd)){
            cerr << "Couldn't read the dataset." << endl;
        }
    }

    template<typename T>
    Data<T> Data<T>::selectFeatures(std::vector<size_t> feats) {
        std::sort(feats.begin(), feats.end());
        Data<T> new_data;
        for(auto const& point: this->m_points){
            auto new_point = make_point<T>(feats.size());
            size_t i = 0;
            for(auto const& feat: feats){
                assert(feat < point->size());
                (*new_point)[i] = (*point)[feat];
                (*new_point).Y() = (*point).Y();
                (*new_point).Id() = (*point).Id();
                (*new_point).Alpha() = (*point).Alpha();
                i++;
            }
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


    template class mltk::Data<int>;
    template class mltk::Data<double>;
    template class mltk::Data<float>;
    template class mltk::Data<int8_t>;
    template class mltk::Data<char>;
    template class mltk::Data<long long int>;
    template class mltk::Data<short int>;
    template class mltk::Data<long double>;
    template class mltk::Data<unsigned char>;
    template class mltk::Data<unsigned int>;
    template class mltk::Data<unsigned short int>;
}