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

    auto parted = mltk::validation::partTrainTest(data, 3, true, true);

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
    //std::cout << mltk::Point(allIndexes) << std::endl;
}

void test2() {
    mltk::Data data("datasets/iris.data");
    mltk::classifier::IMADual<double> ima_dual_inner(mltk::KernelType::INNER_PRODUCT);

    ima_dual_inner.setVerbose(0);

    std::cout << mltk::validation::kfold(data, ima_dual_inner, 10, true, 10, 3).accuracy << std::endl;
        std::cout << data << std::endl;

}

int main(){
    test2();
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