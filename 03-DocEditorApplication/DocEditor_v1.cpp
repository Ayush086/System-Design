#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class DocEditor {
private:
    vector<string> docuementElements;
    string renderedDocument;

public:
    // add plain text
    void addText(string text) {
        docuementElements.push_back(text);
    }

    // add image's path to load it
    void addImage(string imagePath) {
        docuementElements.push_back(imagePath);
    }

    // renders doc by checking type of elemenet at runtime (business logic)
    string renderDocument() {
        if (renderedDocument.empty()) {
            string result;
            for (auto element : docuementElements) {
                if (element.size() > 4 && (element.substr(element.size() - 4) == ".jpg" ||
                                           element.substr(element.size() - 4) == ".png")) {
                    result += "[Image: " + element + "]" + "\n";
                }
                else{
                    result += element + "\n";
                }
            }
            renderedDocument = result;
        }

        return renderedDocument;
    }

    void saveToFile() {
        ofstream file("doc.txt");
        if(file.is_open()) {
            file << renderDocument();
            file.close();
            cout << "Document saved to doc.txt" << endl;
        } else {
            cout << "Error: Failed to save document" << endl;
        }
    }
};

int main() {
    DocEditor editor;

    editor.addText("New Learnings!");
    editor.addImage("picture.jpg");

    cout << editor.renderDocument() << endl;
    editor.saveToFile();
    return 0;
}
