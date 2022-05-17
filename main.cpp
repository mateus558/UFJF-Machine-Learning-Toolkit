#include <ufjfmltk/ufjfmltk.hpp>

using namespace mltk::classifier;

int main(){
    mltk::Data data = mltk::datasets::make_blobs(100, 2, 2, 1, -10, 10, true, 42).dataset;

    std::cout << "size: " << data.size() << std::endl;
    std::cout << "dim: " << data.dim() << std::endl;
    std::cout << "classes distribution: " << mltk::Point<size_t>(data.classesDistribution()) << std::endl;

    BalancedPerceptron ima(data);
    // ima.setMaxTime(20000);

    bool converged = ima.train();

    std::cout << "converged: " << ((converged)?"Reached convergency":"Did not reached convergency") << std::endl;
    if(!converged) return 0;
    mltk::visualize::Visualization vis(data);
    auto solution = ima.getSolution();
    std::cout << solution.w << " " << solution.bias << " " << solution.norm  << " " << solution.margin << " " << solution.margin/solution.w[1] << std::endl;
    vis.plot2DwithHyperplane(0, 1, ima.getSolution());

    return 1;
}