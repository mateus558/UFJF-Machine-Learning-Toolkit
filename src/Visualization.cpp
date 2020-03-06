#include "../includes/Visualization.hpp"
#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

template < typename T >
Visualization< T >::Visualization(){
}

template < typename T >
Visualization< T >::Visualization(Data< T > *sample){
    samples = sample;
}

template < typename T >
void Visualization< T >::setTitle(string title){
#ifdef __unix__
    g.set_title(title);
#elif _WIN32
#endif
}

template < typename T >
void Visualization< T >::setStyle(string style){
#ifdef __unix__
    g.set_style(style);
#elif _WIN32
#endif
}

template < typename T >
void Visualization< T >::createPosNegTemps(){
    size_t i, j, size = samples->getSize(), dim = samples->getDim();
    if(samples->getType() == "Classification") {
        ofstream neg_file("temp/neg.plt"), pos_file("temp/pos.plt"), und_file("temp/und.plt");

        for (i = 0; i < size; i++) {
            if (samples->getPoint(i)->y == 1) {
                for (j = 0; j < dim - 1; j++) {
                    pos_file << (double) (samples->getPoint(i)->x[j]) << " ";
                }
                pos_file << (double) (samples->getPoint(i)->x[j]) << endl;
            } else if (samples->getPoint(i)->y == -1) {
                for (j = 0; j < dim - 1; j++) {
                    neg_file << (double) (samples->getPoint(i)->x[j]) << " ";
                }
                neg_file << (double) (samples->getPoint(i)->x[j]) << endl;
            } else {
                for (j = 0; j < dim - 1; j++) {
                    und_file << (double) (samples->getPoint(i)->x[j]) << " ";
                }
                und_file << (double) (samples->getPoint(i)->x[j]) << endl;
            }
        }
        pos_file.close();
        neg_file.close();
        und_file.close();
    } else{
        ofstream samples_file("temp/samples.plt");

        for (i = 0; i < size; i++) {
            for (j = 0; j < dim - 1; j++) {
                samples_file << (double) (samples->getPoint(i)->x[j]) << " ";
            }
            samples_file << (double) (samples->getPoint(i)->x[j]) << endl;
        }

        samples_file.close();
    }
}

template < typename T >
bool Visualization< T >::valid_file(string file){
    size_t i;
    bool flag = false;
    string ext;

    if(file.empty() || file.size() < 4)
        return false;

    for(i = file.size()-1; i >= 0; i--){
        if(file[i] == '.') break;
        string f(1, file[i]);
        ext = f + ext;
    }

    for(string type : types){
        if(type == ext) flag = true;
    }

    return flag;
}

template < typename T >
vector<string> Visualization< T >::getTempFilesNames(){
    vector<string> files;

#ifdef __unix__
    DIR *dpdf;
    struct dirent *epdf;
    string path = string("temp");

    dpdf = opendir(path.c_str());
    if(dpdf != NULL){
        while((epdf = readdir(dpdf))){
            string file = string(epdf->d_name);
            if(valid_file(file) && !file.empty()){
                files.push_back(file);
            }
        }
    }else{
        cout << "Folder not found!" << endl;
    }

    closedir(dpdf);
#elif _WIN32
    HANDLE hFind;
        WIN32_FIND_DATA data;

        hFind = FindFirstFile(".\\temp\\*.*", &data);
        if (hFind != INVALID_HANDLE_VALUE) {
          do {
            string file_name(data.cFileName);
            if(valid_file(file_name) && !file_name.empty()){
                files.push_back(file_name);
            }
          } while (FindNextFile(hFind, &data));
          FindClose(hFind);
        }
    #else
        cerr << "Can't remove temporary files, please remove manually. (Unsupported System)." << endl;
#endif

    return files;
}

template < typename T >
void Visualization< T >::removeTempFiles(){
    string path;
    vector<string> temps;

    temps = getTempFilesNames();

    for(string file : temps){
        path = file;
        remove(path.c_str());
    }
}

template < typename T >
void Visualization< T >::plot2D(int x, int y){
    string dims = Utils::itos(x) + ":" + Utils::itos(y);
    string cmd;
    if(samples->getType() == "Classification"){
        cmd = "plot 'temp/pos.plt' using " + dims + " title '+1' with points, 'temp/neg.plt' using " + dims + " title '-1' with points";
    }else{
        cmd = "plot 'temp/samples.plt' using " + dims + " with points";
    }
    createPosNegTemps();
#ifdef __unix__
    cmd = "set terminal qt; " + cmd;
    g.cmd(cmd);
#elif _WIN32
    cmd = "set terminal windows; " + cmd;
    cmd = "echo " + cmd + " | gnuplot -persist";
        system(cmd.c_str());
#endif
}

template < typename T >
void Visualization< T >::plot3D(int x, int y, int z){
    string dims = Utils::itos(x) + ":" + Utils::itos(y) + ":" + Utils::itos(z);
    string cmd;
    if(samples->getType() == "Classification"){
        cmd = "splot 'temp/pos.plt' using " + dims + " title '+1' with points, 'temp/neg.plt' using " + dims + " title '-1' with points";
    }else if(samples->getType() == "Regression"){
        cmd = "splot 'temp/samples.plt' using " + dims + " with points";
    }
    createPosNegTemps();
#ifdef __unix__
    cmd = "set terminal qt; " + cmd;
    g.cmd(cmd);
#elif _WIN32
    cmd = "echo " + cmd + " | gnuplot -persist";
        system(cmd.c_str());
#endif
}

template < typename T >
void Visualization< T >::plot2DwithHyperplane(int x, int y, Solution s){
    if(s.norm != s.norm) s.norm = 0.0;

    string feats = Utils::itos(x) + ":" + Utils::itos(y);
    string fx, gx, hx, cmd;
    if(s.bias != 0) {
        fx = "f(x) = " + Utils::dtoa(s.w[x - 1] / -s.w[y - 1]) + "*x + " + Utils::dtoa(s.bias / -s.w[y - 1]);
        gx = "g(x) = " + Utils::dtoa(s.w[x - 1] / -s.w[y - 1]) + "*x + " +
                    Utils::dtoa((s.bias + s.margin * s.norm) / -s.w[y - 1]);
        hx = "h(x) = " + Utils::dtoa(s.w[x - 1] / -s.w[y - 1]) + "*x + " +
                    Utils::dtoa((s.bias - s.margin * s.norm) / -s.w[y - 1]);
    }else{
        fx = "f(x) = " + Utils::dtoa(s.w[x - 1] / s.w[y - 1]) + "*x";
        gx = "g(x) = " + Utils::dtoa(s.w[x - 1] / s.w[y - 1]) + "*x";
        hx = "h(x) = " + Utils::dtoa(s.w[x - 1] / s.w[y - 1]) + "*x";
    }

    if(samples->getType() == "Classification"){
        cmd = fx + "; "+ gx +"; "+ hx +"; plot 'temp/pos.plt' using "+feats+" title '+1' with points, 'temp/neg.plt' using "+feats+" title '-1' with points, f(x) notitle with lines ls 1, g(x) notitle with lines ls 2, h(x) notitle with lines ls 2";
    }else if(samples->getType() == "Regression"){
        cmd = fx + "; "+ gx +"; "+ hx +"; plot 'temp/samples.plt' using "+feats+" title '+1' with points, f(x) notitle with lines ls 1, g(x) notitle with lines ls 2, h(x) notitle with lines ls 2";
    }
    createPosNegTemps();

#ifdef __unix__
    cmd = "set terminal qt; " + cmd;
    g.cmd(cmd);
#elif _WIN32
	cmd = "set terminal windows; " + cmd;
    cmd = "echo " + cmd + " | gnuplot -persist";
        system(cmd.c_str());
#endif
}

template < typename T >
void Visualization< T >::plot3DwithHyperplane(int x, int y, int z, Solution s){
    string feats = Utils::itos(x) + ":" + Utils::itos(y) + ":" + Utils::itos(z);
    string fxy;
    string cmd;

    if(samples->getType() == "Classification"){
        fxy = "f(x,y) = "+Utils::dtoa(s.w[x-1]/-s.w[z-1])+"*x + "+Utils::dtoa(s.w[y-1]/-s.w[z-1])+"*y + "+Utils::dtoa(s.bias/-s.w[z-1]);
        cmd = fxy + "; splot 'temp/pos.plt' using "+ feats +" title '+1' with points, 'temp/neg.plt' using "+ feats +" title '-1' with points, f(x,y) notitle with lines ls 1";
    }else if(samples->getType() == "Regression"){
        std::cout << s.w[x-1] << " " << s.w[y-1] << " " << s.w[z-1] << std::endl;
        fxy = "f(x,y) = "+Utils::dtoa(-s.w[x-1]/s.w[z-1])+"*x + "+Utils::dtoa(-s.w[y-1]/s.w[z-1])+"*y + " + Utils::dtoa(s.bias/-s.w[z-1]);
        cmd = fxy + "; splot 'temp/samples.plt' using "+ feats +" with points, f(x,y) notitle with lines ls 1";
    }

    createPosNegTemps();

#ifdef __unix__
    cmd = "set terminal qt; " + cmd;
    g.cmd(cmd);
#elif _WIN32
    cmd = "set terminal windows; " + cmd;
	cmd = "echo " + cmd + " | gnuplot -persist";
        system(cmd.c_str());
#endif
}

template < typename T >
void Visualization< T >::setSample(Data<T> *sample) {
    this->samples = sample;
}

template < typename T >
Visualization< T >::~Visualization(){
#ifdef __unix__
    g.cmd("quit");
#elif _WIN32
#endif
    removeTempFiles();
}

template class Visualization<int>;
template class Visualization<double>;
template class Visualization<float>;
template class Visualization<int8_t>;
template class Visualization<char>;
template class Visualization<long long int>;
template class Visualization<short int>;
template class Visualization<long double>;
template class Visualization<unsigned char>;
template class Visualization<unsigned int>;
template class Visualization<unsigned short int>;
