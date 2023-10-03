#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"

#include "PugiXml\src\pugixml.hpp"

// L05: TODO 1: Create a struct needed to hold the information to Map node
 
// L05: TODO 2: Create a struct to hold information for a TileSet
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

private:

	bool LoadMap(pugi::xml_node mapFile);
    bool LoadTileSet(pugi::xml_node mapFile);

public: 
    SString mapFileName;
	// L05: TODO 1: Declare a variable data of the struct MapData

private:
    bool mapLoaded;
};

#endif // __MAP_H__