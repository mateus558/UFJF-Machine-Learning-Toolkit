#include <iostream>
#include <memory>
#include "includes/Data.hpp"
#include "includes/Visualization.hpp"
#include "includes/Perceptron.hpp"

using namespace std;

int main(){
    size_t i;
    Data<double> data("iris.data");
    PerceptronPrimal<double> perc(std::make_shared<Data<double> >
    (data));
	Visualization<double> plot(&data);
    Solution sol;

	plot.plot3D(1,2,3);
    perc.setMaxIterations(100);
    perc.setLearningRate(1.0);
    perc.train();

    sol = perc.getSolution();

    std::cout << "Weights: ";
    for(i = 0; i < data.getDim()-1; i++){
        std::cout << sol.w[i] << " ";
    }
    std::cout << sol.w[i] << "\n";
    std::cout << "bias: " << sol.bias << std::endl;

	plot.plot3DwithHyperplane(1,2,3,sol);

	while(true);
}

