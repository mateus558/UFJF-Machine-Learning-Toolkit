/*! Visualization wrapper class
   \file Visualization.hpp
   \author Mateus Coutinho Marim
*/

#ifndef VISUALIZATION_HPP
#define VISUALIZATION_HPP
#ifdef __unix__
#include <dirent.h>
#elif _WIN32
#include <windows.h>
#endif
#if __has_include(<filesystem>)
#include <filesystem>
  namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
  namespace fs = std::experimental::filesystem;
#else
error "Missing the <filesystem> header."
#endif
#define PLOT_FOLDER "plottemp"
#include <string>
#include <map>
#include <array>
#include <utility>
#include "ufjfmltk/visual/gnuplot_i.hpp"
#include "ufjfmltk/core/Data.hpp"
#include "ufjfmltk/core/Solution.hpp"
#include "ufjfmltk/core/Utils.hpp"

namespace mltk{
    /**
     * \brief Namespace for data visualization methods.
     */
    namespace visualize{
    /**
     * \brief Class for data visualization.
     */
    template < typename T = double >
    class Visualization {
        // Attributes
    private :
        /// Sample to be visualized.
        Data< T > *samples;
        std::map<std::string, std::string> configs;
        Gnuplot *g{nullptr};
        std::string plot_folder;
        std::vector<std::string> plot_folders;
        std::vector<std::string> temp_fnames;
        bool is_shared, keep_temp_files;
        static size_t n_plots;

        struct AxisRange{
            double min=0, max=0;
        };

        using AxisRanges = std::array<AxisRange, 3>;

        /**
         * \brief Create temporary files to plot the negative and positive samples.
         * \return void
         */
        std::vector<std::string> createTempFiles(std::string plot_folder="");
        /**
         * \brief valid_file Returns if a given file name is valid.
         * \param file  Name of the file.
         * \return bool
         */
        bool valid_file(std::string file);
        /**
         * \brief getTempFilesNames Returns the name of the files in the temp folder.
         * \return vector<string>
         */
        std::vector<std::string> getTempFilesNames(bool append_path=false);
        /**
         * \brief removeTempFiles Remove the temporary files created in the temp folder.
         */
        void removeTempFiles();
        void configurePlot(const std::string& outname, const std::string& format, const std::string& title, bool save=false,
                           const std::string& x_label="", const std::string& y_label="", const std::string& z_label="");
        AxisRanges configureRange(double scale = 1.0, int x = -1, int y = -1, int z = -1);
        std::string prepareScript(std::string cmd);
        std::vector<std::string> sortLabels(std::vector<std::string>& files, const std::string& type="scatter");
        std::string fetchConfigs();
        void create_plotfolder();
        // Operations
    public :
        explicit Visualization (bool shared_session=true, bool keep_temp_files=false);
        explicit Visualization (Data<T> &sample, bool shared_session=true, bool keep_temp_files=false);

        /*********************************************
         *               Setters                     *
         *********************************************/
        void setTerminal(const std::string& terminal){ configs["terminal"] = terminal;}
        /**
         * \brief Set sample to be visualized.
         * \param sample (???) Data< T > to set for visualization.
         * \return void
         */
        void setSample (Data< T >& sample);
        /**
         * \brief Set plot title.
         * \param title (???) Plot title.
         * \return void
         */
        void setTitle (std::string title);
        /**
         * \brief Set plot style. (points, lines, etc.)
         * \param style (???) Style to be set.
         * \return void
         */
        void setStyle (std::string style);

        std::string execute_command(const std::string& cmd, bool fetch_configs=true);

        std::string plot1DRegresion(int x=0, bool save=false, double scale = 1.0,
                                    const std::string& title="",
                                    const std::string& format="svg",
                                    const std::string& x_label="x", const std::string& y_label="y");

        std::string plot1DRegresionHyperplane(int x, Solution s, bool save=false, double scale = 1.0,
                                              const std::string& title="",
                                              const std::string& format="svg",
                                              const std::string& x_label="x", const std::string& y_label="y");

        /**
         * \brief Plot the selected features in 2D.
         * \param x (???) Feature to be used in the x-axis.
         * \param y (???) Feature to be used in the y-axis.
         * \return void
         */
        std::string plot2D(int x = 0, int y=1, bool save=false, double scale = 1.0,
                    const std::string& title="",
                    const std::string& format="svg",
                    const std::string& x_label="x", const std::string& y_label="y");
        /**
         * \brief Plot the data in 2D with separated by the hyperplane in the solution.
         * \param x (???) Feature to be used in the x-axis.
         * \param y (???) Feature to be used in the y-axis.
         * \return void
         */
        std::string plot2DwithHyperplane(int x, int y, Solution w, bool save=false, double scale = 1.0,
                                         const std::string& title="",
                                         const std::string& format="svg",
                                         const std::string& x_label="x", const std::string& y_label="y");

        std::string plot2DRegresion(int x=0, int y=1, bool save=false, double scale = 1.0,
                                    const std::string& title="",
                                    const std::string& format="svg",
                                    const std::string& x_label="x", const std::string& y_label="y",
                                    const std::string& z_label="z");

        std::string plot2DRegresionHyperplane(int x, int y, Solution s, bool save=false, double scale = 1.0,
                                              const std::string& title="",
                                              const std::string& format="svg",
                                              const std::string& x_label="x", const std::string& y_label="y",
                                              const std::string& z_label="z");

        /**
         * \brief Plot the selected features in 3D.
         * \param x (???) Feature to be used in the x-axis.
         * \param y (???) Feature to be used in the y-axis.
         * \param z (???) Feature to be used in the z-axis.
         * \return void
         */
        std::string plot3D(int x=0, int y=1, int z=2, bool save=false, double scale = 1.0,
                    const std::string& title="",
                    const std::string& format="svg",
                    const std::string& x_label="x", const std::string& y_label="y", const std::string& z_label="z");
        /**
         * \brief Plot the data in 2D with separated by the hyperplane in the solution.
         * \param x (???) Feature to be used in the x-axis.
         * \param y (???) Feature to be used in the y-axis.
         * \param z (???) Feature to be used in the z-axis.
         * \return void
         */
        std::string plot3DwithHyperplane(int x, int y, int z, Solution w, bool save=false, double scale = 1.0,
                                  const std::string& title="",
                                  const std::string& format="svg",
                                  const std::string& x_label="x", const std::string& y_label="y",
                                  const std::string& z_label="z");

        template<class Learner>
        std::string plotDecisionSurface2D(Learner& learner, int x=0, int y=1, bool is_trained = true, size_t grid_dim = 50,
                                          bool save=false,
                                   const double scale = 1.0,
                                   const std::string& title="",
                                   const std::string& format="svg",
                                   const std::string& x_label="x", const std::string& y_label="y"){
            assert(samples->isClassification());
            configurePlot("contour_"+this->samples->name()+"_"+mltk::utils::timestamp(), format, title, save,
                          x_label, y_label);
            AxisRanges axis_ranges = configureRange(scale, x, y);
            mltk::Point<int> classes(this->samples->classes());
            mltk::Point xx = mltk::linspace(axis_ranges[0].min, axis_ranges[0].max, grid_dim);
            mltk::Point yy = mltk::linspace(axis_ranges[1].min, axis_ranges[1].max, grid_dim);
            mltk::Data grid(grid_dim, grid_dim,0);
            auto data_copy = this->samples->selectFeatures({size_t(x+1), size_t(y+1)});

            learner.setSamples(data_copy);
            if(!is_trained) learner.train();

            std::string data_fname = this->plot_folder+data_copy.name()+"_"+mltk::utils::timestamp()+".dat";
            std::ofstream data_file(data_fname);
            if(!data_file.is_open()){
                std::cerr << "error opening file" << std::endl;
            }
            for(int i = 0; i < grid.size(); i++){
                int j;
                for(j = 0; j < grid.dim(); j++){
                    grid(i)[j] = learner.evaluate(mltk::Point({xx[i], yy[j]}));
                    data_file << xx[i] << " " << yy[j] << " " << std::to_string(grid(i)[j]) << "\n";
                }
                data_file << "\n";
            }
            data_file.close();

            std::string dims = utils::itos(1) + ":" + utils::itos(2)+":(0)";
            std::string scatter_cmd;
            std::vector<std::string> temp_files_names, class_names = this->samples->classesNames();
            std::vector<std::ofstream> classes_files(classes.size());

            for(int i = 0; i < classes_files.size(); i++){
                std::string fname = this->plot_folder+class_names[i]+"_data"+mltk::utils::timestamp()+".dat";
                classes_files[i].open(fname);
                if(!classes_files[i].is_open()){
                    std::cerr << "Error opening file!" << std::endl;
                }
                temp_files_names.push_back(fname);
            }

            for(int i = 0; i < data_copy.size(); i++){
                auto class_pos = std::find(classes.begin(), classes.end(), data_copy(i).Y()) - classes.begin();
                classes_files[class_pos] << data_copy(i)[0] << " " << data_copy(i)[1] << " " << data_copy(i).Y() << "\n";
            }
            for(auto & classes_file : classes_files){
                classes_file.close();
            }
            auto names = sortLabels(temp_files_names, "decision");
            for(int i = 0; i < temp_files_names.size(); i++){
                if(i < classes_files.size()-1) {
                    scatter_cmd += "'" + temp_files_names[i] + "' title '"+ names[i] +"' with points,";
                }else{
                    scatter_cmd += "'" + temp_files_names[i] + "' title '"+ names[i] +"' with points;";
                }
            }

            std::string cmd;
            cmd += fetchConfigs();
            cmd += "set pm3d map;";
            cmd += "set pm3d interpolate 0,0;";
            cmd += "set cbrange [" + std::to_string(mltk::min(classes)) +":" +  std::to_string(mltk::max(classes)) + "];";
            cmd += "set cbtics 1;";
            cmd += "set palette rgbformulae 22,13,10;";
            cmd += "splot '"+data_fname+"', " + scatter_cmd;
            temp_files_names.push_back(data_fname);
            cmd = execute_command(cmd, false);
            temp_fnames.insert(temp_fnames.begin(), temp_files_names.begin(), temp_files_names.end());
            return prepareScript(cmd);
        }

        static void cmd(const std::string& command);
        ~Visualization();
    };

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
            create_plotfolder();
            createTempFiles();
        }

        template<typename T>
        void Visualization<T>::create_plotfolder() {
            do{
                this->plot_folder = PLOT_FOLDER+std::to_string(n_plots)+"/";
                if(std::find(plot_folders.begin(), plot_folders.end(), this->plot_folder) == plot_folders.end()) {
                    plot_folders.push_back(this->plot_folder);
                }
                n_plots++;
            }while(fs::exists(this->plot_folder));
            fs::create_directory(this->plot_folder);
        }

        template<typename T>
        void Visualization< T >::setTitle(std::string title){
            configs["title"] = std::move(title);
        }

        template<typename T>
        void Visualization< T >::setStyle(std::string style){
            configs["style"] = std::move(style);
        }

        template<typename T>
        std::vector<std::string> Visualization< T >::createTempFiles(std::string plot_folder){
            size_t i, j, k, size = samples->size(), dim = samples->dim();
            std::vector<std::string> file_names;

            if(plot_folder.empty()) {
                plot_folder = this->plot_folder;
            }

            if(samples->isClassification()) {
                std::vector<std::string> class_names = samples->classesNames();
                std::vector<int> classes = samples->classes();
                std::vector<std::ofstream> temp_files(class_names.size());

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
                std::ofstream samples_file(std::string(plot_folder) + "samples.plt");

                for (i = 0; i < size; i++) {
                    for (j = 0; j < dim; j++) {
                        samples_file << (double) (samples->point(i)->X()[j]) << " ";
                    }
                    samples_file << (double) (samples->point(i)->Y()) << std::endl;
                }

                samples_file.close();
                std::string fname = (this->samples->name().empty())?"samples.plt":this->samples->name()+".plt";
                file_names.emplace_back(std::string(plot_folder) + fname);
            }
            return file_names;
        }

        template<typename T>
        bool Visualization< T >::valid_file(std::string file){
            size_t i;
            bool flag = false;
            std::string ext;

            if(file.empty() || file.size() < 4)
                return false;

            for(i = file.size()-1; i >= 0; i--){
                if(file[i] == '.') break;
                std::string f(1, file[i]);
                ext = f + ext;
            }

            for(std::string type : types){
                if(type == ext) flag = true;
            }

            return flag;
        }

        template<typename T>
        std::vector<std::string> Visualization< T >::getTempFilesNames(bool append_path){
            std::vector<std::string> files;

#ifdef __unix__
            DIR *dpdf;
            struct dirent *epdf;
            auto path = std::string(plot_folder);

            dpdf = opendir(path.c_str());
            if(dpdf != nullptr){
                while((epdf = readdir(dpdf))){
                    std::string file = std::string(epdf->d_name);
                    if(valid_file(file) && !file.empty()){
                        files.push_back((append_path)?(path+file):file);
                    }
                }
            }else{
                std::cout << "Folder not found!" << std::endl;
            }

            closedir(dpdf);
#elif _WIN32
            HANDLE hFind;
                WIN32_FIND_DATA data;

                hFind = FindFirstFile(".\\temp\\*.*", &data);
                if (hFind != INVALID_HANDLE_VALUE) {
                do {
                    std::string file_name(data.cFileName);
                    if(valid_file(file_name) && !file_name.empty()){
                        files.push_back(file_name);
                    }
                } while (FindNextFile(hFind, &data));
                FindClose(hFind);
                }
#else
            std::cerr << "Can't remove temporary files, please remove manually. (Unsupported System)." << std::endl;
#endif

            return files;
        }

        template<typename T>
        void Visualization< T >::removeTempFiles(){
            if(keep_temp_files) {
                auto fname = this->plot_folder;
                fname.erase(std::remove(fname.begin(), fname.end(), '/'), fname.end());
                fs::rename(fname, fname+"_saved_"+mltk::utils::timestamp());
                return;
            }
            std::string path;
            std::vector<std::string> temps;

            temps = getTempFilesNames();

            for(const std::string& file : temps){
                std::string _path = plot_folder + file;
                if(fs::exists(_path)) fs::remove_all(_path);
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
        std::string
        Visualization<T>::plot1DRegresion(int x, bool save, double scale, const std::string &title, const std::string &format,
                                          const std::string &x_label, const std::string &y_label) {
            std::string dims = utils::itos(x+1) + ":" + utils::itos(2);
            std::string cmd("plot ");
            std::vector<std::string> temp_files_names;
            size_t i;
            std::string out_name = (this->samples->name().empty())?"2dplot_"+utils::itos(x)+"_regression":
                                   this->samples->name()+"_2d_"+utils::itos(x)+"_regression";

            configureRange(scale, x);
            configurePlot(out_name, format, title, save, x_label, y_label);

            temp_files_names = getTempFilesNames(true);
            for(i = 0; i < temp_files_names.size() - 1; i++){
                cmd += "\'" + temp_files_names[i] + "\' using " + dims + " with points, ";
            }
            cmd += "\'" + temp_files_names[i] + "\' using " + dims + " with points";

            cmd = execute_command(cmd);
            return prepareScript(cmd);
        }

        template<typename T>
        std::string
        Visualization<T>::plot1DRegresionHyperplane(int x, Solution s, bool save, double scale, const std::string &title,
                                                    const std::string &format, const std::string &x_label,
                                                    const std::string &y_label) {
            std::string dims = utils::itos(x+1) + ":" + utils::itos(this->samples->dim()+1);
            std::string fx, cmd;
            std::vector<std::string> temp_files_names;
            size_t i;
            std::string out_name = (this->samples->name().empty())?"2dplot_"+utils::itos(x)+"_regression":
                                   this->samples->name()+"_2d_"+utils::itos(x)+"_regression";

            configureRange(scale, x);
            configurePlot(out_name, format, title, save, x_label, y_label);

            fx = "f(x) = " + utils::dtoa(s.w[x]) + "*x + " + utils::dtoa(s.bias);
            cmd = fx + ";" + "plot ";

            temp_files_names = getTempFilesNames(true);
            for(i = 0; i < temp_files_names.size() - 1; i++){
                cmd += "\'" + temp_files_names[i] + "\' using " + dims + " with points, ";
            }
            cmd += "\'" + temp_files_names[i] + "\' using " + dims + " notitle with points, f(x) notitle with lines ls 1 lt rgb \"red\"";
            cmd = fetchConfigs() + cmd;

            cmd = execute_command(cmd);
            return prepareScript(cmd);
        }

        template<typename T>
        std::string Visualization< T >::plot2D(int x, int y, bool save, const double scale,
                                               const std::string& title,
                                               const std::string& format,
                                               const std::string& x_label, const std::string& y_label){
            assert(samples->isClassification());
            std::string dims = utils::itos(x+1) + ":" + utils::itos(y+1);
            std::string cmd("plot ");
            std::vector<std::string> temp_files_names, class_names = samples->classesNames();
            Point<int> classes = samples->classes();
            size_t i;
            std::string out_name = (this->samples->name().empty())?"2dplot_"+utils::itos(x)+"_"+utils::itos(y):
                                   this->samples->name()+"_2d_"+utils::itos(x)+"_"+utils::itos(y);

            configureRange(scale, x, y);
            configurePlot(out_name, format, title, save, x_label, y_label);

            temp_files_names = getTempFilesNames(true);
            auto names = sortLabels(temp_files_names);
            for(i = 0; i < temp_files_names.size() - 1; i++){
                cmd += "\'" + temp_files_names[i] + "\' using " + dims + " title \'" + names[i] + "\' with points, ";
            }
            cmd += "\'" + temp_files_names[i] + "\' using " + dims + " title \'" + names[i] + "\' with points";

            cmd = execute_command(cmd);
            return prepareScript(cmd);
        }

        template<typename T>
        std::string Visualization< T >::plot2DwithHyperplane(int x, int y, Solution s, bool save, const double scale,
                                                             const std::string& title,
                                                             const std::string& format,
                                                             const std::string& x_label, const std::string& y_label){
            assert(samples->isClassification());
            if(s.norm != s.norm) s.norm = 0.0;
            std::string feats = utils::itos(x+1) + ":" + utils::itos(y+1);
            std::string fx, gx, hx, cmd;
            std::vector<std::string> temp_files_names, class_names = samples->classesNames();
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

            temp_files_names = getTempFilesNames(true);
            auto names = sortLabels(temp_files_names);
            for(i = 0; i < class_names.size() - 1; i++){
                cmd += "\'" + temp_files_names[i] + "\' using " + feats + " title \'" + names[i] + "\' with points, ";
            }
            cmd += "\'" + temp_files_names[i] + "\' using " + feats + " title \'" + names[i] + "\' with points, f(x) notitle with lines ls 1, g(x) notitle with lines ls 2, h(x) notitle with lines ls 2";
            cmd = execute_command(cmd);
            return prepareScript(cmd);
        }


        template<typename T>
        std::string Visualization<T>::plot2DRegresion(int x, int y, bool save, double scale, const std::string &title,
                                                      const std::string &format, const std::string &x_label,
                                                      const std::string &y_label, const std::string &z_label) {
            std::string dims = utils::itos(x+1) + ":" + utils::itos(y+1) + ":"  + utils::itos(this->samples->dim()+1);
            std::string fx, cmd("splot ");
            std::vector<std::string> temp_files_names;
            size_t i;
            std::string out_name = (this->samples->name().empty())?"2dplot_"+utils::itos(x)+"_regression":
                                   this->samples->name()+"_2d_"+utils::itos(x)+"_regression";

            configureRange(scale, x);
            configurePlot(out_name, format, title, save, x_label, y_label, z_label);

            temp_files_names = getTempFilesNames(true);
            for(i = 0; i < temp_files_names.size() - 1; i++){
                cmd += "\'" + temp_files_names[i] + "\' using " + dims + " with points, ";
            }
            cmd += "\'" + temp_files_names[i] + "\' using " + dims + " notitle with points";
            cmd = execute_command(cmd);
            return prepareScript(cmd);
        }

        template<typename T>
        std::string Visualization<T>::plot2DRegresionHyperplane(int x, int y, Solution s, bool save, double scale,
                                                                const std::string &title, const std::string &format,
                                                                const std::string &x_label, const std::string &y_label,
                                                                const std::string &z_label) {
            std::string dims = utils::itos(x+1) + ":" + utils::itos(y+1) + ":"  + utils::itos(this->samples->dim()+1);
            std::string fxy, cmd;
            std::vector<std::string> temp_files_names;
            size_t i;
            std::string out_name = (this->samples->name().empty())?"2dplot_"+utils::itos(x)+"_regression":
                                   this->samples->name()+"_2d_"+utils::itos(x)+"_regression";

            configureRange(scale, x);
            configurePlot(out_name, format, title, save, x_label, y_label, z_label);

            fxy = "f(x,y) = "+utils::dtoa(s.w[x])+"*x + "+utils::dtoa(s.w[y])+"*y + "+utils::dtoa(s.bias);
            cmd = fxy + "; splot ";

            temp_files_names = getTempFilesNames(true);
            for(i = 0; i < temp_files_names.size() - 1; i++){
                cmd += "\'" + temp_files_names[i] + "\' using " + dims + " with points, ";
            }
            cmd += "\'" + temp_files_names[i] + "\' using " + dims + " notitle with points, f(x,y) notitle with lines ls 1 lt rgb \"red\"";
            cmd = execute_command(cmd);
            return prepareScript(cmd);
        }

        template<typename T>
        std::string Visualization< T >::plot3D(int x, int y, int z, bool save, const double scale,
                                               const std::string& title,
                                               const std::string& format,
                                               const std::string& x_label, const std::string& y_label, const std::string& z_label){
            assert(samples->isClassification());
            std::string dims = utils::itos(x+1) + ":" + utils::itos(y+1) + ":" + utils::itos(z+1);
            std::string cmd("splot ");
            std::vector<std::string> temp_files_names, class_names = samples->classesNames();
            size_t i;
            std::string out_name = (this->samples->name().empty())?"3dplot_"+utils::itos(x)+"_"+utils::itos(y)+"_"+utils::itos(z):
                                   this->samples->name()+"_3d_"+utils::itos(x)+"_"+utils::itos(y)+"_"+utils::itos(z);

            configureRange(scale, x, y, z);
            configurePlot(out_name, format, title, save, x_label, y_label, z_label);

            temp_files_names = getTempFilesNames(true);
            auto names = sortLabels(temp_files_names);
            for(i = 0; i < class_names.size() - 1; i++){
                cmd += "\'" + temp_files_names[i] + "\' using " + dims + " title \'" + names[i] + "\' with points, ";
            }
            cmd += "\'" + temp_files_names[i] + "\' using " + dims + " title \'" + names[i] + "\' with points";
            cmd = execute_command(cmd);
            return prepareScript(cmd);
        }

        template<typename T>
        std::string Visualization< T >::plot3DwithHyperplane(int x, int y, int z, Solution s, bool save, const double scale,
                                                             const std::string& title,
                                                             const std::string& format,
                                                             const std::string& x_label, const std::string& y_label,
                                                             const std::string& z_label){
            assert(samples->isClassification());
            std::string feats = utils::itos(x+1) + ":" + utils::itos(y+1) + ":" + utils::itos(z+1);
            std::string fxy, gxy, hxy, cmd;
            std::vector<std::string> temp_files_names, class_names = samples->classesNames();
            size_t i;
            std::string out_name = (this->samples->name().empty())?"3dplotsol_"+utils::itos(x)+"_"+utils::itos(y)+"_"+utils::itos(z):
                                   this->samples->name()+"_3dsol_"+utils::itos(x)+"_"+utils::itos(y)+"_"+utils::itos(z);

            configureRange(scale, x, y, z);
            configurePlot(out_name, format, title, save, x_label, y_label, z_label);

            gxy = "g(x,y) = "+utils::dtoa(-s.w[x]/s.w[z])+"*x + "+utils::dtoa(-s.w[y]/s.w[z])+"*y + "+utils::dtoa((s.bias + s.margin * s.norm)/-s.w[z]);
            fxy = "f(x,y) = "+utils::dtoa(-s.w[x]/s.w[z])+"*x + "+utils::dtoa(-s.w[y]/s.w[z])+"*y + "+utils::dtoa(s.bias/-s.w[z]);
            hxy = "h(x,y) = "+utils::dtoa(-s.w[x]/s.w[z])+"*x + "+utils::dtoa(-s.w[y]/s.w[z])+"*y + "+utils::dtoa((s.bias - s.margin * s.norm)/-s.w[z]);
            cmd = gxy + ";" + fxy + ";" + hxy + "; splot ";

            temp_files_names = getTempFilesNames(true);
            auto names = sortLabels(temp_files_names);
            for(i = 0; i < class_names.size() - 1; i++){
                cmd += "\'" + temp_files_names[i] + "\' using " + feats + " title \'" + names[i] + "\' with points, ";
            }
            cmd += "\'" + temp_files_names[i] + "\' using " + feats + " title \'" + names[i] + "\' with points, ";
            cmd += "g(x,y) notitle with lines ls 1 lt rgb \"red\",";
            cmd += "f(x,y) notitle with lines ls 1,";
            cmd += "h(x,y) notitle with lines ls 1 lt rgb \"red\";";
            execute_command(cmd);
            return prepareScript(cmd);
        }

        template<typename T>
        void Visualization< T >::setSample(Data<T>& sample) {
            this->samples = &sample;
            if(fs::exists(plot_folder)) {
                removeTempFiles();
            }
            create_plotfolder();
            createTempFiles();
        }

        template<typename T>
        std::vector<std::string> Visualization<T>::sortLabels(std::vector<std::string> &files, const std::string& type) {
            Point<int> classes = samples->classes();
            auto class_names = samples->classesNames();
            std::vector<std::string> names(files.size());
            std::transform(files.begin(), files.end(), names.begin(), [type](std::string& path){
                if(type == "decision") {
                    auto res = mltk::utils::tokenize(mltk::utils::tokenize(path, '_')[0], '/');
                    return mltk::utils::tokenize(mltk::utils::tokenize(path, '_')[0], '/')[1];
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
                                                mltk::utils::tokenize(mltk::utils::tokenize(a, '_')[0], '/')[1]) - class_names.begin();
                    auto bclass_pos = std::find(class_names.begin(), class_names.end(),
                                                mltk::utils::tokenize(mltk::utils::tokenize(b, '_')[0], '/')[1]) - class_names.begin();
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
        Visualization<T>::configurePlot(const std::string &outname, const std::string &format, const std::string &title, bool save,
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
            for(auto& pfolder: plot_folders) {
                if(fs::exists(pfolder)) {
                    fs::remove_all(pfolder);
                }
            }
            plot_folders.clear();
        }

        template<typename T>
        void Visualization<T>::cmd(const std::string &command) {
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
        std::string Visualization<T>::prepareScript(std::string cmd) {
            std::replace(cmd.begin(), cmd.end(), ';', '\n');
            return cmd;
        }

        template<typename T>
        std::string Visualization<T>::execute_command(const std::string& _cmd, bool fetch_configs) {
            auto cmd = (fetch_configs)?fetchConfigs() + _cmd:_cmd;
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
                return cmd;
        }
    }}
#endif
