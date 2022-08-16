#include <ufjfmltk/ufjfmltk.hpp>

using namespace mltk::classifier;

int main(){
    mltk::Data data = mltk::datasets::make_blobs(100, 3, 3, 1, -10, 10, true, 42).dataset;

    std::cout << "size: " << data.size() << std::endl;
    std::cout << "dim: " << data.dim() << std::endl;
    std::cout << "classes distribution: " << mltk::Point<size_t>(data.classesDistribution()) << std::endl;

    mltk::visualize::Visualization<double> plotter;

    plotter.plot3D(data.getFeature(0), data.getFeature(1), data.getFeature(2));

    return 1;
}