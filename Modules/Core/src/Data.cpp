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

#include "Data.hpp"

using namespace std;

template < typename T >
Data< T >::Data(const char* dataset){
    if(!load(string(dataset))){
        cerr << "Couldn't read the dataset." << endl;
    }
}

template < typename T >
Data< T >::Data(const char* dataset, const char* datasetType, const char* pos_class, const char* neg_class){
    this->pos_class = string(pos_class);
    this->neg_class = string(neg_class);
    this->type = string(datasetType);

    if(!load(string(dataset))) {
        cerr << "Couldn't read the dataset." << endl;
    }
}

template < typename T >
Type Data< T >::identifyFileType(string file){
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
bool Data< T >::load(const string& file){
    Type t = identifyFileType(file);

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
bool Data< T >::load_csv(const string& path){
    ifstream input(path.c_str());
    stringstream ss;
    string str, item;
    int _dim, ldim, _size, ssize;
    char deli = ',';
    bool cond, flag, atEnd;

    atEnd = flag = false;
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
                        atEnd = true;
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
    this->dim = _dim;
    this->size = _size;

    //reserve memory for fnames array and set feature names
    fnames.assign(_dim, 0);
    index.assign(_size, 0);
    iota(fnames.begin(), fnames.end(), 1);
    iota(index.begin(), index.end(), 0);

    //reserve memory for points array
    points.resize(_size);
    _size = 0;
    atEnd = false;
    //Read sample (line) from file
    while(getline(input, str)){
        auto new_point = make_shared<Point< T > >();

        ss.str(str);
        _dim = -1;

        //reserve memory for x array
        new_point->x.resize(this->dim, 0.0);

        //Read features from line
        while(getline(ss, item, deli)){
            if(atEnd)
                cond = (!ss.eof() && atEnd);
            else
                cond = _dim != -1;

            if(cond){
                if(Utils::is_number(item))
                    new_point->x[(!atEnd) ? _dim : _dim + 1] = Utils::atod(item.c_str());
            }else{
                double c;

                if(this->isClassification()) {
                    c = process_class(item);
                }else{
                    c = Utils::atod(item.c_str());
                }
                if(this->isClassification())
                    if(c == -1){
                        stats.n_neg++;
                    }else{
                        stats.n_pos++;
                    }
                new_point->y = c;
            }
            _dim++;
        }

        points[_size++] = std::move(new_point);
        points[_size - 1]->id = _size;
        ss.clear();
    }

    is_empty = false;

    return true;
}

template < typename T >
bool Data< T >::load_data(const string& path){
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
                }else if(_dim < ldim && !Utils::is_number(item)){
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
    this->dim = _dim;
    this->size = _size;

    //reserve memory for fnames array and set feature names
    fnames.assign(_dim, 0);
    index.assign(_size, 0);
    iota(fnames.begin(), fnames.end(), 1);
    iota(index.begin(), index.end(), 0);

    //reserve memory for points array
    points.resize(_size);
    _size = 0;

    //get lines from file
    while(getline(input, str)){
        auto new_point = make_shared<Point< T > >();

        ss.str(str);
        ss.clear();
        _dim = 0;
        new_point->x.resize(this->dim, 0.0);

        //Read features from line
        while(getline(ss, item, ' ')){
            const char * pch = strchr(item.c_str(), ':');
            if(!pch){
                if(this->isClassification()) {
                    c = process_class(item);
                }else{
                    c = Utils::atod(item.c_str());
                }
                new_point->y = c;
            }
            //Verify if the class is at the beggining or at the end
            if(!ss.eof()){
                is_feature = false; //Verify if it's including value or fname
                ss1.str(item);
                ss1.clear();
                //Get feature name and value
                while(getline(ss1, item, ':')){
                    if(!is_feature){
                        fnames[_dim] = Utils::stoin(item);
                        is_feature = true;
                    }else{
                        if(Utils::is_number(item)){
                            new_point->x[_dim] = Utils::atod(item.c_str());
                            _dim++;
                        }
                    }
                }
            }
        }
        points[_size++] = std::move(new_point);
        points[_size - 1]->id = _size;
    }

    if(classes.size() == 2){

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
bool Data< T >::load_arff(const string& path){
    ifstream input(path.c_str());
    istringstream ss;
    string str, item;
    int dim, ldim, _size, c;
    bool atEnd, atBegin, flag, cond;

    if(!input){
        cout << "File could not be opened!" << endl;
        return false;
    }

    dim = ldim = _size = c = 0;
    atEnd = atBegin = flag = cond = false;

    //Verify if the class is at the begining or at the end and error check
    while(getline(input, str)){
        dim = 0;
        ss.str(str);

        while(getline(ss, item, ',')){
            //check for invalid feature or class
            if(!Utils::is_number(item) && (item != pos_class || item != neg_class)){
                clog << "Warning: point[" << _size << "] " << dim + 1 << " feature is not a number." << endl;
                dim--;
            }
            if(type == "Classification" || type == "MultiClassification" || type == "BinClassification") {
                if (dim == 0 && !flag) {
                    if (!((item == pos_class) || (item == neg_class))) {
                        atEnd = true;
                        flag = true;
                    }
                } else if (ss.eof() && !flag) {
                    if (!((item == pos_class) || (item == neg_class))) {
                        atBegin = true;
                        flag = true;
                    }
                }
            }
            dim++;
        }

        if(ldim != dim && ldim != 0){
            cerr << "All the samples must have the same dimension!" << endl;
            return false;
        }

        ldim = dim;
        _size++;
        ss.clear();
    }
    input.clear();
    input.seekg(0, ios::beg);

    //initialize dim and _size
    dim--;
    this->dim = dim;
    this->size = _size;

    //reserve memory for fnames array and set feature names
    fnames.assign(dim, 0);
    index.assign(_size, 0);
    iota(fnames.begin(), fnames.end(), 1);
    iota(index.begin(), index.end(), 0);

    //reserve memory for points array
    points.resize(_size);

    _size = 0;

    //Read line (sample) from file
    while(getline(input, str)){
        auto new_point = make_shared<Point< T > >();
        dim = -1;
        ss.str(str);

        //reserve memory for features
        new_point->x.assign(this->dim, 0.0);

        //Read features from line
        while(getline(ss, item, ',')){
            if(atEnd)
                cond = (!ss.eof() && atEnd);
            else
                cond = dim != 0;

            if(cond){
                if(Utils::is_number(item)){
                    new_point->x[dim + 1] = Utils::atod(item.c_str());
                }

            }else{
                if(type == "Classification" || type == "MultiClassification" || type == "BinClassification"){
                    c = process_class(item);
                }
                new_point->y = c;
            }
            dim++;
        }

        points[_size++] = std::move(new_point);
        points[_size - 1]->id = _size;
        ss.clear();
    }

    is_empty = false;

    return true;
}

template < typename T >
bool Data< T >::load_txt(const string& path){
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
            cerr << _dim << " " << this->dim << endl;
            cerr << "All the samples must have the same dimension!" << endl;
            return false;
        }

        d = _dim;
        _size++;
        ss.clear();
    }

    //Initialize _size and _dim
    this->size = _size;
    this->dim = _dim;

    //Set features names
    fnames.assign(_dim, 0);
    index.assign(_size, 0);
    iota(fnames.begin(), fnames.end(), 1);
    iota(index.begin(), index.end(), 0);

    input.clear();
    input.seekg(0, ios::beg);

    //Allocate memory for points array
    points.resize(_size);

    _size = 0;

    //get line from file (sample)
    while(getline(input, str)){
        auto new_point = make_shared<Point< T > >();

        //Allocate memory for features
        new_point->x.resize(_dim, 0.0);
        ss.str(str);
        n = 0;

        //read features from line
        while(getline(ss, item, ' ')){
            if(n >= 2){
                if(Utils::is_number(item))
                    new_point->x[n - 2] = Utils::atod(item.c_str());
                else{ clog << "Warning: point[" << _size << "] " << n - 2 << " feature is not a number." << endl; }
                new_point->y = 0;
            }
            n++;
        }

        points[_size++] = std::move(new_point);
        points[_size - 1]->id = _size;
        ss.clear();
    }

    is_empty = false;

    return true;
}

template < typename T >
bool Data< T >::removePoint(int pid){
    int i;

    if(size == 1){ cout << "Error: RemovePoint, only one point left\n"; return false; }
    //Ids bound verification
    if(pid > points[size-1]->id || pid <= 0) return false;

    if(!index.empty()){
        index.resize(size);

        for(i = 0; i < size; ++i){
            if(i >= pid){
                index[i-1] = index[i] - 1;
            }
        }
    }

    //Find the point by its id and erase it
    for(i = 0; i < size; i++){
        if(points[i]->id == pid){
            if(stats.n_pos > 0 || stats.n_neg > 0){
                if(points[i]->y == 1) stats.n_pos--;
                else if(points[i]->y == -1) stats.n_neg--;
            }
            points.erase(points.begin() + i);
            break;
        }
    }

    size--;

    return true;
}

template < typename T >
void Data< T >::write(const string& fname, string ext){
    int i, j;
    string path = fname + "." + ext;
    ofstream outstream(path.c_str(), ios::out);

    if(!outstream.is_open()){
        cerr << "Can't write in file." << endl;
        return;
    }

    for(i = 0; i < size; i++){
        if(ext == "plt"){
            outstream << points[i]->y << " ";
            for(j = 0; j < dim-1; j++){
                outstream << points[i]->x[j] << " ";
            }
            outstream << points[i]->x[j] << endl;
        }else if(ext == "data"){
            outstream << points[i]->y << " ";
            for(j = 0; j < dim-1; j++){
                outstream << fnames[j] << ":" << points[i]->x[j] << " ";
            }
            outstream << fnames[j] << ":" << points[i]->x[j] << "\n";
        }else if(ext == "csv"){
            outstream << points[i]->y << ",";
            for(j = 0; j < dim-1; j++){
                outstream << points[i]->x[j] << ",";
            }
            outstream << points[i]->x[j] << "\n";
        }
    }

    outstream.close();
}

template < typename T >
vector<bool> Data< T >::removePoints(vector<int> ids){
    int idsize = ids.size(), i;
    bool save;
    shared_ptr<Point< T > > po;
    auto p = points.begin();
    vector<bool> notFound(idsize, true);

    for(; p != points.end();){
        save = true;
        po = (*p);
        for(i = 0; i < idsize; i++){
            if(po->id == ids[i]){
                save = false;
                notFound[i] = false;
                break;
            }
        }

        if(save) p++;
        else{
            p = points.erase(p);
            //Size verification.
            if(size == 1){ clog << "Error: RemovePoint, only one point left." << endl; break;}
            if(po->y == 1) stats.n_pos--;
            else if(po->y == -1) stats.n_neg--;
            size--;
        }
    }

    return notFound;
}

template < typename T >
Data< T >* Data< T >::insertFeatures(std::vector<int> ins_feat){
    size_t i, j, s, offset = 0, fsize = ins_feat.size();
    bool saveflag = false;
    vector<int> new_fnames(fsize, 0);
    shared_ptr<Point< T > > p;
    auto *smout = new Data< T >;

    if(fsize == 0) return this;
    sort(ins_feat.begin(), ins_feat.end());

    //error check
    if(fsize > dim){ cerr << "Error: InsertFeature, fsize(" << ins_feat.size() << ")>dim(" << dim << ")\n"; return smout; }
    smout->setDim(fsize);

    //Copying information to new data array
    for(i = 0; i < size; i++){
        p = make_shared<Point< T > >();
        p->x.resize(fsize);
        p->alpha = points[i]->alpha;
        p->id = points[i]->id;
        p->y = points[i]->y;

        //Copying features
        s = 0, offset = 0;
        for(j = 0; j < dim; j++){
            if(offset < fsize && fnames[j] == ins_feat[offset]){
                saveflag = true;
                offset++;
            }

            if(saveflag){
                p->x[s] = points[i]->x[j];
                new_fnames[s] = fnames[j];
                s++;
                saveflag = false;
            }
        }
        //error check
        if(s != fsize){
            cerr << "Error: Something went wrong on InsertFeature\n";
            cerr << "s = " << s << ", dim = " << dim << ", fsize = " << fsize << endl;
            smout->clear();
            return smout;
        }
        smout->insertPoint(p);
    }
    smout->setFeaturesNames(new_fnames);

    return smout;
}

template < typename T >
void Data< T >::shuffle(const size_t &seed){
    std::mt19937 gen(seed);
    std::uniform_int_distribution<size_t> dist(0, size-1);

    for(auto it = points.begin(); it != points.end(); it++){
        auto pos = points.begin() + dist(gen);
        auto temp = (*it)->id;
        (*it)->id = (*pos)->id;
        (*pos)->id = temp;
        std::iter_swap(it, pos);
    }
}

template < typename T >
bool Data< T >::removeFeatures(std::vector<int> feats){
    size_t i, j, k, psize = points.size(), rsize = feats.size();
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
        for(j = 0; j < dim; j++){
            if(feats[i] == fnames[j]){
                break;
            }
        }
        if(j == dim){
            exist[i] = false;
        }
    }

    //Remove features from each point
    for(i = 0; i < psize; i++){
        if(points[i] == nullptr) clog << "WARNING: point is null." << endl;

        // Iterate through the point features
        for(itr = points[i]->x.begin(),k = 0, j = 0; itr != points[i]->x.end();){
            while(!exist[k] && k < rsize) k++; // go to next existent feature
            if(k == rsize) break;              // Verify if is in the end of the feats vector

            // Feature to remove found, remove it from the point and go to the next feat to remove
            if(fnames[j] == feats[k]){
                itr = points[i]->x.erase(itr);
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
                dim--;
                break;
            }else{
                fitr++;
            }
        }
    }

    return true;
}

template < typename T >
bool Data< T >::insertPoint(Data< T > sample, int _index){
    if(_index > sample.getSize()-1){
        cerr << "Index out of bounds. (insertPoint)" << endl;
        return false;
    }

    insertPoint(sample.getPoint(_index));

    return true;
}

template < typename T >
bool Data< T >::insertPoint(shared_ptr<Point< T > > p){
    //Dimension verification
    if(size > 0 && int(p->x.size()) > dim){
        cerr << "Point with dimension different from the data. (insertPoint)" << endl;
        cerr << "Point dim = " << p->x.size() << " dim = " << dim << endl;
        return false;
    }

    if(size == 0) dim = p->x.size();
    //Insert the point p at the end of the points vector
    points.insert(points.end(), std::move(p));
    size++;
    if(is_empty) is_empty = false;
    
    if(this->isClassification()){
        auto class_pos = std::find(this->classes.begin(), this->classes.end(), points[size-1]->y);

        if(class_pos == this->classes.end()){
            this->class_names.push_back(std::to_string(int(points[size-1]->y)));
            this->classes.push_back(points[size-1]->y);
            this->class_distribution.push_back(1);
        }else{
            this->class_distribution[int(class_pos - this->classes.begin())]++;
        }
    }
    //Give a new id to the point equal to the previous point id plus 1
    points[size-1]->id = size;
    index.push_back(size-1);

    return true;
}

template < typename T >
bool Data< T >::insertPoint(Point< T > &p){
    return this->insertPoint(std::make_shared<Point< T > >(p));
}

template < typename T >
void Data< T >::changeXVector(vector<int> _index){
    int i;
    vector<shared_ptr<Point< T > > > nPoints(size);

    //Copy features and classes of the points making the changes
    for(i = 0; i < size; i++){
        nPoints[i]->x = points[_index[i]]->x;
        nPoints[i]->x = points[_index[i]]->x;
    }

    //Save changes in the class
    points = nPoints;
}

template < typename T >
shared_ptr<Point< T > > Data< T >::getPoint(int _index){
    return points[_index];
}

template < typename T >
void Data< T >::setPoint(int _index, shared_ptr<Point< T > > p){
    points[_index] = p;
}

template < typename T >
void Data< T >::copy(const Data<T> &_data){
    size_t _size = _data.getSize();
    this->points.resize(_size);
    for(size_t i = 0; i < _size; i++){
        this->points[i] = std::make_shared<Point< T > >();
        this->points[i]->x = _data[i]->x;
        this->points[i]->y = _data[i]->y;
        this->points[i]->alpha = _data[i]->alpha;
        this->points[i]->id = _data[i]->id;
    }
    this->fnames = _data.getFeaturesNames();
    this->size = _data.getSize();
    this->classes = _data.getClasses();
    this->class_names = _data.getClassNames();
    this->class_distribution = _data.getClassesDistribution();
    this->stats = _data.getStatistics();
    this->dim = _data.getDim();
    this->type = _data.getType();
    this->index = _data.getIndex();
    this->is_empty = _data.isEmpty();
    this->normalized = _data.isNormalized();
    this->time_mult = _data.getTime_mult();
}

template < typename T >
void Data< T >::copyZero(const Data< T >& other){
    fnames = other.fnames;
    dim = other.dim;
    size = 0;
    is_empty = other.is_empty;
    normalized = other.normalized;
}

template < typename T >
void Data< T >::join(std::shared_ptr<Data< T > > data){
    size_t i, j, dim1 = data->getDim(), antsize = size, size1 = data->getSize();
    vector<int> index1 = data->getIndex(), antindex = index;
    vector<shared_ptr<Point< T > > > points1 = data->getPoints();

    if(dim > dim1){
        cerr << "Error: sample1 dimension must be less or equal to sample2\n";
        exit(1);
    }

    size += size1;

    if(!index.empty() && !index1.empty()){
        index.resize(size, 0);
        for(i = 0; i < antsize; i++) index[i] = antindex[i];
        for(i = 0; i < size1; i++) index[i + antsize] = index1[i];
    }

    points.resize(size);

    for(i = antsize, j = 0; i < size && j < size1; i++, j++){
        points[i] = points1[j];
        if(points1[j]->y == 1) stats.n_pos++;
        else if(points1[j]->y == -1) stats.n_neg++;
    }

}

template < typename T >
void Data< T >::normalize(double p){
    int i = 0, j = 0;
    double norm = 0.0;

    for(i = 0; i < size; ++i){
        for(norm = 0, j = 0; j < dim; ++j){
            norm += pow(fabs(points[i]->x[j]),p);
        }
        points[i]->x.resize(dim+1);
        points[i]->x[j] = 1;
        fnames[j] = j+1;
        norm += pow(fabs(points[i]->x[j]),p);
        norm = pow(norm, 1.0/p);
        for(j = 0; j < dim+1; ++j){
            points[i]->x[j] /= norm;
        }
    }
    dim++;
    fnames.push_back(dim);

    normalized = true;
}

template < typename T >
void Data< T >::normalize(vector<double> &v, double q){
    size_t i = 0, dim = v.size();
    double norm = 0.0;

    for(i = 0; i < dim; ++i)
        norm += pow(fabs(v[i]), q);

    norm = pow(norm, 1.0/q);

    for(i = 0; i < dim; ++i)
        v[i] /= norm;
}

template < typename T >
void Data< T >::setDim(size_t _dim){
    this->dim = _dim;
}

template < typename T >
vector<int> Data< T >::getFeaturesNames() const{
    return fnames;
}

template < typename T >
void Data< T >::setFeaturesNames(const std::vector<int>& _fnames){
    this->fnames = _fnames;
}

template < typename T >
vector<shared_ptr<Point< T > > > Data< T >::getPoints(){
    return points;
}

template < typename T >
vector<int> Data< T >::getIndex() const{
    return index;
}

template < typename T >
int Data< T >::getNumberNegativePoints(){
    return stats.n_neg;
}

template < typename T >
int Data< T >::getNumberPositivePoints(){
    return stats.n_pos;
}

template < typename T >
Statistics< T > Data< T >::getStatistics() const{
    return stats;
}

template < typename T >
bool Data< T >::isEmpty() const{
    return is_empty;
}

template < typename T >
Data< T >& Data< T >::operator=(const Data< T >& data){
    points = data.points;
    fnames = data.fnames;
    index = data.index;
    size = data.size;
    dim = data.dim;
    time_mult = data.time_mult;
    pos_class = data.pos_class;
    neg_class = data.neg_class;
    is_empty = data.is_empty;
    normalized = data.normalized;
    stats = data.stats;

    return *this;
}


template < typename T >
void Data< T >::clear(){
    points.clear();
    fnames.clear();
    index.clear();
    classes.clear();
    class_names.clear();
    size = 0;
    dim = 0;
    stats.n_neg = 0;
    stats.n_pos = 0;
    stats.centroid = Point< T >();
    stats.neg_centroid = Point< T >();
    stats.pos_centroid = Point< T >();
    normalized = false;
    is_empty = true;
}

template < typename T >
Data< T >::~Data(){
    this->clear();
}

template < typename T >
void Data< T >::setIndex(std::vector<int> _index) {
    this->index = _index;
}

template < typename T >
void Data< T >::resetIndex(){
    index.assign(size, 0);
    iota(index.begin(), index.end(), 0);
}

template < typename T >
double Data< T >::getTime_mult() const {
    return time_mult;
}

template < typename T >
bool Data< T >::operator==(const Data< T > &rhs) const {
    if(points.size() != rhs.points.size()) return false;

    size_t i, _size = points.size();

    for(i = 0; i < _size; i++){
        if(*points[i] != *rhs.points[i]){
            return false;
        }
    }

    return fnames == rhs.fnames &&
           index == rhs.index &&
           _size == rhs.size &&
           dim == rhs.dim &&
           time_mult == rhs.time_mult &&
           pos_class == rhs.pos_class &&
           neg_class == rhs.neg_class &&
           is_empty == rhs.is_empty &&
           normalized == rhs.normalized;
}

template < typename T >
bool Data< T >::operator!=(const Data< T > &rhs) const {
    return !(rhs == *this);
}

template<typename T>
Data<T>::Data(size_t size, size_t dim, T val) {
    size_t i;

    this->points.resize(size);
    this->fnames.resize(dim);
    this->index.resize(size);

    for(i = 0; i < size; i++){
        this->points[i] = std::make_shared<Point< T > >(dim, val);
        this->points[i]->id = i+1;
    }

    std::iota(fnames.begin(), fnames.end(), 1);
    std::iota(index.begin(), index.end(), 0);
    this->size = size;
    this->dim = dim;
    this->is_empty = false;
}

template<typename T>
void Data<T>::setType(const string &_type) {
    this->type = _type;
}

template<typename T>
const string &Data<T>::getType() const {
    return type;
}

template<typename T>
int Data<T>::process_class(std::string item) {
    int c = 0;
    auto class_name_it = std::find(this->class_names.begin(), this->class_names.end(), item);
    
    if(class_name_it == class_names.end()){
        this->class_names.push_back(item);
        if(item != "-1"){
            c = this->class_names.size();
        }else{
            c = -1;
        }
    }else{
        if(Utils::is_number(item)) {
            c = std::stoi(item);
        }else{
            c = (int)(class_name_it - this->class_names.begin())+1;
        }
    }

    auto class_it = std::find(this->classes.begin(), this->classes.end(), c);
    if(class_it == this->classes.end()){
        this->classes.push_back(c);
        this->class_distribution.push_back(0);
    }
    auto it_pos = std::find_if(this->classes.begin(), this->classes.end(), [&c](const int &_class){
        return (_class == c);
    });
    std::cout << c << " " << item << " " << (it_pos-this->classes.begin()) << std::endl;
    this->class_distribution[(it_pos-this->classes.begin())]++;
    
    return c;
}

template<typename T>
std::vector<std::string> Data<T>::getClassNames() const{
    return this->class_names;
}

template<typename T>
std::vector<size_t> Data<T>::getClassesDistribution() const{
    return this->class_distribution;
}

template<typename T>
const std::vector<int> Data<T>::getClasses() const {
    return this->classes;
}

template<typename T>
void Data<T>::setClasses(const vector<int> &_classes) {
    this->classes = _classes;
}

template class Data<int>;
template class Data<double>;
template class Data<float>;
template class Data<int8_t>;
template class Data<char>;
template class Data<long long int>;
template class Data<short int>;
template class Data<long double>;
template class Data<unsigned char>;
template class Data<unsigned int>;
template class Data<unsigned short int>;
