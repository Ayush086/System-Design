/*  Template Design Pattern                                                                                                             16/06/2026      */
#include <iostream>
#include <string>
using namespace std;

// Base Class (template definition)
class ModelTrainer{
protected:
    void loadData(const string& path) {
        cout << "[Common]" << endl;
        cout << "Loading dataset from" << path << endl;
        cout << "----------------------------" << endl;
    }

    virtual void preprocessData() {
        cout << "[Common]" << endl;
        cout << "Performing Data preprocessing" << endl;
        cout << "Splitting and Normalization" << endl;
        cout << "Data Preprocessing Completed" << endl;
        cout << "----------------------------" << endl;
    }
    
    virtual void trainModel() = 0;
    virtual void evaluateModel() = 0; 
    
    virtual void saveModel() {
        cout << "[Common]" << endl;
        cout << "Model saving Started" << endl;
        cout << "Random Forest model's saving in progress" << endl;
        cout << "Model saved successfully!" << endl;
        cout << "----------------------------" << endl;
    }
    

    public:
    void trainingPipeline(const string& dataPath) {
        loadData(dataPath);
        preprocessData();
        trainModel();
        evaluateModel();
        saveModel();
    }
    
};


// concrete class : neural nets
class NeuralNetModel : public ModelTrainer {
protected:
    void trainModel() override {
        cout << "[Neural net] << model training started" << endl;
        cout << "[Neural net] training neural network for 50 epochs" << endl;
        cout << "[Neural net] << model training completed" << endl;
        cout << "----------------------------" << endl;
    }
    
    void evaluateModel() override {
        cout << "[Neural net] Model evaluation started" << endl;
        cout << "[Neural net] evualuating model's accuracy and loss on validation set" << endl;
        cout << "[Neural net] Model evaluated successfully!" << endl;
        cout << "----------------------------" << endl;
    }

    void saveModel() override {
        cout << "[Neural net] Model saving Started" << endl;
        cout << "[Neural net] Random Forest model's saving in progress" << endl;
        cout << "[Neural net] Model saved successfully!" << endl;
        cout << "----------------------------" << endl;
    }
};


// concrete class : supervised learning model
class MLModel : public ModelTrainer {
protected:
    void trainModel() override {
        cout << "[ML Model] << model training started" << endl;
        cout << "[ML Model] training neural network for 50 epochs" << endl;
        cout << "[ML Model] << model training completed" << endl;
        cout << "----------------------------" << endl;
    }
    
    void evaluateModel() override {
        cout << "[ML Model] Model evaluation started" << endl;
        cout << "[ML Model] evualuating model's accuracy and loss on validation set" << endl;
        cout << "[ML Model] Model evaluated successfully!" << endl;
        cout << "----------------------------" << endl;
    }
};


int main() {
    cout << "Neural Network Model's Pipeline" << endl;
    ModelTrainer* nn = new NeuralNetModel();
    nn->trainingPipeline("data/dataset.csv");

    cout << endl << endl << "ML Model's Pipeline" << endl;
    ModelTrainer* ml = new MLModel();
    ml->trainingPipeline("data/Images.zip");

    return 0;
}