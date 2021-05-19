#include "ufjfmltk/visual/Visualization.hpp"
#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>

#define PLOT_FOLDER "temp"

namespace mltk{
    namespace visualize{
        using namespace std;
        namespace fs = std::filesystem;
        template<typename T>
        size_t Visualization<T>::n_plots=0;
        template < typename T >
        Visualization< T >::Visualization(bool shared_session): is_shared(shared_session) {
            configs["terminal"] = "wxt";
            if(shared_session) g = new Gnuplot();
        }

        template < typename T >
        Visualization< T >::Visualization(Data<T> &sample, bool shared_session): is_shared(shared_session) {
            samples = &sample;
            configs["terminal"] = "wxt";
            if(shared_session) g = new Gnuplot();
            plot_folder = PLOT_FOLDER+std::to_string(n_plots)+"/";
            n_plots++;
            if(!fs::exists(plot_folder)) {
                fs::create_directory(plot_folder);
            }
            createTempFiles();
        }

        template < typename T >
        void Visualization< T >::setTitle(string title){
            configs["title"] = title;
        }

        template < typename T >
        void Visualization< T >::setStyle(string style){
            configs["style"] = style;
        }

        template < typename T >
        vector<string> Visualization< T >::createTempFiles(){
            size_t i, j, k, size = samples->size(), dim = samples->dim();
            vector<string> file_names;

            if(samples->isClassification()) {
                vector<std::string> class_names = samples->classesNames();
                vector<int> classes = samples->classes();
                vector<ofstream> temp_files(class_names.size());

                for(i = 0; i < class_names.size(); i++){
                    std::string file_name = std::string(plot_folder)+class_names[i]+std::string(".plt");
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
                ofstream samples_file(std::string(plot_folder) + "samples.plt");

                for (i = 0; i < size; i++) {
                    for (j = 0; j < dim - 1; j++) {
                        samples_file << (double) (samples->point(i)->X()[j]) << " ";
                    }
                    samples_file << (double) (samples->point(i)->X()[j]) << endl;
                }

                samples_file.close();
                file_names.emplace_back(std::string(plot_folder) + "samples.plt");
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
        vector<string> Visualization< T >::getTempFilesNames(bool append_path){
            vector<string> files;

        #ifdef __unix__
            DIR *dpdf;
            struct dirent *epdf;
            string path = string(plot_folder);

            dpdf = opendir(path.c_str());
            if(dpdf != nullptr){
                while((epdf = readdir(dpdf))){
                    string file = string(epdf->d_name);
                    if(valid_file(file) && !file.empty()){
                        files.push_back((append_path)?(path+file):file);
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
                remove(string(plot_folder + file).c_str());
            }
        }

        template < typename T >
        void Visualization< T >::plot2D(int x, int y, bool save, const std::string& title, const std::string& format,
                                        const std::string& x_label, const std::string& y_label){
            string dims = utils::itos(x) + ":" + utils::itos(y);
            string cmd("plot ");
            vector<string> temp_files_names, class_names = samples->classesNames();
            size_t i;
            std::string out_name = (this->samples->name().empty())?"2dplot_"+utils::itos(x)+"_"+utils::itos(y):
                                   this->samples->name()+"_2d_"+utils::itos(x)+"_"+utils::itos(y);

            configurePlot(out_name, format, title, save, x_label, y_label);

            if(samples->isClassification()){
                temp_files_names = getTempFilesNames(true);
                for(i = 0; i < class_names.size() - 1; i++){
                    cmd += "\'" + temp_files_names[i] + "\' using " + dims + " title \'" + class_names[i] + "\' with points, ";
                }
                cmd += "\'" + temp_files_names[i] + "\' using " + dims + " title \'" + class_names[i] + "\' with points";
                cmd = fetchConfigs() + cmd;
    #ifdef __unix__
                if(is_shared) g->cmd(cmd);
                else {
                    Gnuplot g_;
                    g_.cmd(cmd);
                }
    #elif _WIN32
                cmd = "set terminal windows; " + cmd;
                cmd = "echo " + cmd + " | gnuplot -persist";
                system(cmd.c_str());
    #endif
            }else{
                if(is_shared) g->plot_xy(samples->getFeature(x), samples->getFeature(y));
                else {
                    Gnuplot g_;
                    g_.plot_xy(samples->getFeature(x), samples->getFeature(y));
                }
            }
        }

        template < typename T >
        void Visualization< T >::plot3D(int x, int y, int z, bool save, const std::string& title,
                                        const std::string& format,
                                        const std::string& x_label, const std::string& y_label, const std::string& z_label){
            string dims = utils::itos(x) + ":" + utils::itos(y) + ":" + utils::itos(z);
            string cmd("splot ");
            vector<string> temp_files_names, class_names = samples->classesNames();
            size_t i;
            std::string out_name = (this->samples->name().empty())?"3dplot_"+utils::itos(x)+"_"+utils::itos(y)+"_"+utils::itos(z):
                                   this->samples->name()+"_3d_"+utils::itos(x)+"_"+utils::itos(y)+"_"+utils::itos(z);

            configurePlot(out_name, format, title, save, x_label, y_label, z_label);

            if(samples->isClassification()){
                temp_files_names = getTempFilesNames(true);
                for(i = 0; i < class_names.size() - 1; i++){
                    cmd += "\'" + temp_files_names[i] + "\' using " + dims + " title \'" + class_names[i] + "\' with points, ";
                }
                cmd += "\'" + temp_files_names[i] + "\' using " + dims + " title \'" + class_names[i] + "\' with points";
                cmd = fetchConfigs() + cmd;
                #ifdef __unix__
                if(is_shared) g->cmd(cmd);
                else {
                    Gnuplot g_;
                    g_.cmd(cmd);
                }
                #elif _WIN32
                cmd = "echo " + cmd + " | gnuplot -persist";
                system(cmd.c_str());
                #endif
            }else if(samples->getType() == "Regression"){
                if(is_shared) g->plot_xyz(this->samples->getFeature(x), this->samples->getFeature(y), this->samples->getFeature(z));
                else {
                    Gnuplot g_;
                    g_.plot_xyz(this->samples->getFeature(x), this->samples->getFeature(y), this->samples->getFeature(z));
                }
            }
        }

        template < typename T >
        void Visualization< T >::plot2DwithHyperplane(int x, int y, Solution s, bool save, const std::string& title,
                                                      const std::string& format,
                                                      const std::string& x_label, const std::string& y_label){
            if(s.norm != s.norm) s.norm = 0.0;
            string feats = utils::itos(x) + ":" + utils::itos(y);
            string fx, gx, hx, cmd;
            vector<string> temp_files_names, class_names = samples->classesNames();
            size_t i;
            std::string out_name = (this->samples->name().empty())?"2dplotsol_"+utils::itos(x)+"_"+utils::itos(y):
                                   this->samples->name()+"_2dsol_"+utils::itos(x)+"_"+utils::itos(y);

            configurePlot(out_name, format, title, save, x_label, y_label);

            if(s.bias != 0) {
                fx = "f(x) = " + utils::dtoa(s.w[x] / -s.w[y]) + "*x + " +
                                utils::dtoa(s.bias / -s.w[y]);
                gx = "g(x) = " + utils::dtoa(s.w[x] / -s.w[y]) + "*x + " +
                                utils::dtoa((s.bias + s.margin * s.norm) / -s.w[y]);
                hx = "h(x) = " + utils::dtoa(s.w[x] / -s.w[y]) + "*x + " +
                                utils::dtoa((s.bias - s.margin * s.norm) / -s.w[y]);
            }else{
                fx = "f(x) = " + utils::dtoa(s.w[x] / s.w[y]) + "*x";
                gx = "g(x) = " + utils::dtoa(s.w[x] / s.w[y]) + "*x";
                hx = "h(x) = " + utils::dtoa(s.w[x] / s.w[y]) + "*x";
            }

            cmd = fx + "; "+ gx +"; "+ hx +"; plot ";

            if(samples->isClassification()){
                temp_files_names = getTempFilesNames(true);
                for(i = 0; i < class_names.size() - 1; i++){
                    cmd += "\'" + temp_files_names[i] + "\' using " + feats + " title \'" + class_names[i] + "\' with points, ";
                }
                cmd += "\'" + temp_files_names[i] + "\' using " + feats + " title \'" + class_names[i] + "\' with points, f(x) notitle with lines ls 1, g(x) notitle with lines ls 2, h(x) notitle with lines ls 2";
            }else if(samples->getType() == "Regression"){
                cmd += "'"+ std::string(plot_folder) +"samples.plt' using "+feats+" title '+1' with points, f(x) notitle with lines ls 1, g(x) notitle with lines ls 2, h(x) notitle with lines ls 2";
            }
        #ifdef __unix__
            cmd = fetchConfigs() + cmd;
            if(is_shared) {
                g->cmd(cmd);
            }else {
                Gnuplot g_;
                g_.cmd(cmd);
            }
        #elif _WIN32
            cmd = "set terminal windows; " + cmd;
            cmd = "echo " + cmd + " | gnuplot -persist";
                system(cmd.c_str());
        #endif
        }

        template < typename T >
        void Visualization< T >::plot3DwithHyperplane(int x, int y, int z, Solution s, bool save, const std::string& title,
                                                      const std::string& format,
                                                      const std::string& x_label, const std::string& y_label,
                                                      const std::string& z_label){
            string feats = utils::itos(x) + ":" + utils::itos(y) + ":" + utils::itos(z);
            string fxy, cmd;
            vector<string> temp_files_names, class_names = samples->classesNames();
            size_t i;
            std::string out_name = (this->samples->name().empty())?"3dplotsol_"+utils::itos(x)+"_"+utils::itos(y)+"_"+utils::itos(z):
                                   this->samples->name()+"_3dsol_"+utils::itos(x)+"_"+utils::itos(y)+"_"+utils::itos(z);

            configurePlot(out_name, format, title, save, x_label, y_label, z_label);

            fxy = "f(x,y) = "+utils::dtoa(s.w[x]/s.w[z])+"*x + "+utils::dtoa(s.w[y]/s.w[z])+"*y + "+utils::dtoa(s.bias/s.w[z]);
            cmd = fxy + "; splot ";

            if(samples->isClassification()){
                temp_files_names = getTempFilesNames(true);
                for(i = 0; i < class_names.size() - 1; i++){
                    cmd += "\'" + temp_files_names[i] + "\' using " + feats + " title \'" + class_names[i] + "\' with points, ";
                }
                cmd += "\'" + temp_files_names[i] + "\' using " + feats + " title \'" + class_names[i] + "\' with points, f(x,y) notitle with lines ls 1";

            }else if(samples->getType() == "Regression"){
                cmd += "'"+ std::string(plot_folder) +"samples.plt' using "+ feats +" with points, f(x,y) notitle with lines ls 1";
            }
        #ifdef __unix__
            cmd = fetchConfigs() + cmd;
            if(is_shared) {
                g->cmd(cmd);
            }else {
                Gnuplot g_;
                g_.cmd(cmd);
            }
        #elif _WIN32
            cmd = "set terminal windows; " + cmd;
            cmd = "echo " + cmd + " | gnuplot -persist";
                system(cmd.c_str());
        #endif
        }

        template < typename T >
        void Visualization< T >::setSample(Data<T>& sample) {
            this->samples = &sample;
            if(fs::exists(plot_folder)) {
                removeTempFiles();
                fs::remove(plot_folder);
            }
            plot_folder = PLOT_FOLDER+std::to_string(n_plots)+"/";
            n_plots++;
            if(!fs::exists(plot_folder)) {
                fs::create_directory(plot_folder);
            }
            createTempFiles();
        }

        template<typename T>
        void
        Visualization<T>::configurePlot(const string &outname, const string &format, const string &title, bool save,
                                        const std::string& x_label, const std::string& y_label, const std::string& z_label) {
            configs["save"] = (save)?"true":"false";
            configs["output_name"] = outname;
            configs["output_format"] = format;
            configs["title"] = (title.empty())?"":title;
            configs["x_label"] = (x_label.empty())?"":x_label;
            configs["y_label"] = (y_label.empty())?"":y_label;
            configs["z_label"] = (z_label.empty())?"":z_label;
        }

        template<typename T>
        std::string Visualization<T>::fetchConfigs() {
            std::string confs;
            if(configs["save"] == "true") {
                confs += std::string("set terminal ") + configs["output_format"]+"; ";
                confs += std::string("set output '") + configs["output_name"] +"." + configs["output_format"]+"'; ";
            }else if(!configs["terminal"].empty()){
                confs += std::string("set terminal ") + configs["terminal"]+"; ";
            }else {
                confs += std::string("set terminal x11; ");
            }

            if(!configs["title"].empty()) {
                confs += std::string("set title '") + configs["title"] + "';";
            }
            if(!configs["x_label"].empty()) {
                confs += std::string("set xlabel '") + configs["x_label"] + "';";
            }
            if(!configs["y_label"].empty()) {
                confs += std::string("set ylabel '") + configs["y_label"] + "';";
            }
            if(!configs["z_label"].empty()) {
                confs += std::string("set zlabel '") + configs["z_label"] + "';";
            }
            return confs;
        }

        template < typename T >
        Visualization< T >::~Visualization(){
            this->samples = nullptr;
            delete g;
            g = nullptr;
            removeTempFiles();
            fs::remove(plot_folder);
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
    }
}