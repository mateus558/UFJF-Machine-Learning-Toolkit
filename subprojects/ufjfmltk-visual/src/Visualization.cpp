#include "ufjfmltk/visual/Visualization.hpp"
#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>
namespace mltk{
    namespace visualize{
    using namespace std;

    template < typename T >
    Visualization< T >::Visualization()= default;

    template < typename T >
    Visualization< T >::Visualization(Data<T> &sample){
        samples = &sample;
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
    vector<string> Visualization< T >::createTempFiles(){
        size_t i, j, k, size = samples->size(), dim = samples->dim();
        vector<string> file_names;
        if(samples->getType() == "Classification") {
            vector<std::string> class_names = samples->classesNames();
            vector<int> classes = samples->classes();
            vector<ofstream> temp_files(class_names.size());
            for(i = 0; i < class_names.size(); i++){
                std::string file_name = std::string("temp/")+class_names[i]+std::string(".plt");
                temp_files[i].open(file_name);
                if(!temp_files[i].is_open()){
                    std::cerr << "Error opening the file " + file_name + "." << std::endl;
                }else{
                    file_names.push_back(file_name);
                }
            }
            for (i = 0; i < size; i++) {
                for(j = 0; j < classes.size(); j++){
                    if (samples->point(i)->Y() == classes[j]) {
                        for (k = 0; k < dim - 1; k++) {
                            temp_files[j] << (double) (samples->point(i)->X()[k]) << " ";
                        }
                        temp_files[j] << (double) (samples->point(i)->X()[k]) << endl;
                    }
                }
            }
            for(i = 0; i < temp_files.size(); i++){
                temp_files[i].close();
            }
        } else{
            ofstream samples_file("temp/samples.plt");

            for (i = 0; i < size; i++) {
                for (j = 0; j < dim - 1; j++) {
                    samples_file << (double) (samples->point(i)->X()[j]) << " ";
                }
                samples_file << (double) (samples->point(i)->X()[j]) << endl;
            }

            samples_file.close();
            file_names.emplace_back("temp/samples.plt");
        }
        return file_names;
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
        if(dpdf != nullptr){
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
            remove(string("temp/" + file).c_str());
        }
    }

    template < typename T >
    void Visualization< T >::plot2D(int x, int y){
        string dims = utils::itos(x) + ":" + utils::itos(y);
        string cmd("plot ");
        vector<string> temp_files_names, class_names = samples->classesNames();
        size_t i;

        temp_files_names = createTempFiles();

        if(samples->getType() == "Classification"){
            for(i = 0; i < class_names.size() - 1; i++){
                cmd += "\'" + temp_files_names[i] + "\' using " + dims + " title \'" + class_names[i] + "\' with points, ";
            }
            cmd += "\'" + temp_files_names[i] + "\' using " + dims + " title \'" + class_names[i] + "\' with points";
        }else{
            cmd = "plot 'temp/samples.plt' using " + dims + " with points";
        }
    #ifdef __unix__
        cmd = "set terminal qt; " + cmd;
        g.cmd(cmd);
    #elif _WIN32
        cmd = "set terminal windows; " + cmd;
        cmd = "echo " + cmd + " | gnuplot -persist";
            system(cmd.c_str());
    #endif
        std::cout << cmd << std::endl;
    }

    template < typename T >
    void Visualization< T >::plot3D(int x, int y, int z){
        string dims = utils::itos(x) + ":" + utils::itos(y) + ":" + utils::itos(z);
        string cmd("splot ");
        vector<string> temp_files_names, class_names = samples->classesNames();
        size_t i;

        temp_files_names = createTempFiles();

        if(samples->getType() == "Classification"){
            for(i = 0; i < class_names.size() - 1; i++){
                cmd += "\'" + temp_files_names[i] + "\' using " + dims + " title \'" + class_names[i] + "\' with points, ";
            }
            cmd += "\'" + temp_files_names[i] + "\' using " + dims + " title \'" + class_names[i] + "\' with points";
        }else if(samples->getType() == "Regression"){
            cmd = "splot 'temp/samples.plt' using " + dims + " with points";
        }
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

        string feats = utils::itos(x) + ":" + utils::itos(y);
        string fx, gx, hx, cmd;
        vector<string> temp_files_names, class_names = samples->classesNames();
        size_t i;

        temp_files_names = createTempFiles();

        if(s.bias != 0) {
            fx = "f(x) = " + utils::dtoa(s.w[x - 1] / -s.w[y - 1]) + "*x + " +
                            utils::dtoa(s.bias / -s.w[y - 1]);
            gx = "g(x) = " + utils::dtoa(s.w[x - 1] / -s.w[y - 1]) + "*x + " +
                            utils::dtoa((s.bias + s.margin * s.norm) / -s.w[y - 1]);
            hx = "h(x) = " + utils::dtoa(s.w[x - 1] / -s.w[y - 1]) + "*x + " +
                            utils::dtoa((s.bias - s.margin * s.norm) / -s.w[y - 1]);
        }else{
            fx = "f(x) = " + utils::dtoa(s.w[x - 1] / s.w[y - 1]) + "*x";
            gx = "g(x) = " + utils::dtoa(s.w[x - 1] / s.w[y - 1]) + "*x";
            hx = "h(x) = " + utils::dtoa(s.w[x - 1] / s.w[y - 1]) + "*x";
        }

        cmd = fx + "; "+ gx +"; "+ hx +"; plot ";

        if(samples->getType() == "Classification"){
            for(i = 0; i < class_names.size() - 1; i++){
                cmd += "\'" + temp_files_names[i] + "\' using " + feats + " title \'" + class_names[i] + "\' with points, ";
            }
            cmd += "\'" + temp_files_names[i] + "\' using " + feats + " title \'" + class_names[i] + "\' with points, f(x) notitle with lines ls 1, g(x) notitle with lines ls 2, h(x) notitle with lines ls 2";
        }else if(samples->getType() == "Regression"){
            cmd += "'temp/samples.plt' using "+feats+" title '+1' with points, f(x) notitle with lines ls 1, g(x) notitle with lines ls 2, h(x) notitle with lines ls 2";
        }
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
        string feats = utils::itos(x) + ":" + utils::itos(y) + ":" + utils::itos(z);
        string fxy, cmd;
        vector<string> temp_files_names, class_names = samples->classesNames();
        size_t i;

        temp_files_names = createTempFiles();
        fxy = "f(x,y) = "+utils::dtoa(s.w[x-1]/-s.w[z-1])+"*x + "+utils::dtoa(s.w[y-1]/-s.w[z-1])+"*y + "+utils::dtoa(s.bias/-s.w[z-1]);
        cmd = fxy + "; splot ";

        if(samples->getType() == "Classification"){
            for(i = 0; i < class_names.size() - 1; i++){
                cmd += "\'" + temp_files_names[i] + "\' using " + feats + " title \'" + class_names[i] + "\' with points, ";
            }
            cmd += "\'" + temp_files_names[i] + "\' using " + feats + " title \'" + class_names[i] + "\' with points, f(x,y) notitle with lines ls 1";

        }else if(samples->getType() == "Regression"){
            cmd += "'temp/samples.plt' using "+ feats +" with points, f(x,y) notitle with lines ls 1";
        }
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
}}