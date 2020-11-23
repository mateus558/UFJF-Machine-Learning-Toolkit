#ifdef __unix__
#include <dirent.h>
#elif _WIN32
#include <windows.h>
#endif
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <ctime>
#include <memory>
#include <iomanip>

#include "Visualization.hpp"
#include "Perceptron.hpp"
#include "LMS.hpp"
#include "IMA.hpp"
#include "SMO.hpp"
#include "RFE.hpp"
#include "Golub.hpp"
#include "Fisher.hpp"
#include "AOS.hpp"
#include "KNNClassifier.hpp"
#include "KNNRegressor.hpp"
#include "KMeans.hpp"

using namespace std;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
using namespace mltk;

int verbose = 1;
bool sair = false, inva = false;
double max_time = 110.0f;
string data_folder = "../DB/";
std::shared_ptr<Data<double> > samples(std::make_shared<Data<double> >());
Data<double> test_sample;
Data<double> train_sample;
Solution sol;
Visualization<double> plot(*samples);

//Menus utilities
void clear();
vector<string> list_datasets(bool list);
bool valid_file(string file);
void waitUserAction();
void exitProgram();

//Prints that are common in all menus
void header();
int selector();

//Functions to show menus options
void mainMenu();
void datasetMenu();
void dataMenu();
void VisualizationMenu();
void classifiersMenu();
void regressorsMenu();
void clusterersMenu();
void featureSelectionMenu();
void validationMenu();

//Functions to call the execution of the menus options
void mainOption(int);
void datasetOption(int);
void dataOption(int);
void VisualizationOption(int);
void classifiersOption(int);
void featureSelectionOption(int);
void regressorsOption(int);
void clusterersOption(int);
void primalRegressorsOption(int);
void dualRegressorsOption(int);
void primalClassifiersOption(int);
void dualClassifiersOption(int);
void validationOption(int);


int main(int argc, char* argv[]){
    if(argc > 1){
        samples->load(string(argv[1]));
    }

    while (true) {
        if(sair) break;
        clear();
        mainMenu();
    }

    return 0;
}

bool valid_file(string file){
    size_t i;
    bool flag = false;
    string ext;

    if(file.empty() || file.size() < 4)
        return false;

    for(i = file.size()-1; i >= 0; i--){
        if(file[i] == '.') break;
        string f(1, file[i]);
        ext = f + ext;
    }

    for(string type : types){
        if(type == ext) flag = true;
    }

    return flag;
}

vector<string> list_datasets(bool list){
    vector<string> files;

#ifdef __unix__
    DIR *dpdf;
    struct dirent *epdf;

    string temp = data_folder;
    temp.erase(temp.begin() + temp.size()-1);
    string path = "./" + data_folder;

    dpdf = opendir(path.c_str());
    if(dpdf != nullptr){
        while((epdf = readdir(dpdf))){
            string file = string(epdf->d_name);
            if(valid_file(file) && !file.empty()){

                if(list) cout << "[" << files.size() << "] " << file << endl;
                files.push_back(file);
            }
        }
    }

    closedir(dpdf);
#elif _WIN32
    HANDLE hFind;
    WIN32_FIND_DATA data1;
    string path = "..\\DB\\*.*";

    hFind = FindFirstFile(path.c_str(), &data1);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            string file_name(data1.cFileName);
            if(valid_file(file_name) && !file_name.empty()){
                if(list) cout << "[" << files.size() << "] " << file_name << endl;
                files.push_back(file_name);
            }
        } while (FindNextFile(hFind, &data1));
        FindClose(hFind);
    }
#else
    cout << "This system is not supported for this function..." << endl;
#endif

    return files;
}

void clear(){
#ifdef __unix__
    system("clear");
#elif _WIN32
    system("CLS");
#else
    int n;
        for (n = 0; n < 10; n++)
          printf( "\n\n\n\n\n\n\n\n\n\n" );
#endif
}

void exitProgram(){
    exit(0);
}

void waitUserAction(){
    cout << "\nPress ENTER to continue..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    cin.get();
}

void header(){
    cout << "         *----------------------------------------------------------* " << endl;
    cout << "         *                 Machine Learning Toolkit                 * " << endl;
    cout << "         *----------------------------------------------------------* " << endl;
    cout << endl;
    cout << "Select an option:\n" << endl;
}

int selector(){
    int o;
    string opt;

    if(inva){
        cout << endl;
        cout << "Invalid option." << endl;
        inva = false;
        cout << endl;
    }
    cout << " > ";
    cin >> opt;
    if(opt != "m") {
        istringstream iss(opt);
        iss >> o;
    }else{
        o = 109;
    }
    clear();
    return o;
}

void mainMenu(){
    int option;

    clear();
    header();

    cout << "1 - Dataset" << endl;
    cout << "2 - Data" << endl;
    cout << "3 - Data Visualization" << endl;
    cout << "4 - Classifiers" << endl;
    cout << "5 - Feature Selection" << endl;
    cout << "6 - Regressors" << endl;
    cout << "7 - Clusteres" << endl;
    cout << "8 - Validation" << endl;
    cout << endl;
    cout << "9 - Set Verbose" << endl;
    cout << "10 - Set Max Time" << endl;
    cout << endl;
    cout << "--------------------------" <<endl;
    cout << "0 - Exit" << endl;

    option = selector();
    mainOption(option);
}

void datasetMenu(){
    int option;

    clear();
    header();

    cout << "1 - Load a dataset" << endl;
    cout << "2 - Dataset information " << endl;
    cout << "3 - Features names" << endl;
    cout << "4 - Divide dataset in Train/Test" << endl;
    cout << "5 - Save Train/Test dataset" << endl;
    cout << endl;
    cout << "0 - Back to the main menu" << endl;

    option = selector();
    datasetOption(option);
}

void dataMenu(){
    int option;

    clear();
    header();

    cout << "1 - Insert features" << endl;
    cout << "2 - Remove features" << endl;
    cout << "3 - Remove point" << endl;
    cout << "4 - Data variance" << endl;
    cout << "5 - Data radius" << endl;
    cout << "6 - Distance from the center of the classes" << endl;
    cout << "7 - Normalize dataset" << endl;
    cout << "8 - Print dataset" << endl;
    cout << endl;
    cout << "0 - Back to the main menu" << endl;

    option = selector();
    dataOption(option);
}

void VisualizationMenu(){
    int option;

    clear();
    header();

    cout << "1 - Plot features in 2D" << endl;
    cout << "2 - Plot features in 3D" << endl;
    cout << "3 - Plot features in 2D with hyperplane" << endl;
    cout << "4 - Plot features in 3D with hyperplane" << endl;
    cout << endl;
    cout << "0 - Back to the main menu" << endl;

    option = selector();
    VisualizationOption(option);
}

void classifiersMenu(){
    int option;

    clear();
    header();

    cout << "1 - Primal Classifiers" << endl;
    cout << "2 - Dual Classifiers" << endl;
    cout << endl;
    cout << "0 - Back to the main menu" << endl;

    option = selector();
    classifiersOption(option);
}

void featureSelectionMenu(){
    int option;

    clear();
    header();

    cout << "1 - Recursive Feature Elimination (RFE)" << endl;
    cout << "2 - Golub" << endl;
    cout << "3 - Fisher" << endl;
    cout << "4 - Admissible Ordered Search (AOS)" << endl;
    cout << endl;
    cout << "0 - Back to the main menu" << endl;

    option = selector();
    featureSelectionOption(option);
}

void regressorsMenu() {
    int option;

    clear();
    header();

    cout << "1 - Primal Regressors" << endl;
    cout << "2 - Dual Regressors" << endl;
    cout << endl;
    cout << "0 - Back to the main menu" << endl;

    option = selector();
    regressorsOption(option);
}

void clusterersMenu(){
    int option;

    clear();
    header();

    cout << "1 - K-means" << endl;
    cout << endl;
    cout << "0 - Back to the main menu" << endl;

    option = selector();
    clusterersOption(option);
}

void validationMenu(){
    int opt;

    clear();
    header();

    cout << "1 - IMAp" << endl;
    cout << "2 - IMA Dual" << endl;
    cout << "3 - SMO" << endl;
    cout << endl;
    cout << "0 - Back to main menu." << endl;

    opt = selector();
    validationOption(opt);
}


void mainOption(int option){
    switch(option){
        case 1:
            datasetMenu();
            break;
        case 2:
            dataMenu();
            break;
        case 3:
            VisualizationMenu();
            break;
        case 4:
            classifiersMenu();
            break;
        case 5:
            featureSelectionMenu();
            break;
        case 6:
            regressorsMenu();
            break;
        case 7:
            clusterersMenu();
            break;
        case 8:
            validationMenu();
            waitUserAction();
            break;
        case 9:
            cout << "Actual Verbose = " << verbose << endl;
            cout << "Set verbose: ";
            cin >> verbose;
            waitUserAction();
            break;
        case 10:
            cout << "Actual MAX_TIME = " << max_time << endl;
            cout << "Enter the max time: ";
            cin >> max_time;

            waitUserAction();
            break;
        case 0:
            exitProgram();
            break;
        default:
            inva = true;
            break;
    }
    mainMenu();
}

void datasetOption(int option){
    size_t i = 0;
    bool list = false;
    vector<string> files;

    switch(option){
        case 1:
            if(samples->isEmpty()){
                string isReg;
                string sid, path;

                files = list_datasets(true);
                cout << endl;
                cout << "Enter the number of the DB (must be in the DB folder): ";
                cin >> sid;

                path = data_folder + files[utils::stoin(sid)];
                clock_t begin = clock();
                cout << "\n" << path << endl;
                samples->load(path);
                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to load.\n";
            }else{
                char y;
                cout << "Wish to load a new dataset?[y|n]" << endl;
                cout << " > ";
                cin >> y;

                if(y == 'y'){
                    samples->clear();
                    if(!test_sample.isEmpty()){
                        test_sample.clear();
                        train_sample.clear();
                    }
                    cout << "\nOld dataset erased, select this option again."<< endl;
                }
            }

            waitUserAction();
            break;
        case 2:
            if(!samples->isEmpty()){
                cout << "Dataset type: " << samples->getType() << endl;
                cout << "Number of features: " << samples->getDim() << endl;
                cout << "Number of samples: " << samples->getSize() << endl;
                if(samples->getType() == "Classification") {
                    vector<string> class_names = samples->getClassNames();
                    vector<size_t> class_frequency = samples->getClassesDistribution();
                    if(class_names.size() == 2) {
                        cout << "Negative points: " << samples->getNumberNegativePoints() << endl;
                        cout << "Positive points: " << samples->getNumberPositivePoints() << endl;
                    }else {
                        cout << "Class names and frequency [name - freq]: \n[";
                        for (i = 0; i < class_names.size() - 1; i++) {
                            cout << class_names[i] << " - "<< class_frequency[i] << ", ";
                        }
                        cout << class_names[i] << " - "<< class_frequency[i] << "]\n";
                    }
                }
                if(!test_sample.isEmpty()){
                    cout << "\n\nTest sample information\n\n";
                    cout << "Number of features: " << test_sample.getDim() << endl;
                    cout << "Number of samples: " << test_sample.getSize() << endl;
                    if(samples->getType() == "Classification") {
                        vector<string> class_names = samples->getClassNames();
                        vector<size_t> class_frequency = samples->getClassesDistribution();
                        if(class_names.size() == 2) {
                            cout << "Negative points: " << samples->getNumberNegativePoints() << endl;
                            cout << "Positive points: " << samples->getNumberPositivePoints() << endl;
                        }else {
                            cout << "Class names and frequency [name - freq]: \n[";
                            for (i = 0; i < class_names.size() - 1; i++) {
                                cout << class_names[i] << " - "<< class_frequency[i] << ", ";
                            }
                            cout << class_names[i] << " - "<< class_frequency[i] << "]\n";
                        }
                    }
                }

                if(!train_sample.isEmpty()){
                    cout << "\n\nTrain sample information\n\n";
                    cout << "Number of features: " << train_sample.getDim() << endl;
                    cout << "Number of samples: " << train_sample.getSize() << endl;
                    if(samples->getType() == "Classification") {
                        vector<string> class_names = samples->getClassNames();
                        vector<size_t> class_frequency = samples->getClassesDistribution();
                        if(class_names.size() == 2) {
                            cout << "Negative points: " << samples->getNumberNegativePoints() << endl;
                            cout << "Positive points: " << samples->getNumberPositivePoints() << endl;
                        }else {
                            cout << "Class names and frequency [name - freq]: \n[";
                            for (i = 0; i < class_names.size() - 1; i++) {
                                cout << class_names[i] << " - "<< class_frequency[i] << ", ";
                            }
                            cout << class_names[i] << " - "<< class_frequency[i] << "]\n";
                        }
                    }
                }
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;

        case 3:
            if(!samples->isEmpty()){
                vector<int> fnames = samples->getFeaturesNames();

                for(int f : fnames){
                    cout << f << " : ";
                }
                cout << endl;
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 4:
            if(!samples->isEmpty()){
                int fold;
                unsigned int seed;

                if(test_sample.isEmpty()){
                    cout << "K-Fold: ";
                    cin >> fold;
                    cout << "Seed for timestamps: ";
                    cin >> seed;

                    Validation<double> valid(*samples, nullptr, seed);

                    clock_t begin = clock();
                    valid.partTrainTest(fold);
                    test_sample = *valid.getTestSample();
                    train_sample = *valid.getTrainSample();
                    clock_t end = clock();

                    cout << "\nDone!" << endl;
                    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                    cout << "Size of the test sample: " << test_sample.getSize() << endl;
                    cout << endl;
                    cout << elapsed_secs << " seconds to compute.\n";

                }else cout << "Test sample already divided...\n";
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 5:
            /*if(_data != NULL && !_data->empty()){
                if(!test_sample){
                    cerr << "Divide the train/test datasets first...\n" << endl;
                }else{
                    string outfile = _data->get_dataset_name();
                    Sample *sample = Sample::copy(_data->get_samples());

                    outfile = outfile + "_train";
                    _data->write(outfile, sample, 0);
                    outfile = _data->get_dataset_name();
                    outfile = outfile + "_test";
                    _data->write(outfile, test_sample, 0);

                    delete sample;
                }
            }else cout << "Load a dataset first...\n\n";
            */
            waitUserAction();
            break;
        case 0:
            mainMenu();
            break;
        default:
            inva = true;
            break;
    }
    datasetMenu();
}

void dataOption(int option){
    int i = 0, j = 0;

    switch(option){
        case 1:
            if(!samples->isEmpty()){
                int totalFeat, flag_feat, f, fnamesize;
                vector<int> feats;
                vector<int> fnames = samples->getFeaturesNames();

                fnamesize = fnames.size();
                cout << "Insert how many features: ";
                cin >> totalFeat;
                feats.resize(totalFeat);

                for(i = 0; i < totalFeat; i++){
                    cout << "Feature " << i + 1 << ": ";
                    cin >> f;
                    feats[i] = f;

                    for(flag_feat = 0, j = 0; j < fnamesize; j++){
                        if(feats[i] == fnames[j]){
                            flag_feat = 1;
                        }
                    }

                    if(!flag_feat){
                        cout << "Feature " << feats[i] << " does not belongs to the set.\n";
                        i--;
                        cin.clear();
                    }
                }
                Data<double> *temp = samples->insertFeatures(feats);

                if(!temp->isEmpty()){
                    samples.reset();
                    samples = make_shared<Data<double> >(*temp);
                }else{
                    cerr << "Something went wrong." << endl;
                }
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 2:
            if(!samples->isEmpty()){
                int totalFeat, flag_feat, f, fnamesize;
                vector<int> feats;
                vector<int> fnames = samples->getFeaturesNames();

                fnamesize = fnames.size();

                cout << endl;
                while(true){
                    cout << "Remove how many features (-1 to cancel): ";
                    cin >> totalFeat;
                    if(totalFeat > fnamesize - 1){
                        clear();
                        cout << "Can't remove more features than exist." << endl;
                    }else if(totalFeat >= 0){
                        break;
                    }
                }

                for(i = 0; i < totalFeat; i++){
                    cout << "Feature " << i + 1 << ": ";
                    cin >> f;
                    feats.push_back(f);
                    for(flag_feat = 0, j = 0; j < fnamesize; j++){
                        if(feats[i] == fnames[j]){
                            flag_feat = 1;
                        }
                    }

                    if(!flag_feat){
                        cout << "Feature " << feats[i] << " does not belongs to the set.\n";
                        i--;
                    }
                }
                cout << endl;
                samples->removeFeatures(feats);

                fnames = samples->getFeaturesNames();

            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 3:
            if(!samples->isEmpty()){
                cout << "Point to remove index (-1 to cancel): ";
                cin >> i;

                if(i < 0){
                    break;
                }

                samples->removePoint(i);
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 4:
            if(!samples->isEmpty()){
                int index;

                cout << "Feature to be ignored (-1 doesnt ignore any feature): ";
                cin >> index;
                clock_t begin = clock();
                cout << endl;
                cout << "The variance values is: " << Statistics<double>::variance(samples, index) << endl;
                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 5:
            if(!samples->isEmpty()){
                int index, q;

                cout << "Feature to be ignored: ";
                cin >> index;
                cout << "Value of q [q = 1 | q = 2]: ";
                cin >> q;
                cout << endl;

                clock_t begin = clock();
                cout << "The value of the radius is: " << Statistics<double>::getRadius(samples, index, q) << endl;
                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 6:
            if(!samples->isEmpty()){
                int index;

                cout << "Feature to be ignored: ";
                cin >> index;
                cout << endl;

                clock_t begin = clock();
                cout << "The value of the center of the classes are: " << Statistics<double>::getDistCenters(samples, index) << endl;
                cout << endl;
                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

                cout << elapsed_secs << " seconds to compute.\n";
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 7:
            if(!samples->isEmpty()){
                int q, p;

                cout << "Value of q: ";
                cin >> q;
                cout << endl;

                if(q == -1.0){
                    p = 1.0;
                }else if(q == 1.0){
                    p = 100.0;
                }else{
                    p = q/(q-1.0);
                }

                clock_t begin = clock();
                samples->normalize(p);
                cout << "The dataset was normalized." << endl;
                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 8:
            if(!samples->isEmpty()){
                cout << *samples << endl;
            }else cout << "Load a dataset first...\n\n";

            waitUserAction();
            break;
        case 0:
            mainMenu();
            break;
        default:
            inva = true;
            break;
    }
    dataMenu();
}

void VisualizationOption(int opt){
    int x, y, z;

    plot.setSample(&(*samples));
    switch (opt) {
        case 1:
            cout << "Enter the feature to plot in the x-axis: ";
            cin >> x;
            cout << "Enter the feature to plot in the y-axis: ";
            cin >> y;

            plot.plot2D(x, y);
            break;
        case 2:
            cout << "Enter the feature to plot in the x-axis: ";
            cin >> x;
            cout << "Enter the feature to plot in the y-axis: ";
            cin >> y;
            cout << "Enter the feature to plot in the z-axis: ";
            cin >> z;
            plot.plot3D(x, y, z);
            break;
        case 3:
            if(sol.w.empty()){
                cout << "Run a classifier in the data first." << endl;
                waitUserAction();
                break;
            }
            cout << "Enter the feature to plot in the x-axis: ";
            cin >> x;
            cout << "Enter the feature to plot in the y-axis: ";
            cin >> y;

            plot.plot2DwithHyperplane(x, y, sol);
            break;
        case 4:
            if(sol.w.empty()){
                cout << "Run a classifier in the data first." << endl;
                waitUserAction();
                break;
            }
            cout << "Enter the feature to plot in the x-axis: ";
            cin >> x;
            cout << "Enter the feature to plot in the y-axis: ";
            cin >> y;
            cout << "Enter the feature to plot in the z-axis: ";
            cin >> z;

            plot.plot3DwithHyperplane(x, y, z, sol);
            break;
        case 0:
            mainMenu();
            break;
        default:
            inva = true;
            break;
    }
    VisualizationMenu();
}

void classifiersOption(int option){
    int opt;

    switch (option) {
        case 1:
            clear();
            header();

            cout << "1 - Perceptron Primal" << endl;
            cout << "2 - Perceptron Primal with fixed margin" << endl;
            cout << "3 - Incremental Margin Algorithm Primal (IMAp)" << endl;
            cout << "4 - K-Nearest Neighbors (KNN)" << endl;
            cout << endl;
            cout << "0 - Back to classifiers menu" << endl;
            cout << "m - Back to main menu." << endl;

            opt = selector();
            primalClassifiersOption(opt);
            break;
        case 2:
            clear();
            header();

            cout << "1 - Perceptron Dual" << endl;
            cout << "2 - Perceptron Dual with fixed margin" << endl;
            cout << "3 - Incremental Margin Algorithm Dual (IMA Dual)" << endl;
            cout << "4 - Sequential Minimal Optimization (SMO)" << endl;
            cout << endl;
            cout << "0 - Back to classifiers menu" << endl;
            cout << "m - Back to main menu." << endl;

            opt = selector();
            dualClassifiersOption(opt);
            break;
        case 0:
            mainMenu();
            break;
        default:
            inva = true;
            break;
    }
    classifiersMenu();
}

void featureSelectionOption(int option){
    double p, q, alpha_aprox, kernel_param = 0;
    int opt, flex, kernel_type, ddim, jump, branching, branch_form, choice_form, prof_look_ahead, cut;
    Timer time;
    classifier::IMAp<double> imap(samples);
    classifier::IMADual<double> imadual(samples);
    classifier::SMO<double> smo;
    Validation<double>::CrossValidation cv;
    RFE<double> rfe;
    Golub<double> golub;
    Fisher<double> fisher;
    AOS<double> aos;
    shared_ptr<Data<double> > res;
    KernelType type;

    clear();
    header();

    switch (option){
        case 1:
            if(!samples->isEmpty()) {
                cout << "Recursive Feature Elimination (RFE)" << endl;
                cout << "1 - IMAp" << endl;
                cout << "2 - IMA Dual" << endl;
                cout << "3 - SMO" << endl;
                cout << endl;
                cout << "0 - Back to Feature Selection menu" << endl;
                cout << "m - Back to Main menu" << endl;
                opt = selector();
                switch (opt) {
                    case 1:
                        cout << "q-norm value: ";
                        cin >> q;
                        cout << "Flexibilization value (0 - no flexibilization): ";
                        cin >> flex;
                        cout << "Alpha aproximation: ";
                        cin >> alpha_aprox;

                        if(q == -1.0){
                            p = 1.0;
                        }else if(q == 1.0){
                            p = 100.0;
                        }else{
                            p = q/(q-1.0);
                        }

                        imap.setqNorm(q);
                        imap.setFlexible(flex);
                        imap.setAlphaAprox(alpha_aprox);
                        imap.setMaxTime(max_time);
                        rfe.setClassifier(&imap);
                        break;
                    case 2:
                        cout << "Kernel (0)Inner Product (1)Polynomial (2)Gaussian: ";
                        cin >> kernel_type;

                        if (kernel_type == 1) {
                            cout << "Polynomial degree: ";
                        } else if (kernel_type == 2) {
                            cout << "Gaussian gamma: ";
                        }

                        if (kernel_type != 0) {
                            cin >> kernel_param;
                        }
                        switch(kernel_type){
                            case 0:
                                type = KernelType::INNER_PRODUCT;
                                break;
                            case 1:
                                type = KernelType::POLYNOMIAL;
                                break;
                            case 2:
                                type = KernelType::GAUSSIAN;
                                break;
                            default:
                                type = KernelType::INVALID_TYPE;
                                break;
                        }
                        imadual.setKernelParam(kernel_param);
                        imadual.setKernelType(type);
                        imadual.setMaxTime(max_time);
                        rfe.setClassifier(&imadual);
                        break;
                    case 3:
                        cout << "Kernel (0)Inner Product (1)Polynomial (2)Gaussian: ";
                        cin >> kernel_type;

                        if (kernel_type == 1) {
                            cout << "Polynomial degree: ";
                        } else if (kernel_type == 2) {
                            cout << "Gaussian gamma: ";
                        }

                        if (kernel_type != 0) {
                            cin >> kernel_param;
                        }
                        switch(kernel_type){
                            case 0:
                                type = KernelType::INNER_PRODUCT;
                                break;
                            case 1:
                                type = KernelType::POLYNOMIAL;
                                break;
                            case 2:
                                type = KernelType::GAUSSIAN;
                                break;
                            default:
                                type = KernelType::INVALID_TYPE;
                                break;
                        }
                        smo.setKernelParam(kernel_param);
                        smo.setKernelType(type);
                        rfe.setClassifier(&smo);
                        break;
                    case 0:
                        featureSelectionMenu();
                        break;
                    case 109:
                        mainMenu();
                        break;
                    default:
                        featureSelectionOption(1);
                        break;
                }
                clear();
                cout << endl;
                cout << "Desired dimension (max. " << samples->getDim() << "): ";
                cin >> ddim;
                cout << "Features eliminated at a time: ";
                cin >> jump;
                cout << endl;

                rfe.setJump(jump);
                rfe.setDepth(samples->getDim() - ddim);

                clear();
                cout << "\n--------- Cross-Validation ---------\n" << endl;
                cout << "Number of Cross-Validation: ";
                cin >> cv.qtde;

                if (cv.qtde > 0) {
                    cout << "K-Fold: ";
                    cin >> cv.fold;
                    cout << "From how many in how many dimensions: ";
                    cin >> cv.jump;
                    cout << "Error margin: ";
                    cin >> cv.limit_error;
                }
                rfe.setCrossValidation(&cv);
                rfe.setVerbose(verbose);
                rfe.setSamples(samples);
                clear();
                time.Reset();
                res = rfe.selectFeatures();
                samples.reset();
                samples = res;

                cout << time.Elapsed()/1000 << " seconds to compute.\n";
            }else{
                cout << "Load a dataset first..." << endl;
            }
            waitUserAction();
            featureSelectionOption(1);
            break;
        case 2:
            if(!samples->isEmpty()) {
                cout << "Golub" << endl;
                cout << "1 - IMAp" << endl;
                cout << "2 - IMA Dual" << endl;
                cout << "3 - SMO" << endl;
                cout << endl;
                cout << "0 - Back to Feature Selection menu" << endl;
                cout << "m - Back to Main menu" << endl;
                opt = selector();
                switch (opt) {
                    case 1:
                        cout << "q-norm value: ";
                        cin >> q;
                        cout << "Flexibilization value (0 - no flexibilization): ";
                        cin >> flex;
                        cout << "Alpha aproximation: ";
                        cin >> alpha_aprox;

                        if(q == -1.0){
                            p = 1.0;
                        }else if(q == 1.0){
                            p = 100.0;
                        }else{
                            p = q/(q-1.0);
                        }

                        imap.setqNorm(q);
                        imap.setFlexible(flex);
                        imap.setAlphaAprox(alpha_aprox);
                        imap.setMaxTime(max_time);
                        golub.setClassifier(&imap);
                        break;
                    case 2:
                        cout << "Kernel (0)Inner Product (1)Polynomial (2)Gaussian: ";
                        cin >> kernel_type;

                        if (kernel_type == 1) {
                            cout << "Polynomial degree: ";
                        } else if (kernel_type == 2) {
                            cout << "Gaussian gamma: ";
                        }

                        if (kernel_type != 0) {
                            cin >> kernel_param;
                        }
                        switch(kernel_type){
                            case 0:
                                type = KernelType::INNER_PRODUCT;
                                break;
                            case 1:
                                type = KernelType::POLYNOMIAL;
                                break;
                            case 2:
                                type = KernelType::GAUSSIAN;
                                break;
                            default:
                                type = KernelType::INVALID_TYPE;
                                break;
                        }
                        imadual.setKernelParam(kernel_param);
                        imadual.setKernelType(type);
                        imadual.setMaxTime(max_time);
                        golub.setClassifier(&imadual);
                        break;
                    case 3:
                        cout << "Kernel (0)Inner Product (1)Polynomial (2)Gaussian: ";
                        cin >> kernel_type;

                        if (kernel_type == 1) {
                            cout << "Polynomial degree: ";
                        } else if (kernel_type == 2) {
                            cout << "Gaussian gamma: ";
                        }

                        if (kernel_type != 0) {
                            cin >> kernel_param;
                        }
                        switch(kernel_type){
                            case 0:
                                type = KernelType::INNER_PRODUCT;
                                break;
                            case 1:
                                type = KernelType::POLYNOMIAL;
                                break;
                            case 2:
                                type = KernelType::GAUSSIAN;
                                break;
                            default:
                                type = KernelType::INVALID_TYPE;
                                break;
                        }
                        smo.setKernelParam(kernel_param);
                        smo.setKernelType(type);
                        golub.setClassifier(&smo);
                        break;
                    case 0:
                        featureSelectionMenu();
                        break;
                    case 109:
                        mainMenu();
                        break;
                    default:
                        featureSelectionOption(2);
                        break;
                }
                clear();
                cout << endl;
                cout << "Desired dimension (max. " << samples->getDim() << "): ";
                cin >> ddim;
                golub.setVerbose(verbose);
                golub.setFinalDimension(ddim);
                golub.setSamples(samples);
                clear();
                time.Reset();
                res = golub.selectFeatures();
                samples.reset();
                samples = res;

                cout << time.Elapsed()/1000 << " seconds to compute.\n";
            }else{
                cout << "Load a dataset first..." << endl;
            }
            waitUserAction();
            featureSelectionMenu();
            break;
        case 3:
            if(!samples->isEmpty()) {
                cout << "Fisher" << endl;
                cout << "1 - IMAp" << endl;
                cout << "2 - IMA Dual" << endl;
                cout << "3 - SMO" << endl;
                cout << endl;
                cout << "0 - Back to Feature Selection menu" << endl;
                cout << "m - Back to Main menu" << endl;
                opt = selector();
                switch (opt) {
                    case 1:
                        cout << "q-norm value: ";
                        cin >> q;
                        cout << "Flexibilization value (0 - no flexibilization): ";
                        cin >> flex;
                        cout << "Alpha aproximation: ";
                        cin >> alpha_aprox;

                        if(q == -1.0){
                            p = 1.0;
                        }else if(q == 1.0){
                            p = 100.0;
                        }else{
                            p = q/(q-1.0);
                        }

                        imap.setqNorm(q);
                        imap.setFlexible(flex);
                        imap.setAlphaAprox(alpha_aprox);
                        imap.setMaxTime(max_time);
                        fisher.setClassifier(&imap);
                        break;
                    case 2:
                        cout << "Kernel (0)Inner Product (1)Polynomial (2)Gaussian: ";
                        cin >> kernel_type;

                        if (kernel_type == 1) {
                            cout << "Polynomial degree: ";
                        } else if (kernel_type == 2) {
                            cout << "Gaussian gamma: ";
                        }

                        if (kernel_type != 0) {
                            cin >> kernel_param;
                        }
                        switch(kernel_type){
                            case 0:
                                type = KernelType::INNER_PRODUCT;
                                break;
                            case 1:
                                type = KernelType::POLYNOMIAL;
                                break;
                            case 2:
                                type = KernelType::GAUSSIAN;
                                break;
                            default:
                                type = KernelType::INVALID_TYPE;
                                break;
                        }
                        imadual.setKernelParam(kernel_param);
                        imadual.setKernelType(type);
                        imadual.setMaxTime(max_time);
                        fisher.setClassifier(&imadual);
                        break;
                    case 3:
                        cout << "Kernel (0)Inner Product (1)Polynomial (2)Gaussian: ";
                        cin >> kernel_type;

                        if (kernel_type == 1) {
                            cout << "Polynomial degree: ";
                        } else if (kernel_type == 2) {
                            cout << "Gaussian gamma: ";
                        }

                        if (kernel_type != 0) {
                            cin >> kernel_param;
                        }
                        switch(kernel_type){
                            case 0:
                                type = KernelType::INNER_PRODUCT;
                                break;
                            case 1:
                                type = KernelType::POLYNOMIAL;
                                break;
                            case 2:
                                type = KernelType::GAUSSIAN;
                                break;
                            default:
                                type = KernelType::INVALID_TYPE;
                                break;
                        }
                        smo.setKernelParam(kernel_param);
                        smo.setKernelType(type);
                        fisher.setClassifier(&smo);
                        break;
                    case 0:
                        featureSelectionMenu();
                        break;
                    case 109:
                        mainMenu();
                        break;
                    default:
                        featureSelectionOption(3);
                        break;
                }
                clear();
                cout << endl;
                cout << "Desired dimension (max. " << samples->getDim() << "): ";
                cin >> ddim;
                fisher.setVerbose(verbose);
                fisher.setFinalDimension(ddim);
                fisher.setSamples(samples);
                clear();
                time.Reset();
                res = fisher.selectFeatures();
                samples.reset();
                samples = res;

                cout << time.Elapsed()/1000 << " seconds to compute.\n";
            }else{
                cout << "Load a dataset first..." << endl;
            }
            waitUserAction();
            featureSelectionMenu();
            break;
        case 4:

            if(!samples->isEmpty()) {
                cout << "Admissible Ordered Search (AOS)" << endl;
                cout << "1 - IMAp" << endl;
                cout << "2 - IMA Dual" << endl;
                cout << "3 - SMO" << endl;
                cout << endl;
                cout << "0 - Back to Feature Selection menu" << endl;
                cout << "m - Back to Main menu" << endl;
                opt = selector();
                switch (opt) {
                    case 1:
                        cout << "q-norm value: ";
                        cin >> q;
                        cout << "Flexibilization value (0 - no flexibilization): ";
                        cin >> flex;
                        cout << "Alpha aproximation: ";
                        cin >> alpha_aprox;

                        if(q == -1.0){
                            p = 1.0;
                        }else if(q == 1.0){
                            p = 100.0;
                        }else{
                            p = q/(q-1.0);
                        }

                        imap.setqNorm(q);
                        imap.setFlexible(flex);
                        imap.setAlphaAprox(alpha_aprox);
                        imap.setMaxTime(max_time);
                        aos.setClassifier(&imap);
                        break;
                    case 2:
                        cout << "Kernel (0)Inner Product (1)Polynomial (2)Gaussian: ";
                        cin >> kernel_type;

                        if (kernel_type == 1) {
                            cout << "Polynomial degree: ";
                        } else if (kernel_type == 2) {
                            cout << "Gaussian gamma: ";
                        }

                        if (kernel_type != 0) {
                            cin >> kernel_param;
                        }
                        switch(kernel_type){
                            case 0:
                                type = KernelType::INNER_PRODUCT;
                                break;
                            case 1:
                                type = KernelType::POLYNOMIAL;
                                break;
                            case 2:
                                type = KernelType::GAUSSIAN;
                                break;
                            default:
                                type = KernelType::INVALID_TYPE;
                                break;
                        }
                        imadual.setKernelParam(kernel_param);
                        imadual.setKernelType(type);
                        imadual.setMaxTime(max_time);
                        aos.setClassifier(&imadual);
                        break;
                    case 3:
                        cout << "Kernel (0)Inner Product (1)Polynomial (2)Gaussian: ";
                        cin >> kernel_type;

                        if (kernel_type == 1) {
                            cout << "Polynomial degree: ";
                        } else if (kernel_type == 2) {
                            cout << "Gaussian gamma: ";
                        }

                        if (kernel_type != 0) {
                            cin >> kernel_param;
                        }
                        switch(kernel_type){
                            case 0:
                                type = KernelType::INNER_PRODUCT;
                                break;
                            case 1:
                                type = KernelType::POLYNOMIAL;
                                break;
                            case 2:
                                type = KernelType::GAUSSIAN;
                                break;
                            default:
                                type = KernelType::INVALID_TYPE;
                                break;
                        }
                        smo.setKernelParam(kernel_param);
                        smo.setKernelType(type);
                        aos.setClassifier(&smo);
                        break;
                    case 0:
                        featureSelectionMenu();
                        break;
                    case 109:
                        mainMenu();
                        break;
                    default:
                        featureSelectionOption(4);
                        break;
                }

                cout << endl;
                cout << "Desired dimension (max. " << samples->getDim() << "): ";
                cin >> ddim;
                cout << "Branching factor (max. " << samples->getDim() << "): ";
                cin >> branching;
                cout << endl;
                cout << "Branching sorting: (1)W (2)W/center (3)W*radius/center (4)W*radius (5)W*Golub (6)W*Fisher: ";
                cin >> branch_form;
                cout << "Choice: (1)Margin (2)Margin*Dist.Centers: ";
                cin >> choice_form;
                cout << "Look-Ahead depth: ";
                cin >> prof_look_ahead;
                cout << "Cut depth: ";
                cin >> cut;

                aos.setVerbose(verbose);
                aos.setFinalDimension(ddim);
                aos.setSamples(samples);
                aos.setCrossValidation(&cv);
                aos.setBreadth(branching);
                aos.setChoiceShape(choice_form);
                aos.setSortingShape(branch_form);
                aos.setLookAheadDepth(prof_look_ahead);
                aos.setCut(cut);

                clear();
                time.Reset();
                res = aos.selectFeatures();

                samples.reset();
                samples = res;

                cout << time.Elapsed()/1000 << " seconds to compute.\n";

            }else{
                cout << "Load a dataset first..." << endl;
            }

            waitUserAction();
            featureSelectionMenu();
            break;
        case 0:
            mainMenu();
            break;
        default:
            featureSelectionMenu();
            break;
    }
}

void regressorsOption(int option) {
    int opt;
    switch (option){
        case 1:
            clear();
            header();

            cout << "1 - Least Mean Squares Primal (LMS Primal)" << endl;
            cout << "2 - K-nearest neighbors (KNN)" << endl;
            cout << endl;
            cout << "0 - Back to regressors menu" << endl;
            cout << "m - Back to main menu." << endl;

            opt = selector();
            primalRegressorsOption(opt);
            break;
        case 2:
            clear();
            header();

            cout << "1 - Least Mean Squares Dual (LMS Dual)" << endl;
            cout << endl;
            cout << "0 - Back to regressors menu" << endl;
            cout << "m - Back to main menu." << endl;

            opt = selector();
            dualRegressorsOption(opt);
            break;
        case 0:
            mainMenu();
            break;
        default:
            inva = true;
            break;
    }
    regressorsMenu();
}

void clusterersOption(int option){
    switch (option){
        case 1:
            if(!samples->isEmpty()) {
                size_t k;
                size_t initialization;
                cout << "k value: ";
                cin >> k;
                cout << endl;
                cout << "Initialization [0 - random; 1 - kmeanspp]: ";
                cin >> initialization;
                clusterer::KMeans<double> kmeans(samples, k, (initialization == 0) ? "random" : "kmeanspp");
                kmeans.setMaxTime(max_time);
                kmeans.train();
                auto conf_m = Validation<double>::generateConfusionMatrix(kmeans, *samples);

                Data<double> _data;
                _data.copy(*samples);
                for(size_t i = 0; i < _data.getSize(); i++){
                    auto point = _data[i];
                    point->Y() = kmeans.evaluate(*point);
                }
                Visualization<double> vis(_data);
                vector<int> classes(_data.getClasses().size());
                iota(classes.begin(), classes.end(), 1);
                _data.setClasses(classes);
                cout << endl;
                utils::printConfusionMatrix(classes, conf_m);
                if(_data.getDim() >= 3 )
                    vis.plot3D(1,2,3);
                if(_data.getDim() == 2)
                    vis.plot2D(1, 2);
                waitUserAction();
            }else{
                cout << "Load a dataset first..." << endl;
            }
            break;
        case 0:
            clusterersMenu();
            break;
        case 109:
            mainMenu();
            break;
        default:
            inva = true;
            break;
    }
}

void primalRegressorsOption(int option) {
    double eta = 0.5;

    switch (option) {
        case 1:
            if(!samples->isEmpty()){
                std::cout << "Value of the learning rate: ";
                std::cin >> eta;

                regressor::LMSPrimal<double> lms(samples, eta, 2);

                lms.setMaxIterations(20);
                lms.setMaxTime(max_time);
                lms.train();
                sol = lms.getSolution();

                waitUserAction();
            }else{
                cout << "Load a dataset first..." << endl;
            }
            break;
        case 2:
            if(!samples->isEmpty()) {
                size_t k;
                cout << "k value: ";
                cin >> k;
                cout << "Enter a point to evaluate:" << endl;
                vector<double> feats(samples->getDim());
                for(size_t i = 0; i < samples->getDim(); i++){
                    cout << "Dim " << i << ": ";
                    cin >> feats[i];
                }
                cout << endl;

                regressor::KNNRegressor<double> knn(samples, k);
                double value = knn.evaluate(Point<double>(feats));

                cout << "Evaluated value: " << value << endl;

            }else{
                cout << "Load a dataset first..." << endl;
            }
            break;
        case 0:
            regressorsMenu();
            break;
        case 109:
            mainMenu();
            break;
        default:
            inva = true;
            break;
    }
    regressorsOption(1);
}

void dualRegressorsOption(int option) {
    switch (option) {
        case 1:
            if(!samples->isEmpty()){
                waitUserAction();
            }else{
                cout << "Load a dataset first..." << endl;
            }
            break;
        case 0:
            regressorsMenu();
            break;
        case 109:
            mainMenu();
            break;
        default:
            inva = true;
            break;
    }
    regressorsOption(2);
}

void validationOption(int option){
    int fold, qtde, kernel_type;
    int p, q, i, norm, flexible, svs;
    double rate, gamma, alpha_prox, kernel_param = 0;
    ValidationSolution val_sol;

    switch(option){
        case 1:
            if(!samples->isEmpty()){
                classifier::IMAp<double> imap(samples);

                cout << "Quantity of K-fold: ";
                cin >> qtde;
                cout << "Number of folds: ";
                cin >> fold;

                cout << "[1]p or [2]q norm: ";
                cin >> norm;
                cout << endl;

                if(norm == 1){
                    cout << "p-norm value: ";
                    cin >> p;
                    if(p == 1.0){
                        q = -1.0;
                    }else{
                        q = (int)(p/(p-1.0));
                    }
                }else{
                    cout << "q-norm value: ";
                    cin >> q;
                    if(q == -1.0){
                        p = 1.0;
                    }else if(q == 1.0){
                        p = 100.0;
                    }else{
                        p = (int)(q/(q-1.0));
                    }
                }
                cout << endl;
                cout << "Flexibilization value [0 - no flexibilization]: ";
                cin >> flexible;
                cout << endl;
                cout << "Alpha aproximation value [1 - alpha]: ";
                cin >> alpha_prox;
                cout << endl;
                cout << max_time << endl;

                imap.setMaxTime(max_time);
                imap.setpNorm(p);
                imap.setqNorm(q);
                imap.setVerbose(verbose);
                imap.setFlexible(flexible);
                imap.setAlphaAprox(alpha_prox);

                clock_t begin = clock();
                Validation<double> validate(*samples, &imap, 10);

                validate.setVerbose(verbose);
                validate.partTrainTest(fold);
                val_sol = validate.validation(fold, qtde);
                clock_t end = clock();

                cout << "\n\n   " << fold << "-Fold Cross Validation stats:" << endl;
                cout << "\nAccuracy: "<< val_sol.accuracy << endl;
                cout << "Precision: "<< val_sol.precision << endl;
                cout << "Recall: "<< val_sol.recall << endl;
                cout << endl;

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";
            }else{
                cout << "Load a dataset first..." << endl;
            }
            waitUserAction();
            break;
        case 2:
            if(!samples->isEmpty()){
                Kernel K;

                cout << "Quantity of K-fold: ";
                cin >> qtde;
                cout << "Number of folds: ";
                cin >> fold;

                cout << "Learning rate: ";
                cin >> rate;
                cout << "Kernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
                cin >> kernel_type;

                if(kernel_type != 0){
                    if(kernel_type == 1){
                        cout << "Polynomial degree: ";
                    }else{
                        cout << "Gaussian gamma: ";
                    }
                    cin >> kernel_param;
                }

                K.setType(kernel_type);
                K.setParam(kernel_param);

                clock_t begin = clock();
                classifier::IMADual<double> ima_dual(samples, &K, rate, nullptr);
                ima_dual.setMaxTime(max_time);

                Validation<double> validate(*samples, &ima_dual, 10);

                validate.setVerbose(verbose);
                validate.partTrainTest(fold);
                val_sol = validate.validation(fold, qtde);
                clock_t end = clock();

                cout << "\nAccuracy: "<< val_sol.accuracy << endl;
                cout << "Precision: "<< val_sol.precision << endl;
                cout << "Recall: "<< val_sol.recall << endl;
                cout << endl;

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";
            }else{
                cout << "Load a dataset first..." << endl;
            }
            waitUserAction();
            break;
        case 3:
            if(!samples->isEmpty()){
                Kernel K;

                cout << "Quantity of K-fold: ";
                cin >> qtde;
                cout << "Number of folds: ";
                cin >> fold;

                cout << "Kernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
                cin >> kernel_type;

                if(kernel_type != 0){
                    if(kernel_type == 1){
                        cout << "Polynomial degree: ";
                    }else{
                        cout << "Gaussian gamma: ";
                    }
                    cin >> kernel_param;
                }

                K.setType(kernel_type);
                K.setParam(kernel_param);

                clock_t begin = clock();
                classifier::SMO<double> smo(samples, &K, verbose);
                smo.setMaxTime(max_time);

                Validation<double> validate(*samples, &smo, 10);

                validate.setVerbose(verbose);
                validate.partTrainTest(fold);
                val_sol = validate.validation(fold, qtde);
                clock_t end = clock();

                cout << "\nAccuracy: "<< val_sol.accuracy << endl;
                cout << "Precision: "<< val_sol.precision << endl;
                cout << "Recall: "<< val_sol.recall << endl;
                cout << endl;

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";
            }else{
                cout << "Load a dataset first..." << endl;
            }
            waitUserAction();
            break;
        case 0: case 109:
            mainMenu();
            break;
        default:
            inva = true;
            break;
    }
    mainOption(8);
}

void primalClassifiersOption(int option){
    int p, q, i, norm, flexible, svs;
    double rate, gamma, alpha_prox;

    switch (option) {
        case 1:
            if(!samples->isEmpty()){
                cout << "Value of the learning rate: ";
                cin >> rate;
                cout << "Value of the q norm: ";
                cin >> q;
                cout << endl;

                classifier::PerceptronPrimal<double> perc(samples, q, rate);

                clock_t begin = clock();
                perc.train();
                sol = perc.getSolution();

                cout << "Number of steps through data: " << perc.getSteps() << endl;
                cout << "Number of updates: " << perc.getUpdates() << endl;
                cout << "Weights vector:" << endl;
                cout << "[";
                for(i = 0; i < sol.w.size(); i++){
                    cout << sol.w[i] << ", ";
                }
                cout << sol.bias <<  "]" << endl;
                cout << endl;

                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";

                waitUserAction();
            }else{
                cout << "Load a dataset first..." << endl;
            }
            break;
        case 2:
            if(!samples->isEmpty()){
                cout << "Value of the learning rate: ";
                cin >> rate;
                cout << "Value of the q norm: ";
                cin >> q;
                cout << "Gamma value: ";
                cin >> gamma;
                cout << endl;

                classifier::PerceptronFixedMarginPrimal<double> perc(samples, gamma, q, rate);

                clock_t begin = clock();
                perc.train();
                sol = perc.getSolution();

                cout << "Number of steps through data: " << perc.getSteps() << endl;
                cout << "Number of updates: " << perc.getUpdates() << endl;
                cout << "Weights vector:" << endl;
                cout << "[";
                for(i = 0; i < sol.w.size(); i++){
                    cout << sol.w[i] << ", ";
                }
                cout << sol.bias <<  "]" << endl;
                cout << endl;

                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";

                waitUserAction();
            }else{
                cout << "Load a dataset first..." << endl;
            }
            break;
        case 3:
            if(!samples->isEmpty()){
                cout << "[1]p or [2]q norm: ";
                cin >> norm;
                cout << endl;

                if(norm == 1){
                    cout << "p-norm value: ";
                    cin >> p;
                    if(p == 1.0){
                        q = -1.0;
                    }else{
                        q = p/(p-1.0);
                    }
                }else{
                    cout << "q-norm value: ";
                    cin >> q;
                    if(q == -1.0){
                        p = 1.0;
                    }else if(q == 1.0){
                        p = 100.0;
                    }else{
                        p = (int)(q/(q-1.0));
                    }
                }
                cout << endl;
                cout << "Flexibilization value [0 - no flexibilization]: ";
                cin >> flexible;
                cout << endl;

                cout << "Alpha aproximation value [1 - alpha]: ";
                cin >> alpha_prox;
                cout << endl;

                classifier::IMAp<double> imap(samples);

                imap.setMaxTime(max_time);
                imap.setpNorm(p);
                imap.setqNorm(q);
                imap.setFlexible(flexible);
                imap.setAlphaAprox(alpha_prox);

                clock_t begin = clock();

                if(imap.train()){
                    sol = imap.getSolution();
                    cout << "Training successful..." << endl;
                    cout << "\nMargin = " << sol.margin << ", Support Vectors = " << sol.svs << "\n" << endl;
                }

                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";

                waitUserAction();
            }else{
                cout << "Load a dataset first..." << endl;
            }
            break;
        case 4:
            if(!samples->isEmpty()) {
                size_t k;
                cout << "k value: ";
                cin >> k;
                cout << endl;

                classifier::KNNClassifier<double> knn(*samples, k);
                vector<string> class_names = samples->getClassNames();
                vector<int> classes = samples->getClasses();

                auto conf_matrix = Validation<double>::generateConfusionMatrix(knn, *samples);
                cout << "Confusion Matrix: " << endl;
                utils::printConfusionMatrix(classes, conf_matrix);
                waitUserAction();
            }else{
                cout << "Load a dataset first..." << endl;
            }
            break;
        case 0:
            classifiersMenu();
            break;
        case 109:
            mainMenu();
            break;
        default:
            inva = true;
            break;
    }
    classifiersOption(1);
}

void dualClassifiersOption(int option){
    int i, kernel_type = 0, kernel_param = 0;
    double rate;
    Kernel K;

    switch (option) {
        case 1:
            if(!samples->isEmpty()){
                cout << "Learning rate: ";
                cin >> rate;
                cout << "Kernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
                cin >> kernel_type;

                if(kernel_type != 0){
                    if(kernel_type == 1){
                        cout << "Polynomial degree: ";
                    }else{
                        cout << "Gaussian gamma: ";
                    }
                    cin >> kernel_param;
                }

                clock_t begin = clock();

                classifier::PerceptronDual<double> perc_dual(samples, rate, kernel_type, kernel_param);
                perc_dual.train();

                sol = perc_dual.getSolution();

                cout << endl;
                cout << "Alpha vector:" << endl;
                cout << "[";

                for(i = 0; i < sol.alpha.size(); i++){
                    cout << sol.alpha[i] << ", ";
                }

                cout << sol.bias <<  "]" << endl;
                cout << endl;
                cout << "Weights vector:" << endl;
                cout << "[";

                for(i = 0; i < sol.w.size(); i++){
                    cout << sol.w[i] << ", ";
                }

                cout << sol.bias <<  "]" << endl;
                cout << endl;

                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";
            }else{
                cout << "Load a dataset first..." << endl;
            }
            waitUserAction();
            break;
        case 2:
            if(!samples->isEmpty()){
                double gamma;
                cout << "Learning rate: ";
                cin >> rate;
                cout << "Gamma value: ";
                cin >> gamma;
                cout << "Kernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
                cin >> kernel_type;

                if(kernel_type != 0){
                    if(kernel_type == 1){
                        cout << "Polynomial degree: ";
                    }else{
                        cout << "Gaussian gamma: ";
                    }
                    cin >> kernel_param;
                }

                clock_t begin = clock();

                classifier::PerceptronFixedMarginDual<double> perc_fixmargin_dual(samples, gamma, rate, kernel_type, kernel_param);
                perc_fixmargin_dual.train();

                sol = perc_fixmargin_dual.getSolution();
                cout << endl;
                cout << "Alpha vector:" << endl;
                cout << "[";
                for(i = 0; i < sol.alpha.size(); i++){
                    cout << sol.alpha[i] << ", ";
                }
                cout << sol.bias <<  "]" << endl;
                cout << endl;

                cout << "Weights vector:" << endl;
                cout << "[";
                for(i = 0; i < sol.w.size(); i++){
                    cout << sol.w[i] << ", ";
                }
                cout << sol.bias <<  "]" << endl;
                cout << endl;

                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";
            }else{
                cout << "Load a dataset first..." << endl;
            }
            waitUserAction();
            break;
        case 3:
            if(!samples->isEmpty()){
                cout << "Learning rate: ";
                cin >> rate;
                cout << "Kernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
                cin >> kernel_type;

                if(kernel_type != 0){
                    if(kernel_type == 1){
                        cout << "Polynomial degree: ";
                    }else{
                        cout << "Gaussian gamma: ";
                    }
                    cin >> kernel_param;
                }

                K.setType(kernel_type);
                K.setParam(kernel_param);

                clock_t begin = clock();
                classifier::IMADual<double> ima_dual(samples, &K, rate, nullptr);

                ima_dual.setMaxTime(max_time);
                ima_dual.setVerbose(verbose);
                ima_dual.train();
                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";

                sol = ima_dual.getSolution();
            }else{
                cout << "Load a dataset first..." << endl;
            }
            waitUserAction();
            break;
        case 4:
            if(!samples->isEmpty()){
                cout << "Kernel [0]Inner Product [1]Polynomial [2]Gaussian: ";
                cin >> kernel_type;

                if(kernel_type != 0){
                    if(kernel_type == 1){
                        cout << "Polynomial degree: ";
                    }else{
                        cout << "Gaussian gamma: ";
                    }
                    cin >> kernel_param;
                }

                K.setParam(kernel_param);
                K.setType(kernel_type);

                clock_t begin = clock();
                classifier::SMO<double> smo(samples, &K, verbose);

                smo.setMaxTime(max_time);
                smo.setVerbose(verbose);
                smo.train();
                clock_t end = clock();

                double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
                cout << endl;
                cout << elapsed_secs << " seconds to compute.\n";

                sol = smo.getSolution();
                waitUserAction();
                classifiersOption(2);
            }else{
                cout << "Load a dataset first..." << endl;
            }
            waitUserAction();
            break;
        case 0:
            classifiersMenu();
            break;
        case 109:
            mainMenu();
            break;
        default:
            break;
    }
    classifiersOption(2);
}
