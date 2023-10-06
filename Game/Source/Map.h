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

    // L06: DONE 7: Implement the method that receives the gid and returns a Rect

    SDL_Rect GetRect(uint gid) {
        SDL_Rect rect = { 0 };

        int relativeIndex = gid - firstgid;
        rect.w = tilewidth;
        rect.h = tileheight;
        rect.x = margin + (tilewidth + spacing) * (relativeIndex % columns);
        rect.y = margin + (tileheight + spacing) * (relativeIndex / columns);

        return rect;
    }
};

struct MapLayer
{
    // L06: DONE 1: Add the info to the MapLayer Struct
    int id;
    SString name;
    int width;
    int height;
    uint* tiles;

    // L06: DONE 6: Short function to get the gid value of x,y
    uint Get(int x, int y) const
    {
        return tiles[(y * width) + x];
    }
};

struct MapData
{
    int width;
    int height;
    int tilewidth;
    int tileheight;
    List<TileSet*> tilesets;

    // L06: DONE 2: Add a list/array of layers to the map
    List<MapLayer*> layers;
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

    // L06: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
    iPoint MapToWorld(int x, int y) const;

public: 
    SString name;
    SString path;

private:
    // L05: DONE 1: Declare a variable data of the struct MapData
    MapData mapData;
    bool mapLoaded;
};




#endif // __MAP_H__