/* Iterator Design Pattern                                                                                                                  Date: 16/07/2026 */
#include <iostream>
#include <vector>
#include <stack>
#include <string>
using namespace std;

// Iterator
template<typename T> // because datatype will be based on the iterator we're using
class Iterator {
public:
    virtual bool hasNext() = 0;
    virtual T next() = 0;
};

template<typename T>
class Iterable {
public:
    virtual Iterator<T>* getIterator() = 0;
};

/* Iterables */
// Linked list
class LinkedList : public Iterable<int> {
public:
    int data;
    LinkedList* next;

    LinkedList(int val) {
        this->data = val;
        this->next = nullptr;
    }

    Iterator<int>* getIterator() override;
};

// Binary tree
class BinaryTree : public Iterable<int> {
public:
    int data;
    BinaryTree* left;
    BinaryTree* right;

    BinaryTree(int data) {
        this->data = data;
        this->left = nullptr;
        this->right = nullptr;
    }

    Iterator<int>* getIterator() override;
};

// song + playlist
class Song {
public:
    string title;
    string artist;

    Song(const string& t, const string& a) {
        this->title = t;
        this->artist = a;
    }
};


class Playlist : public Iterable<Song> {
public:
    vector<Song> songs;

    void addSong(const Song& s) {
        this->songs.push_back(s);
    }

    Iterator<Song>* getIterator() override;
};


// iterators
class LinkedListIterator : public Iterator<int> {
private:
    LinkedList* current;

public:
    LinkedListIterator(LinkedList* head) {
        current = head;
    }

    bool hasNext() override {
        return current != nullptr;
    }

    int next() override {
        int val = current->data;
        current = current->next;
        return val;
    }
};

class BinaryTreeInorderIterator : public Iterator<int> {
private:
    stack<BinaryTree*> stk;
    BinaryTree* current;

    void pushLefts(BinaryTree* node) {
        while(node) {
            stk.push(node);
            node = node->left;
        }
    }

public:
    BinaryTreeInorderIterator(BinaryTree* root) {
        current = root;
        pushLefts(current);
    }

    bool hasNext() override {
        return !stk.empty();
    }

    int next() override {
        BinaryTree* node = stk.top();
        stk.pop();

        int val = node->data;
        if(node->right) pushLefts(node->right);

        return val;
    }
};


class PlayListIterator : public Iterator<Song> {
private:
    vector<Song> vec;
    size_t index;

public:
    PlayListIterator(vector<Song> v) {
        this->vec = v;
        this->index = 0;
    }

    bool hasNext() override {
        return index < vec.size();
    }

    Song next() override {

        return vec[index++];
    }
};


// linking
Iterator<int>* LinkedList::getIterator() {
    return new LinkedListIterator(this);
}

Iterator<int>* BinaryTree::getIterator() {
    return new BinaryTreeInorderIterator(this);
}

Iterator<Song>* Playlist::getIterator() {
    return new PlayListIterator(songs);
}


int main() {

    //------------------------------------------------
    // LinkedList: 1 → 2 → 3
    LinkedList* list = new LinkedList(1);
    list->next = new LinkedList(2);
    list->next->next = new LinkedList(3);

    Iterator<int>* iterator1 = list->getIterator();

    cout << "LinkedList contents: ";

    while (iterator1->hasNext()) {
        cout << iterator1->next() << " ";
    }

    //------------------------------------------------

    // BinaryTree:
    //    2
    //   / \
    //  1   3
    BinaryTree* root = new BinaryTree(2);
    root->left  = new BinaryTree(1);
    root->right = new BinaryTree(3);

    Iterator<int>* iterator2 = root->getIterator();

    cout << endl << "BinaryTree inorder: ";

    while (iterator2->hasNext()) {
        cout << iterator2->next() << " ";
    }

    //------------------------------------------------

    // Playlist
    Playlist playlist;
    playlist.addSong(Song("Admirin You", "Karan Aujla"));
    playlist.addSong(Song("Husn", "Anuv Jain"));

    Iterator<Song>* iterator3 = playlist.getIterator();

    cout << endl << "Playlist songs: " << endl;;

    while (iterator3->hasNext()) {
        Song s = iterator3->next();
        cout << "  " << s.title << " by " << s.artist << endl;
    }

    //------------------------------------------------
    delete list->next->next;
    delete list->next;
    delete list;
    delete root->left;
    delete root->right;
    delete root;

    return 0;
}