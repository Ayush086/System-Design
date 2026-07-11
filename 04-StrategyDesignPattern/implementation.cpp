#include <iostream>
using namespace std;

// INTERFACES
// for walk
class WalkableRobo {
public:
    virtual void walk() = 0;
    virtual ~WalkableRobo() {}
};

// strategies for walk
class NormalWalk : public WalkableRobo {
public:
    void walk() override {
        cout << "Walking normally..." << endl;
    }
};

class NoWalk : public WalkableRobo {
public:
    void walk() override {
        cout << "Can't walk" << endl;
    }
};


// for talk
class TalkableRobo {
public:
    virtual void talk() = 0;
    virtual ~TalkableRobo() {}
};

// strategies for talk
class NormalTalk : public TalkableRobo {
public:
    void talk() override {
        cout << "Talking normally..." << endl;
    }
};

class NoTalk : public TalkableRobo {
public:
    void talk() override {
        cout << "Can't talk" << endl;
    }
};


// for fly
class FlyableRobo {
public:
    virtual void fly() = 0;
    virtual ~FlyableRobo() {}
};

// strategies for fly
class NormalFly : public FlyableRobo {
public:
    void fly() override {
        cout << "Flying normally..." << endl;
    }
};

class NoFly : public FlyableRobo {
public:
    void fly() override {
        cout << "Can't Fly" << endl;
    }
};


// BASE CLASS
class Robot {
protected:
    WalkableRobo* walkBehavior;
    TalkableRobo* talkBehavior;
    FlyableRobo* flyBehavior;

public:
    // constructor
    Robot(WalkableRobo* walk, TalkableRobo* talk, FlyableRobo* fly) {
        this->walkBehavior = walk;
        this->talkBehavior = talk;
        this->flyBehavior = fly;
    }

    void walk() {
        walkBehavior->walk();
    }
    void talk() {
        talkBehavior->talk();
    }
    void fly() {
        flyBehavior->fly();
    }

    virtual void projection() = 0;
};

/* Robot Types */
class CompanionRobot : public Robot {
public:
    CompanionRobot(WalkableRobo* w, TalkableRobo* t, FlyableRobo* f) : Robot(w, t, f) {}

    void projection() override {
        cout << "Companion robot's projection" << endl;
    }
};

class WorkerRobot : public Robot {
public:
    WorkerRobot(WalkableRobo* w, TalkableRobo* t, FlyableRobo* f) : Robot(w, t, f) {}

    void projection() override {
        cout << "worker robot's projection" << endl;
    }
};


// main function
int main() {
    Robot* robot1 = new CompanionRobot(
        new NormalWalk(),
        new NormalTalk(),
        new NoFly()
    );

    robot1->walk();
    robot1->talk();
    robot1->fly();
    robot1->projection();


    cout << "================================================" << endl << endl;


    Robot* robot2 = new WorkerRobot(
        new NoWalk(),
        new NoTalk(),
        new NormalFly()
    );

    robot2->walk();
    robot2->talk();
    robot2->fly();
    robot2->projection();
    
    return 0;
}











