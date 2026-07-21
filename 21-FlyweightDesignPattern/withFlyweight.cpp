/*Flyweight Design Pattern                                                                                                 Date: 21/07/2026 */
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <random>
#include <memory>
#include <chrono>
using namespace std;

// Flyweight - Intrinsic specific
class AsteroidFlyweight {
private:
    // intrinsic properties (can be reused, duplicated)
    int length;
    int width;
    int weight;
    string colour;
    string texture;
    string material;

public:
    AsteroidFlyweight(int l, int w, int wt, string col, string tex, string mat) {
        this->length = l;
        this->width = w;
        this->weight = wt;
        this->colour = col;
        this->texture = tex;
        this->material = mat;
    }

    void render(int posX, int posY, int velocityX, int velocityY) {
        cout << "Rendering " << colour <<", " << texture << ", " << material <<
            " asteroid at (" << posX <<", " << posY << ") Size: " << length << "x" << width <<
            " Velocity: (" << velocityX <<", " << velocityY << ")" << endl;
    }

    static size_t getMemoryUsage() {
        return sizeof(int) * 3 +  // length, width, weight, x, y, velx, vely
                sizeof(string) * 3 +  // colour, texture, material
                32 * 3; // approx string data
    }
};

class AsteroidFactory {
private:
    static unordered_map<string, AsteroidFlyweight*> flyweights;

public:
    static AsteroidFlyweight* getAsteroid(int length, int width, int weight,
                            string colour, string texture, string material) {
        string key = to_string(length) + "-" + to_string(width) + "-" + to_string(weight) + "-" + colour + "-" + texture + "-" + material;

        if(flyweights.find(key) == flyweights.end()) {
            flyweights[key] = new AsteroidFlyweight(length, width, weight, colour, texture, material);
        }

        return flyweights[key];
    }

    static int getFlyweightCount() {
        return flyweights.size();
    }

    static size_t getTotalFlyweightMemory() {
        return flyweights.size() * AsteroidFlyweight::getMemoryUsage();
    }

    static void cleanup() {
        flyweights.clear();
    }
};

// static member definition
unordered_map<string, AsteroidFlyweight*> AsteroidFactory::flyweights;


// asteroid context - for extrinsic values
class AsteroidContext {
private:
    AsteroidFlyweight* flyweight;
    int posX, posY;
    int velocityX, velocityY;

public:
    AsteroidContext(AsteroidFlyweight* fw, int pX, int pY, int vX, int vY) {
        this->flyweight = fw;
        this->posX = pX;
        this->posY = pY;
        this->velocityX = vX;
        this->velocityY = vY;
    }

    void render() {
        flyweight->render(posX, posY, velocityX, velocityY);
    }

    void update() {
        posX += velocityX;
        posY += velocityY;
    }

    static size_t getMemoryUsage() {
        return sizeof(int) * 4 + sizeof(AsteroidFlyweight*);
    }
};


class SapceGameWithFlyweight {
private:
    vector<AsteroidContext*> asteroids;

public:
    void spawnAsteroids(int count) {
        cout << endl << "=== Spawning " << count << " asteroids ===" << endl;
        
        vector<string> colors = {"Red", "Blue", "Gray"};
        vector<string> textures = {"Rocky", "Metallic", "Icy"};
        vector<string> materials = {"Iron", "Stone", "Ice"};
        int sizes[] = {25, 35, 45};
        
        for (int i = 0; i < count; i++) {
            int type = i % 3;

            AsteroidFlyweight* flyweight = AsteroidFactory::getAsteroid(
                sizes[type], sizes[type], sizes[type] * 10,
                colors[type], textures[type], materials[type]
            );

            asteroids.push_back(new AsteroidContext(
                flyweight, 
                100 + i * 50,          // x: 100, 150, 200, 250...
                200 + i * 30,          // y: 200, 230, 260, 290...
                1,                     // All move right with velocity 1
                2                      // All move down with velocity 2
            ));
        }
        
        cout << "Created " << asteroids.size() << " asteroid objects" << endl;
        cout << "Total flyweight objects: " << AsteroidFactory::getFlyweightCount() << endl;
    }

    void renderAll() {
        cout << endl << "--- Rendering first 5 asteroids ---" << endl;
        for (int i = 0; i < min(5, (int)asteroids.size()); i++) {
            asteroids[i]->render();
        }
    }

    size_t calculateMemoryUsage() {
        size_t contextMemory = asteroids.size() * AsteroidContext::getMemoryUsage();
        size_t flyweightMemory = AsteroidFactory::getTotalFlyweightMemory();
        return contextMemory + flyweightMemory;
    }

    int getAsteroidCount() {
        return asteroids.size();
    }
};


int main() {
    const int ASTEROID_COUNT = 1000000; 
    
    cout << endl << "WITH FLYWEIGHT PATTERN" << endl;
    SapceGameWithFlyweight* game = new SapceGameWithFlyweight();

    game->spawnAsteroids(ASTEROID_COUNT);

    game->renderAll();

    size_t totalMemory = game->calculateMemoryUsage();

    cout << endl << "=== MEMORY USAGE ===" << endl;
    cout << "Total asteroids: " << ASTEROID_COUNT << endl;                           
    cout << "Memory per asteroid: " << AsteroidContext::getMemoryUsage() << " bytes" << endl; 
    cout << "Total memory used: " << totalMemory << " bytes" << endl;           
    cout << "Memory in MB: " << totalMemory / (1024.0 * 1024.0) << " MB" << endl;  

    return 0;
}