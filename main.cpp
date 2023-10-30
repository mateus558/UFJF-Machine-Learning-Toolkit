#include <ufjfmltk/ufjfmltk.hpp>

using namespace mltk::classifier;

int main(){
    mltk::Data data = mltk::datasets::make_blobs(7, 3, 3, 1, -10, 10, true, 42).dataset;

    std::cout << "size: " << data.size() << std::endl;
    std::cout << "dim: " << data.dim() << std::endl;
    std::cout << "classes distribution: " << mltk::Point<size_t>(data.classesDistribution()) << std::endl;

//    mltk::visualize::Visualization<double> plotter;

//    plotter.plot3D(data.getFeature(0), data.getFeature(1), data.getFeature(2));

    auto validData = mltk::validation::partTrainTest(data, 3, true);

    std::cout << "train size: " << validData.train.size() << std::endl;
    std::cout << "test size: " << validData.test.size() << std::endl;

    // iterate over the train data

    for(auto &p : validData.train){
        std::cout << *p << std::endl;
    }

    std::cout << std::endl;

    // iterate over the test data

    for(auto &p : validData.test){
        std::cout << *p << std::endl;
    }

    return 1;
}