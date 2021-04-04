//
// Created by mateus on 03/04/2021.
//
#include <iostream>
#include <execution>
#include <tuple>
#include <fstream>
#include "../../Modules/Validation/Validation.hpp"
#include "kNNEnsembleWPDSM.hpp"
#include <map>

#define DATASET_FOLDER "../datasets/"

template<typename Data, typename Model>
double estimate_ensemble_acc(Data& data, Model ensemble){
    return mltk::validation::kkfold(data, ensemble, 10, 10, 42, 0).accuracy;
}

int main(){
    std::vector<std::string> datasets = {"pima.data", "sonar.data", "bupa.data", "wdbc.data", "ionosphere.data", "biodegradetion.csv",
                                         "vehicle.csv", "seismic-bumps.arff", "ThoraricSurgery.arff"};
    bool at_end[10] = {false,false,false,false,false,false,false,true, true,false};
    auto alphas = mltk::linspace<double>(0, 0.7, 8);
    std::vector<std::pair<std::string, bool>> datasets_pair;
    std::vector<std::vector<size_t>> orders;
    std::vector<std::tuple<std::string, double, double>> results;
    std::map<std::string, std::vector<std::tuple<std::vector<size_t>,double, double>>> log;

    for(int i = 0; i < datasets.size(); i++){
        datasets_pair.emplace_back(datasets[i], at_end[i]);
    }

    std::cout << "Creating permutations..." << std::endl;

    std::vector<size_t> subs_order(7);
    std::iota(subs_order.begin(), subs_order.end(), 0);

    do {
        orders.push_back(subs_order);
    }while(std::next_permutation(subs_order.begin(), subs_order.end()));
    int patience = int(orders.size()*0.009920635);

    std::cout << orders.size() << " combinations generated.\n" << std::endl;
    std::cout << "Patience= " << patience << std::endl;
    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::shuffle(orders.begin()+1, orders.end(), engine);
    std::for_each(std::execution::par, datasets_pair.begin(), datasets_pair.end(), [&results,&log, orders, alphas,
                                                                                        patience](auto& dataset_pair){
        mltk::Data<double> data("../datasets/" + dataset_pair.first, dataset_pair.second);

        std::clog << dataset_pair.first << std::endl;

        for(const auto alpha: alphas) {
            double max_acc = 0.0;
            int _patience = patience;

            std::for_each(std::execution::par_unseq, orders.begin(), orders.end(), [&max_acc, &log, &_patience, patience,
                                                                                    alpha, &data, dataset_pair](const auto& order){
               if(max_acc == 100.0 || _patience <= 0) return;
                mltk::ensemble::kNNEnsembleWPDSM<double> knn(data, 5, alpha, order);
                knn.set_test(true);
                double acc = estimate_ensemble_acc(data, knn);

                if (acc > max_acc) {
                    max_acc = acc;
                    _patience = patience;
                }else{
                    _patience--;
                }

                log[dataset_pair.first].emplace_back(order, alpha, acc);
                std::cout << dataset_pair.first << " " <<  alpha << "\t" << mltk::Point<size_t>(order) << "\t" << acc <<
                "\t" << max_acc << "\tpatience= " << _patience << std::endl;
            });

            std::cout << "\nDataset: " << dataset_pair.first << " - Max acc: " << max_acc << " - Alpha: "<< alpha <<"\n\n";
            results.emplace_back(dataset_pair.first, alpha, max_acc);
        }
    });

    std::ofstream file("max_acc_log/max_accs_DSM.csv");

    if(!file.is_open()){
        std::cerr << "Error opening results file!" << std::endl;
        return 1;
    }

    for(auto result: results){
        file << std::get<0>(result) << "," << std::get<1>(result) << "," << std::get<2>(result) << std::endl;
    }
    file.close();

    for(const auto& dataset: log){
        std::ofstream ofile("max_acc_log/max_acc_k_5_"+dataset.first+"_log.csv");
        if(!ofile.is_open()){
            std::cerr << "Error opening log file!" << std::endl;
            return 1;
        }
        for(auto result: dataset.second){
            auto feats = std::get<0>(result);

            ofile << "\"[";
            int i;
            for(i = 0; i < feats.size()-1; i++){
                ofile << feats[i] << ", ";
            }
            ofile << feats[i] << "]\",";
            ofile << std::get<1>(result) << "," << std::get<2>(result) << std::endl;
        }
        ofile.close();
    }
}