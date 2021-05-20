/*! Visualization wrapper class
   \file Visualization.hpp
   \author Mateus Coutinho Marim
*/

#ifndef VISUALIZATION_HPP
#define VISUALIZATION_HPP
#ifdef __unix__
#include <dirent.h>
#include "gnuplot_i.hpp"
#elif _WIN32
#include <windows.h>
#endif
#include "ufjfmltk/core/Data.hpp"
#include "ufjfmltk/core/Solution.hpp"
#include <string>
#include <map>
#include <filesystem>

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
        bool is_shared{true}, keep_temp_files{false};
        static size_t n_plots;

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
        void configureRange(double scale = 1.0, int x = -1, int y = -1, int z = -1);
        std::string prepareScript(std::string cmd);
        std::vector<std::string> sortLabels(std::vector<std::string>& files, const std::string& type="scatter");
        std::string fetchConfigs();
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

        /**
         * \brief Plot the selected features in 2D.
         * \param x (???) Feature to be used in the x-axis.
         * \param y (???) Feature to be used in the y-axis.
         * \return void
         */
        std::string plot2D(int x, int y, bool save=false, double scale = 1.0,
                    const std::string& title="",
                    const std::string& format="svg",
                    const std::string& x_label="x", const std::string& y_label="y");
        /**
         * \brief Plot the selected features in 3D.
         * \param x (???) Feature to be used in the x-axis.
         * \param y (???) Feature to be used in the y-axis.
         * \param z (???) Feature to be used in the z-axis.
         * \return void
         */
        std::string plot3D(int x, int y, int z, bool save=false, double scale = 1.0,
                    const std::string& title="",
                    const std::string& format="svg",
                    const std::string& x_label="x", const std::string& y_label="y", const std::string& z_label="z");
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
        std::string plotDecisionSurface2D(Learner& learner, int x, int y, bool is_trained = true, size_t grid_dim = 50,
                                          bool save=false,
                                   const double scale = 1.0,
                                   const std::string& title="",
                                   const std::string& format="svg",
                                   const std::string& x_label="x", const std::string& y_label="y"){
            mltk::Point<int> classes(this->samples->classes());
            auto _x = this->samples->getFeature(x);
            auto _y = this->samples->getFeature(y);
            double x_min = mltk::min(_x), y_min = mltk::min(_y);
            double x_max = scale*mltk::max(_x), y_max = scale*mltk::max(_y);
            y_min += (y_min > 0)?(1.0-scale)*y_min:-(1.0-scale)*y_min;
            x_min += (x_min > 0)?(1.0-scale)*x_min:-(1.0-scale)*x_min;
            mltk::Point xx = mltk::linspace(x_min, x_max, grid_dim);
            mltk::Point yy = mltk::linspace(y_min, y_max, grid_dim);
            mltk::Data grid(grid_dim, grid_dim,0);
            auto data_copy = this->samples->selectFeatures({size_t(x), size_t(y)});

            learner.setSamples(data_copy);
            learner.setVerbose(0);
            if(!is_trained) learner.train();
            configurePlot("contour_"+this->samples->name()+"_"+mltk::utils::timestamp(), format, title, save,
                          x_label, y_label);

            std::string data_fname = data_copy.name()+mltk::utils::timestamp()+".dat";
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
                std::string fname = class_names[i]+"_data"+mltk::utils::timestamp()+".dat";
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
            cmd += "set xrange ["+ std::to_string(x_min) + ":"+ std::to_string(x_max) + "];";
            cmd += "set yrange ["+ std::to_string(y_min) + ":"+ std::to_string(y_max) + "];";
            cmd += "set cbrange [" + std::to_string(mltk::min(classes)) +":" +  std::to_string(mltk::max(classes)) + "];";
            cmd += "set cbtics 1;";
            cmd += "set palette rgbformulae 22,13,10;";
            cmd += "splot '"+data_fname+"', " + scatter_cmd;
            temp_files_names.push_back(data_fname);
            if(is_shared) g->cmd(cmd);
            else {
                Gnuplot g_;
                g_.cmd(cmd);
            }
            if(!keep_temp_files) {
                for (auto &temp_file: temp_files_names) {
                    std::filesystem::remove_all(temp_file);
                }
            }
            return prepareScript(cmd);
        }

        static void cmd(const std::string& command);
        ~Visualization();
    };
}}

#endif
