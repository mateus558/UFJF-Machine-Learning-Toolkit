#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#else
#include "ufjfmltk/visual/Visualization.hpp"
#endif
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
        template<typename T>
        Visualization< T >::Visualization(bool shared_session, bool keep_temp_files):
        is_shared(shared_session),
        keep_temp_files(keep_temp_files) {
            configs["terminal"] = "wxt";
            if(shared_session) g = new Gnuplot();
        }

        template<typename T>
        Visualization< T >::Visualization(Data<T> &sample, bool shared_session, bool keep_temp_files):
        is_shared(shared_session),
        keep_temp_files(keep_temp_files) {
            samples = &sample;
            configs["terminal"] = "wxt";
            if(shared_session) g = new Gnuplot();
            this->plot_folder = PLOT_FOLDER+std::to_string(n_plots)+"/";
            n_plots++;
            bool exist_folder = fs::exists(this->plot_folder);
            if(!exist_folder) {
                fs::create_directory(this->plot_folder);
            }
            createTempFiles();
        }

        template<typename T>
        void Visualization< T >::setTitle(string title){
            configs["title"] = title;
        }

        template<typename T>
        void Visualization< T >::setStyle(string style){
            configs["style"] = style;
        }

        template<typename T>
        vector<string> Visualization< T >::createTempFiles(std::string plot_folder){
            size_t i, j, k, size = samples->size(), dim = samples->dim();
            vector<string> file_names;

            if(plot_folder.empty()) {
                plot_folder = this->plot_folder;
            }

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
                    auto point = (*samples)(i);
                    auto class_pos = std::find(classes.begin(), classes.end(), point.Y()) - classes.begin();
                    auto class_name = class_names[class_pos];
                    int idx = 0;
                    for(const auto& x: point) {
                        if(idx < point.size()-1) {
                            temp_files[class_pos] << x << " ";
                        }else {
                            temp_files[class_pos] << x << std::endl;
                        }
                        idx++;
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

        template<typename T>
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

        template<typename T>
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

        template<typename T>
        void Visualization< T >::removeTempFiles(){
            if(keep_temp_files) return;
            string path;
            vector<string> temps;

            temps = getTempFilesNames();

            for(string file : temps){
                std::string path = plot_folder + file;
                if(fs::exists(path)) fs::remove_all(path);
            }
            if(fs::exists(plot_folder)) fs::remove_all(plot_folder);
            if(!temp_fnames.empty()) {
                for(const auto& file: temp_fnames) {
                    if(fs::exists(file)) {
                        fs::remove(file);
                    }
                }
            }
            temp_fnames.clear();
        }

        template<typename T>
        std::string Visualization< T >::plot2D(int x, int y, bool save, const double scale,
                                        const std::string& title,
                                        const std::string& format,
                                        const std::string& x_label, const std::string& y_label){
            string dims = utils::itos(x+1) + ":" + utils::itos(y+1);
            string cmd("plot ");
            vector<string> temp_files_names, class_names = samples->classesNames();
            Point<int> classes = samples->classes();
            size_t i;
            std::string out_name = (this->samples->name().empty())?"2dplot_"+utils::itos(x)+"_"+utils::itos(y):
                                   this->samples->name()+"_2d_"+utils::itos(x)+"_"+utils::itos(y);

            configureRange(scale, x, y);
            configurePlot(out_name, format, title, save, x_label, y_label);

            if(samples->isClassification()){
                temp_files_names = getTempFilesNames(true);
                auto names = sortLabels(temp_files_names);
                for(i = 0; i < temp_files_names.size() - 1; i++){
                    cmd += "\'" + temp_files_names[i] + "\' using " + dims + " title \'" + names[i] + "\' with points, ";
                }
                cmd += "\'" + temp_files_names[i] + "\' using " + dims + " title \'" + names[i] + "\' with points";
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
            return prepareScript(cmd);
        }

        template<typename T>
        std::string Visualization< T >::plot3D(int x, int y, int z, bool save, const double scale,
                                        const std::string& title,
                                        const std::string& format,
                                        const std::string& x_label, const std::string& y_label, const std::string& z_label){
            string dims = utils::itos(x+1) + ":" + utils::itos(y+1) + ":" + utils::itos(z+1);
            string cmd("splot ");
            vector<string> temp_files_names, class_names = samples->classesNames();
            size_t i;
            std::string out_name = (this->samples->name().empty())?"3dplot_"+utils::itos(x)+"_"+utils::itos(y)+"_"+utils::itos(z):
                                   this->samples->name()+"_3d_"+utils::itos(x)+"_"+utils::itos(y)+"_"+utils::itos(z);

            configureRange(scale, x, y, z);
            configurePlot(out_name, format, title, save, x_label, y_label, z_label);

            if(samples->isClassification()){
                temp_files_names = getTempFilesNames(true);
                auto names = sortLabels(temp_files_names);
                for(i = 0; i < class_names.size() - 1; i++){
                    cmd += "\'" + temp_files_names[i] + "\' using " + dims + " title \'" + names[i] + "\' with points, ";
                }
                cmd += "\'" + temp_files_names[i] + "\' using " + dims + " title \'" + names[i] + "\' with points";
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
            return prepareScript(cmd);
        }

        template<typename T>
        std::string Visualization< T >::plot2DwithHyperplane(int x, int y, Solution s, bool save, const double scale,
                                                      const std::string& title,
                                                      const std::string& format,
                                                      const std::string& x_label, const std::string& y_label){
            if(s.norm != s.norm) s.norm = 0.0;
            string feats = utils::itos(x+1) + ":" + utils::itos(y+1);
            string fx, gx, hx, cmd;
            vector<string> temp_files_names, class_names = samples->classesNames();
            size_t i;
            std::string out_name = (this->samples->name().empty())?"2dplotsol_"+utils::itos(x)+"_"+utils::itos(y):
                                   this->samples->name()+"_2dsol_"+utils::itos(x)+"_"+utils::itos(y);

            configureRange(scale, x, y);
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

            cmd = fx + ";"+ gx +";"+ hx +";plot ";

            if(samples->isClassification()){
                temp_files_names = getTempFilesNames(true);
                auto names = sortLabels(temp_files_names);
                for(i = 0; i < class_names.size() - 1; i++){
                    cmd += "\'" + temp_files_names[i] + "\' using " + feats + " title \'" + names[i] + "\' with points, ";
                }
                cmd += "\'" + temp_files_names[i] + "\' using " + feats + " title \'" + names[i] + "\' with points, f(x) notitle with lines ls 1, g(x) notitle with lines ls 2, h(x) notitle with lines ls 2";
            }else if(samples->getType() == "Regression"){
                cmd += "'"+ std::string(plot_folder) +"samples.plt' using "+feats+" title '+1' with points, f(x) notitle with lines ls 1, g(x) notitle with lines ls 2, h(x) notitle with lines ls 2";
            }
            cmd = fetchConfigs() + cmd;
        #ifdef __unix__
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
            return prepareScript(cmd);
        }

        template<typename T>
        std::string Visualization< T >::plot3DwithHyperplane(int x, int y, int z, Solution s, bool save, const double scale,
                                                      const std::string& title,
                                                      const std::string& format,
                                                      const std::string& x_label, const std::string& y_label,
                                                      const std::string& z_label){
            string feats = utils::itos(x+1) + ":" + utils::itos(y+1) + ":" + utils::itos(z+1);
            string fxy, gxy, hxy, cmd;
            vector<string> temp_files_names, class_names = samples->classesNames();
            size_t i;
            std::string out_name = (this->samples->name().empty())?"3dplotsol_"+utils::itos(x)+"_"+utils::itos(y)+"_"+utils::itos(z):
                                   this->samples->name()+"_3dsol_"+utils::itos(x)+"_"+utils::itos(y)+"_"+utils::itos(z);

            configureRange(scale, x, y, z);
            configurePlot(out_name, format, title, save, x_label, y_label, z_label);

            gxy = "g(x,y) = "+utils::dtoa(-s.w[x]/s.w[z])+"*x + "+utils::dtoa(-s.w[y]/s.w[z])+"*y + "+utils::dtoa((s.bias + s.margin * s.norm)/-s.w[z]);
            fxy = "f(x,y) = "+utils::dtoa(-s.w[x]/s.w[z])+"*x + "+utils::dtoa(-s.w[y]/s.w[z])+"*y + "+utils::dtoa(s.bias/-s.w[z]);
            hxy = "h(x,y) = "+utils::dtoa(-s.w[x]/s.w[z])+"*x + "+utils::dtoa(-s.w[y]/s.w[z])+"*y + "+utils::dtoa((s.bias - s.margin * s.norm)/-s.w[z]);
            cmd = gxy + ";" + fxy + ";" + hxy + "; splot ";

            if(samples->isClassification()){
                temp_files_names = getTempFilesNames(true);
                auto names = sortLabels(temp_files_names);
                for(i = 0; i < class_names.size() - 1; i++){
                    cmd += "\'" + temp_files_names[i] + "\' using " + feats + " title \'" + names[i] + "\' with points, ";
                }
                cmd += "\'" + temp_files_names[i] + "\' using " + feats + " title \'" + names[i] + "\' with points, ";
                cmd += "g(x,y) notitle with lines ls 1 lt rgb \"red\",";
                cmd += "f(x,y) notitle with lines ls 1,";
                cmd += "h(x,y) notitle with lines ls 1 lt rgb \"red\";";
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
            return prepareScript(cmd);
        }

        template<typename T>
        void Visualization< T >::setSample(Data<T>& sample) {
            this->samples = &sample;
            if(fs::exists(plot_folder)) {
                removeTempFiles();
            }
            plot_folder = PLOT_FOLDER+std::to_string(n_plots)+"/";
            n_plots++;
            if(!fs::exists(plot_folder)) {
                fs::create_directory(plot_folder);
            }
            createTempFiles();
        }

        template<typename T>
        std::vector<std::string> Visualization<T>::sortLabels(vector<std::string> &files, const std::string& type) {
            Point<int> classes = samples->classes();
            auto class_names = samples->classesNames();
            std::vector<std::string> names(files.size());
            std::transform(files.begin(), files.end(), names.begin(), [type](std::string& path){
                if(type == "decision") {
                    return mltk::utils::tokenize(path, '_')[0];
                }else{
                    return mltk::utils::tokenize(mltk::utils::tokenize(path, '.')[0], '/')[1];
                }
            });
            std::sort(names.begin(), names.end(),[&class_names, &classes](const auto& a, const auto& b){
                auto aclass_pos = std::find(class_names.begin(), class_names.end(), a) - class_names.begin();
                auto bclass_pos = std::find(class_names.begin(), class_names.end(), b) - class_names.begin();
                return classes[aclass_pos] < classes[bclass_pos];
            });
            std::sort(files.begin(), files.end(), [&class_names, &classes, &type](const auto& a, const auto& b){
                if(type == "decision") {
                    auto aclass_pos = std::find(class_names.begin(), class_names.end(),
                                                mltk::utils::tokenize(a, '_')[0]) - class_names.begin();
                    auto bclass_pos = std::find(class_names.begin(), class_names.end(),
                                                mltk::utils::tokenize(b, '_')[0]) - class_names.begin();
                    return classes[aclass_pos] < classes[bclass_pos];
                }else{
                    auto aclass_pos = std::find(class_names.begin(), class_names.end(),
                                                mltk::utils::tokenize(mltk::utils::tokenize(a, '.')[0], '/')[1]) - class_names.begin();
                    auto bclass_pos = std::find(class_names.begin(), class_names.end(),
                                                mltk::utils::tokenize(mltk::utils::tokenize(b, '.')[0], '/')[1]) - class_names.begin();
                    return classes[aclass_pos] < classes[bclass_pos];
                }
            });
            return names;
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
                confs += std::string("set terminal ") + configs["terminal"]+";";
            }else {
                confs += std::string("set terminal x11;");
            }

            if(!configs["title"].empty()) {
                confs += std::string("set title '") + configs["title"] + "';";
            }
            if(!configs["xrange"].empty()) {
                confs += "set xrange " + configs["xrange"] + ";";
                configs["xrange"].clear();
            }
            if(!configs["yrange"].empty()) {
                confs += "set yrange " + configs["yrange"] + ";";
                configs["yrange"].clear();
            }
            if(!configs["zrange"].empty()) {
                confs += "set zrange " + configs["zrange"] + ";";
                configs["zrange"].clear();
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

        template<typename T>
        Visualization< T >::~Visualization(){
            this->samples = nullptr;
            delete g;
            g = nullptr;
            removeTempFiles();
        }

        template<typename T>
        void Visualization<T>::cmd(const string &command) {
            Gnuplot g;

            g.cmd(command);
        }

        template<typename T>
        typename Visualization<T>::AxisRanges Visualization<T>::configureRange(const double scale, const int x,
                                                                               const int y, const int z) {
            AxisRanges axis_ranges;
            if(x > -1) {
                auto _x = this->samples->getFeature(x);
                double x_min = mltk::min(_x), x_max = scale*mltk::max(_x);
                x_min += (x_min > 0)?(1.0-scale)*x_min:-(1.0-scale)*x_min;
                axis_ranges[0].min = (x_min <= 0)?std::floor(x_min):std::ceil(x_min);
                axis_ranges[0].max = (x_max <= 0)?std::floor(x_max):std::ceil(x_max);
                configs["xrange"] = "[" + std::to_string(axis_ranges[0].min) + ":" + std::to_string(axis_ranges[0].max) + "]";
            }
            if(y > -1) {
                auto _y = this->samples->getFeature(y);
                double y_min = mltk::min(_y), y_max = scale*mltk::max(_y);
                y_min += (y_min > 0)?(1.0-scale)*y_min:-(1.0-scale)*y_min;
                axis_ranges[1].min = (y_min <= 0)?std::floor(y_min):std::ceil(y_min);
                axis_ranges[1].max = (y_max <= 0)?std::floor(y_max):std::ceil(y_max);
                configs["yrange"] = "[" + std::to_string(axis_ranges[1].min) + ":" + std::to_string(axis_ranges[1].max) + "]";
            }
            if(z > -1) {
                auto _z = this->samples->getFeature(z);
                double z_min = mltk::min(_z), z_max = scale*mltk::max(_z);
                z_min += (z_min > 0)?(1.0-scale)*z_min:-(1.0-scale)*z_min;
                axis_ranges[2].min = (z_min <= 0)?std::floor(z_min):std::ceil(z_min);
                axis_ranges[2].max = (z_max <= 0)?std::floor(z_max):std::ceil(z_max);
                configs["zrange"] = "[" + std::to_string(axis_ranges[2].min) + ":" + std::to_string(axis_ranges[2].max) + "]";
            }
            return axis_ranges;
        }

        template<typename T>
        std::string Visualization<T>::prepareScript(string cmd) {
            std::replace(cmd.begin(), cmd.end(), ';', '\n');
            return cmd;
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