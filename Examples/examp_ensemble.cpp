#include <iostream>
#include "VotingClassifier.hpp"
#include "IMA.hpp"
#include "KNN.hpp"

int main(int argc, char *argv[]){
    mltk::Data<double> data;
    mltk::IMAp<double> ima;
    mltk::PerceptronPrimal<double> perc;
    mltk::KNNClassifier<double> knn(3);
    data.setClassesAtEnd(true);
    data.load("dataset_54_vehicle.csv");

    ima.setVerbose(0);
    mltk::VotingClassifier<double> voter(data, "hard", ima, perc, ima);

    voter.train();
    std::cout << "Original class: " << data[100]->Y() << "\nPredicted class: " << voter.evaluate(*data[100]) << std::endl;
    
}