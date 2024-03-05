#include <ufjfmltk/ufjfmltk.hpp>

using namespace mltk::classifier;

void test() {
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
}

void test1() {
    mltk::Data data = mltk::datasets::make_blobs(7, 2, 3, 1, -10, 10, true, 42).dataset;
    std::cout << data << std::endl;

    auto parted = mltk::validation::partTrainTest(data, 3, true, true, 42);

    // std::cout << parted.train << std::endl;

    // std::cout << "-------------------------------------------"  << std::endl;

    // std::cout << parted.test << std::endl;

    auto testIndexes = mltk::Point(parted.test.getIndex());
    auto trainIndexes = mltk::Point(parted.train.getIndex());
    
    //std::cout << trainIndexes << std::endl;
    //std::cout << testIndexes << std::endl;
    //mltk::classifier::KNNClassifier<double> knn(data, 3);
    std::vector<int> allIndexes(data.size());

    std::for_each(trainIndexes.begin(), trainIndexes.end(), [&allIndexes](auto &i){
        allIndexes[i] = i+1;
    });

    std::for_each(testIndexes.begin(), testIndexes.end(), [&allIndexes](auto &i){
        allIndexes[i] = i+1;
    });
    
    std::cout << parted.test << std::endl;

    std::cout << "-------------------------------------------"  << std::endl;
    std::cout << parted.train << std::endl;
    std::cout << "-------------------------------------------"  << std::endl;
    std::cout << data << std::endl;
    //std::cout << mltk::Point(allIndexes) << std::endl;
}

void test2() {
    mltk::Data data("datasets/iris.data");
    mltk::classifier::IMADual<double> ima_dual_inner(mltk::KernelType::INNER_PRODUCT);

    ima_dual_inner.setVerbose(0);

    std::cout << mltk::validation::kfold(data, ima_dual_inner, 10, true, 10, 3).accuracy << std::endl;
        std::cout << data << std::endl;

}

template <typename T>
std::vector<mltk::Data<T>> stratified_partition(mltk::Data<T> objects, int n) {
    std::multimap<std::string, mltk::PointPointer<T>> classified_objects;
    std::map<std::string, int> label_map; 

    objects.shuffle();

    for (const mltk::PointPointer<T> obj : objects) {
        classified_objects.insert({std::to_string(obj->Y()), obj});
    }

    int i = 0;
    for(const auto& label: objects.classes()) {
        label_map.insert({std::to_string(label), i});
        i++;
    }

    std::vector<mltk::Data<T>> partitions(n);

    // To keep track which partition should be filled next 
    std::vector<int> fillIndex(n, 0);

    // Smarter distribution
    for (const auto pair : classified_objects) {
        // Calculate the index based on respective label's count
        int i = fillIndex[label_map[pair.first]] % n;
        fillIndex[label_map[pair.first]]++;
        partitions[i].insertPoint(pair.second, true);
    }

    return partitions;
}

void test3(){
    mltk::Data data = mltk::datasets::make_blobs(7, 2, 3, 1, -10, 10, true, 42).dataset;
    std::cout << data << std::endl;

    auto partitions = stratified_partition(data, 10);

    for (const auto& partition : partitions) {
        std::cout << partition << std::endl;
        std::cout << "-------------------------------------------"  << std::endl;
    }

    std::cout << data << std::endl;
}


int main(){
    mltk::Data wine("iris.data");
    mltk::classifier::BalancedPerceptron<double> bp;
    mltk::ensemble::BaggingClassifier<double> bag(wine, bp, 50);
    std::cout << mltk::validation::kkfold(wine, bag, 10, 10).accuracy << std::endl;
    //test1();
    // mltk::Data data = mltk::datasets::make_blobs(7, 2, 3, 1, -10, 10, true, 42).dataset;
    // std::cout << data << std::endl;

    // // mltk::classifier::KNNClassifier<double> knn(3);

    // // auto result = mltk::validation::kkfold(data, knn, 3, 3, true);
    // // std::cout << "accuracy: " << result.accuracy << std::endl;
    //    mltk::classifier::BalancedPerceptron<double> bp;
    // mltk::classifier::IMAp<double> imap;
    // mltk::classifier::KNNClassifier<double, mltk::metrics::dist::Hassanat<double>> knn(3);
    // //mltk::classifier::IMADual<double> ima_dual(mltk::KernelType::GAUSSIAN, 0.5);
    // //ima_dual.setVerbose(0);
    // imap.setVerbose(0);
    
    // mltk::ensemble::VotingClassifier<double> voting(data, "hard", bp, knn, imap);
    // std::cout << "teste" << std::endl;
    // mltk::validation::kfold(data, voting, 10, true, 0, 1).accuracy;
    return 1;
}