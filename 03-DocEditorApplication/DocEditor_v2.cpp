#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <string>

using namespace std;

// abstract class
class DocumentElement {
public:
    virtual string render() = 0; // it can be override
};


// implementation of various elements
class TextElement : public DocumentElement {
private:
    string text;

public:
    TextElement(string text) {
        this->text = text;
    }

    string render() override {
        return text;
    }
};

class ImageElement : public DocumentElement {
private:
    string imagePath;

public:
    ImageElement(string imagePath){
        this->imagePath = imagePath;
    }

    string render() override {
        return "[Image: " + imagePath + "]";
    }
};

class NewLineElement : public DocumentElement {
public: 
    string render() override {
        return "\n";
    }
};

class TabSpaceElement : public DocumentElement {
public:
    string render() override {
        return "\t";
    }
};


// class that will hold collection of elements
class Document {
private:
    vector<DocumentElement*> docElements;

public:
    void addElement(DocumentElement* element){
        docElements.push_back(element);
    }

    // renders the whole doc by concatenating all elements
    string render() {
        string result;
        for(auto element: docElements) {
            result += element->render();
        }
        return result;
    }
};


// persistance class for file saving functionalities
class Persistance {
public:
    virtual void save(string data) = 0;
};

// storage classes
class FileStorage : public Persistance {
public:
    void save(string data) override {
        ofstream outFile("document.txt");
        if(outFile) {
            outFile << data;
            outFile.close();
            cout << "Document saved successfully to document.txt" << endl;
        } else {
            cout << "Error occurred while saving the document" << endl;
        }
    }
};

class DBStorage : public Persistance {
public: 
    void save(string data) override {
        // save to DB
    }
};



class DocumentEditor {
private:
    Document* document;
    Persistance* storage;
    string renderedDocument;

public:
    // constructor
    DocumentEditor(Document* document, Persistance* storage) {
        this->document = document;
        this->storage = storage;
    }

    void addText(string text) {
        document->addElement(new TextElement(text));
    }

    void addImage(string imagePath) {
        document->addElement(new ImageElement(imagePath));
    }

    void addNewLine() {
        document->addElement(new NewLineElement());
    }

    void addTabSpace() {
        document->addElement(new TabSpaceElement());
    }

    string renderDocument() {
        if (renderedDocument.empty()) {
            renderedDocument = document->render();
        } 
        return renderedDocument;
    }

    void saveDocument() {
        storage->save(renderDocument());
    }
};


// client usage
int main()
{
    Document* document = new Document();
    Persistance* persistance = new FileStorage();

    DocumentEditor* editor = new DocumentEditor(document, persistance);

    // usage
    editor->addText("New Learning! v2");
    editor->addNewLine();
    editor->addText("Document Editor builder version 2");
    editor->addNewLine();
    // editor->addText("Testing");
    editor->addTabSpace();
    editor->addText("tab space implementation.");
    editor->addNewLine();
    editor->addImage("test/NewImage.jpg");
    editor->addNewLine();
    editor->addNewLine();
    editor->addText("Made by Ayush Uttarwar :)");

    cout << editor->renderDocument() << endl;

    editor->saveDocument();


    return 0;
}

