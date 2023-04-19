#ifndef CONWAY
#define CONWAY

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <vector>

struct pos {
    int x;
    int y;
} typedef pos;

class Conway {
public:
    Conway(int size);
    void initSDL();
    void randomizeTable();
    char countNeighbors(pos position);
    void addNeighbors(pos position);
    void subNeighbors(pos position);
    void render();
    void update();
    void copyTable();
    void closeSDL();
    void getInput();
    int size;
    bool exit;
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event currentEvent;
    std::vector<std::vector<char>>* table;
    std::vector<std::vector<char>>* lastTable;
    std::vector<int>* wrapIndex;
};

#endif
