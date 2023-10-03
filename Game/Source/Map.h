#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"

#include "PugiXml\src\pugixml.hpp"

// L05: DONE 1: Create a struct needed to hold the information to Map node
struct TileSet
{
    int firstgid;
    SString name;
    int tilewidth;
    int tileheight;
    int spacing;
    int margin;
    int tilecount;
    int columns;

    SDL_Texture* texture;
};

struct MapData
{
    int width;
    int height;
    int tilewidth;
    int tileheight;
    List<TileSet*> tilesets;
};

// L05: DONE 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!

class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node config);

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool Update(float dt);

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load(SString mapFileName);

public: 
    SString name;
    SString path;

private:
    // L05: DONE 1: Declare a variable data of the struct MapData
    MapData mapData;
    bool mapLoaded;
};




#endif // __MAP_H__