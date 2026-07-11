/* Composite Design Pattern                                                                             14/06/2026 */
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

// Base interface (component)
// abstract class
class FileSystemItem {
public:
    virtual ~FileSystemItem() {}
    virtual void ls(int indent = 0) = 0;
    virtual void openAll(int indent = 0) = 0;
    virtual int getSize() = 0;
    virtual FileSystemItem* cd(const string& name) = 0;
    virtual string getName() = 0;
    virtual bool isFolder() = 0;
};

// Leaf: file
class File : public FileSystemItem {
private:
    string name;
    int size;
public:
    File(const string& n, int s) {
        this->name = n;
        this->size = s;
    }

    void ls(int indent = 0) override {
        cout << string(indent, ' ') << name << endl;
    }

    void openAll(int indent = 0) override {
        cout << string(indent, ' ') << name << endl;
    }

    int getSize() override {
        return size;
    }

    FileSystemItem* cd(const string&) override {
        return nullptr; // not supported for files
    }

    string getName() override {
        return name;
    }

    bool isFolder() override {
        return false;
    }
};


// Component : folder
class Folder : public FileSystemItem {
private:
    vector<FileSystemItem*> children;
    string name;

public:
    Folder(const string& n){
        this->name = n;
    }

    void add(FileSystemItem* item) {
        children.push_back(item);
    }

    void ls(int indent = 0) override {
        for(auto child : children){
            if(child->isFolder()){
                cout << string(indent, ' ') << "(+) " << child->getName() << endl;
            } else {
                cout << string(indent, ' ') << child->getName() << endl;
            }
        }
    }

    void openAll(int indent = 0) override {
        cout << string(indent, ' ') << "(+) " << name << endl;
        for(auto child : children) {
            child->openAll(indent+4);
        }
    }

    int getSize() override {
        int total = 0;
        for(auto child: children) {
            total += child->getSize();
        }
        return total;
    }

    FileSystemItem* cd(const string& target) override {
        for(auto child: children){
            if(child->isFolder() && child->getName() == target) return child;
        }
        // not found or doesn't exist
        return nullptr;
    }

    string getName() override {
        return name;
    }

    bool isFolder() override {
        return true;
    }

    ~Folder() {
        for(auto c: children) delete c;
    }
};


int main() {
    Folder* root = new Folder("root");
    root->add(new File("f1.txt", 1));
    root->add(new File("f2.txt", 1));

    Folder* docs = new Folder("docs");
    docs->add(new File("doc.txt", 1));
    docs->add(new File("file.cpp", 1));
    root->add(docs);

    Folder* images = new Folder("images");
    images->add(new File("image.jpeg", 1));
    docs->add(images);

    // root->openAll();

    // root->ls();

    cout << "root size: " << root->getSize() << endl;

    FileSystemItem* cwd = root->cd("docs");
    if(cwd != nullptr){
        cwd->ls();
    }else {
        cout << "folder doesn't exist" << endl;
    }

    delete root;
    return 0;
}