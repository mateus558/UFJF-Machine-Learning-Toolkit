/*! Visualization wrapper class
   \file Visualization.hpp
   \author Mateus Coutinho Marim
*/

#ifndef VISUALIZATION__HPP
#define VISUALIZATION__HPP
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

    namespace mltk{
        /**
         * \brief Namespace for data visualization methods.
         */
        namespace visualize{
        /**
         * \brief Class for data visualization.
         */
        template < typename T >
        class Visualization {
            // Attributes
        private :
            /// Sample to be visualized.
            Data< T > *samples;
            std::map<std::string, std::string> configs;
            Gnuplot *g{nullptr};
            bool is_shared{true};

            /**
             * \brief Create temporary files to plot the negative and positive samples.
             * \return void
             */
            std::vector<std::string> createTempFiles();
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
            std::vector<std::string> getTempFilesNames();
            /**
             * \brief removeTempFiles Remove the temporary files created in the temp folder.
             */
            void removeTempFiles();
            void configurePlot(const std::string& outname, const std::string& format, const std::string& title, bool save=false,
                               const std::string& x_label="", const std::string& y_label="", const std::string& z_label="");
            std::string fetchConfigs();
            // Operations
        public :
            Visualization (bool shared_session=true);
            explicit Visualization (Data<T> &sample, bool shared_session=true);

            /*********************************************
             *               Setters                     *
             *********************************************/
            void setTerminal(const std::string& terminal){ configs["terminal"] = terminal;}
            /**
             * \brief Set sample to be visualized.
             * \param sample (???) Data< T > to set for visualization.
             * \return void
             */
            void setSample (Data< T > *sample);
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
            void plot2D(int x, int y, bool save=false, const std::string& title="", const std::string& format="svg",
                        const std::string& x_label="x", const std::string& y_label="y");
            /**
             * \brief Plot the selected features in 3D.
             * \param x (???) Feature to be used in the x-axis.
             * \param y (???) Feature to be used in the y-axis.
             * \param z (???) Feature to be used in the z-axis.
             * \return void
             */
            void plot3D(int x, int y, int z, bool save=false, const std::string& title="", const std::string& format="svg",
                        const std::string& x_label="x", const std::string& y_label="y", const std::string& z_label="z");
            /**
             * \brief Plot the data in 2D with separated by the hyperplane in the solution.
             * \param x (???) Feature to be used in the x-axis.
             * \param y (???) Feature to be used in the y-axis.
             * \return void
             */
            void plot2DwithHyperplane(int x, int y, Solution w, bool save=false, const std::string& title="",
                                      const std::string& format="svg",
                                      const std::string& x_label="x", const std::string& y_label="y");
            /**
             * \brief Plot the data in 2D with separated by the hyperplane in the solution.
             * \param x (???) Feature to be used in the x-axis.
             * \param y (???) Feature to be used in the y-axis.
             * \param z (???) Feature to be used in the z-axis.
             * \return void
             */
            void plot3DwithHyperplane(int x, int y, int z, Solution w, bool save=false, const std::string& title="",
                                      const std::string& format="svg",
                                      const std::string& x_label="x", const std::string& y_label="y",
                                      const std::string& z_label="z");
            ~Visualization();
        };
}}

#endif
