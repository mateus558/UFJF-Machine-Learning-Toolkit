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
#include "Data.hpp"
#include "Solution.hpp"

#include <string>


/**
 * \brief Class for visualize data using gnuplot.
 */
template < typename T >
class Visualization {
    // Attributes
private :
    /// Sample to be visualized.
    Data< T > *samples;
    /// Interface to gnuplot.
#ifdef __unix__
    Gnuplot g;
#elif _WIN32

#endif
    /**
     * \brief Create temporary files to plot the negative and positive samples.
     * \return void
     */
    void createPosNegTemps();
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
    // Operations
public :
    Visualization ();
    Visualization (Data< T > *sample);

    /*********************************************
     *               Setters                     *
     *********************************************/

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
    void plot2D(int x, int y);
    /**
     * \brief Plot the selected features in 3D.
     * \param x (???) Feature to be used in the x-axis.
     * \param y (???) Feature to be used in the y-axis.
     * \param z (???) Feature to be used in the z-axis.
     * \return void
     */
    void plot3D(int x, int y, int z);
    /**
     * \brief Plot the data in 2D with separated by the hyperplane in the solution.
     * \param x (???) Feature to be used in the x-axis.
     * \param y (???) Feature to be used in the y-axis.
     * \return void
     */
    void plot2DwithHyperplane(int x, int y, Solution w);
    /**
     * \brief Plot the data in 2D with separated by the hyperplane in the solution.
     * \param x (???) Feature to be used in the x-axis.
     * \param y (???) Feature to be used in the y-axis.
     * \param z (???) Feature to be used in the z-axis.
     * \return void
     */
    void plot3DwithHyperplane(int x, int y, int z, Solution w);
    ~Visualization();
};

#endif
