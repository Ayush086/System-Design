#pragma once
#include <iostream>
#include "../strategies/SequentialPlayStrategy.hpp"
#include "../strategies/CustomStrategy.hpp"
#include "../strategies/RandomPlayStrategy.hpp"
#include "../enums/PlayStrategyType.hpp"
using namespace std;

class StrategyManager {
private:
    static StrategyManager* instance;
    SequentialPlayStrategy* sequentialStrategy;
    RandomPlayStrategy* randomStrategy;
    CustomStrategy* customStrategy;

    StrategyManager() {
        sequentialStrategy = new SequentialPlayStrategy();
        randomStrategy = new RandomPlayStrategy();
        customStrategy = new CustomStrategy();
    }

public:
    static StrategyManager* getInstance() {
        if(!instance) instance = new StrategyManager();

        return instance;
    }

    PlayStrategy* getStrategy(PlayStrategyType type){
        if(type == PlayStrategyType::SEQUENTIAL){
            return sequentialStrategy;
        } else if(type == PlayStrategyType::RANDOM) {
            return randomStrategy;
        } else { // custom
            return customStrategy;
        }
    }
};

// create singleton instance
StrategyManager* StrategyManager::instance = nullptr;