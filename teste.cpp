#include <iostream>
#include <memory>
#include "includes/Data.hpp"
//#include "includes/Visualization.hpp"
#include "includes/SMO.hpp"
#include "includes/Kernel.hpp"

using namespace std;

int main(){
    size_t i;
    Data<double> data("iris.data");
    Kernel k;
    k.setType(1);
    SMO<double> smo(std::make_shared<Data<double> > (data), &k);
//	Visualization<double> plot(&data);
    Solution sol;

    std::cout << data << std::endl;

//	plot.plot3D(1,2,3);
    
    smo.train();

    sol = smo.getSolution();

    std::cout << "Weights: ";
    for(i = 0; i < data.getDim()-1; i++){
        std::cout << sol.w[i] << " ";
    }
    std::cout << sol.w[i] << "\n";
    std::cout << "bias: " << sol.bias << std::endl;
    cout << smo.evaluate(*data[0]) << endl;
	//plot.plot3DwithHyperplane(1,2,3,sol);

	//while(true);
}