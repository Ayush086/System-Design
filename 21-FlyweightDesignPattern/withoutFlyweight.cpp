#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <random>
#include <memory>
#include <chrono>
using namespace std;

class Asteroid {
private:
    // intrinsic properties (can be reused, duplicated)
    int length;
    int width;
    int weight;
    string colour;
    string texture;
    string material;

    // extrinsic properties (unique for each object)
    int posX, posY;
    int velocityX, velocityY;

public:
    Asteroid(int l, int w, int wt, string col, string tex, string mat, int pX, int pY, int vX, int vY) {
        this->length = l;
        this->width = w;
        this->weight = wt;
        this->colour = col;
        this->texture = tex;
        this->material = mat;
        this->posX = pX;
        this->posY = pY;
        this->velocityX = vX;
        this->velocityY = vY;
    }

    void render() {
        cout << "Rendering " << colour <<", " << texture << ", " << material <<
            " asteroid at (" << posX <<", " << posY << ") Size: " << length << "x" << width <<
            " Velocity: (" << velocityX <<", " << velocityY << ")" << endl;
    }

    static size_t getMemoryUsage() {
        return sizeof(int) * 7 +  // length, width, weight, x, y, velx, vely
                sizeof(string) * 3 +  // colour, texture, material
                32 * 3; // approx string data
    }
};

class SpaceGame {
private:
    vector<Asteroid*> asteroids;

public:
    void spawnAsteroids(int count) {
        cout << endl << "=== Spawning " << count << " asteroids ===" << endl;
        
        vector<string> colors = {"Red", "Blue", "Gray"};
        vector<string> textures = {"Rocky", "Metallic", "Icy"};
        vector<string> materials = {"Iron", "Stone", "Ice"};
        int sizes[] = {25, 35, 45};
        
        for (int i = 0; i < count; i++) {
            int type = i % 3;

            asteroids.push_back(new Asteroid(
                sizes[type], sizes[type], sizes[type] * 10,
                colors[type], textures[type], materials[type],
                100 + i * 50,          // x: 100, 150, 200, 250...
                200 + i * 30,          // y: 200, 230, 260, 290...
                1,                     // All move right with velocity 1
                2                      // All move down with velocity 2
            ));
        }
        
        cout << "Created " << asteroids.size() << " asteroid objects" << endl;
    }

    void renderAll() {
        cout << endl << "--- Rendering first 5 asteroids ---" << endl;
        for (int i = 0; i < min(5, (int)asteroids.size()); i++) {
            asteroids[i]->render();
        }
    }
    
    size_t calculateMemoryUsage() {
        return asteroids.size() * Asteroid::getMemoryUsage();
    }
    
    int getAsteroidCount() { 
        return asteroids.size(); 
    }
};


int main() {    
    const int ASTEROID_COUNT = 1000000; 
    
    cout << endl << "WITHOUT FLYWEIGHT PATTERN" << endl;
    SpaceGame* game = new SpaceGame();

    game->spawnAsteroids(ASTEROID_COUNT);

    // Show first 5 asteroids to see the pattern
    game->renderAll();

    // Calculate and display memory usage
    size_t totalMemory = game->calculateMemoryUsage();

    cout << endl << "=== MEMORY USAGE ===" << endl;
    cout << "Total asteroids: " << ASTEROID_COUNT << endl;                           
    cout << "Memory per asteroid: " << Asteroid::getMemoryUsage() << " bytes" << endl; 
    cout << "Total memory used: " << totalMemory << " bytes" << endl;           
    cout << "Memory in MB: " << totalMemory / (1024.0 * 1024.0) << " MB" << endl;     
    
    return 0;
}