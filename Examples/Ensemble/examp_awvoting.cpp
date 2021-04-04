//
// Created by mateuscmarim on 05/01/2021.
//

#include "AutoWeightedVoting.hpp"
#include "IMA.hpp"
#include "KNNClassifier.hpp"
#include "AdaBoostClassifier.hpp"
#include "PerceptronCommittee.hpp"
#include "../../Modules/Validation/Validation.hpp"

using namespace mltk;

double compute_acc(Data<double>& data, Learner<double> *learner){
    auto acc = 0;
    for(auto const& p: data){
        auto pred = learner->evaluate(*p);
        if(pred == p->Y()){
            acc++;
        }
    }
    return 100.0*double(acc)/ data.size();
}

int main(int argc, char* argv[]){
    mltk::Data<double> data("../datasets/bupa.data");
    mltk::classifier::IMAp<double> ima;
    auto valid = mltk::validation::partTrainTest(data, 10, 0);
    auto acc = 0.0;
    mltk::classifier::BalancedPerceptron<double> bp(valid.train);
    mltk::classifier::PerceptronPrimal<double> perc(mltk::make_data<double>(valid.train));

    ima.setVerbose(0);

    mltk::classifier::KNNClassifier<double> knn(valid.train, 5);
    mltk::ensemble::AdaBoostClassifier<double> ada(valid.train, bp, 10);
    ada.train();
    bp.setSeed(0);
    mltk::ensemble::AutoWeightedVoting<double> awvote(valid.train, true, ada, knn, perc);
    awvote.setMaxIterations(100);
    awvote.setSeed(0);
    knn.train();

    ima.setSamples(valid.train);
    knn.setSamples(valid.train);

    bp.train();

    perc.train();

    awvote.train();
    auto weights = awvote.getBestWeights();
    std::cout << "Best weights: " << weights << std::endl;
    auto voter = awvote.getVoter();
    voter.setWeights(weights.X());
    auto test = awvote.getValidPair().test;
    std::cout << "ima accuracy: " << compute_acc(valid.test, &ima) << std::endl;
    std::cout << "KNN accuracy: " << compute_acc(valid.test, &knn) << std::endl;
    std::cout << "BP accuracy: " << compute_acc(valid.test, &bp) << std::endl;
    std::cout << "Perceptron accuracy: " << compute_acc(valid.test, &perc) << std::endl;
    std::cout << "Adaboost accuracy: " << compute_acc(valid.test, &ada) << std::endl;
    std::cout << "Awvote acc: " << compute_acc(test, &voter)  << " " << voter.getWeights() << std::endl;
    //std::cout << "Awvote acc: " << 100 - validation::kfold(data, voter, 10, 0, 1) << std::endl;
    std::cout << "Best generation: " << awvote.getBestGeneration() << std::endl;
}
