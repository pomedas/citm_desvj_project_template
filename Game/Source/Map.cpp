
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    return ret;
}

bool Map::Start() {

    //Calls the functon to load the map, make sure that the filename is assigned
    SString mapPath = path;
    mapPath += name;
    Load(mapPath);

    return true;
}

bool Map::Update(float dt)
{
    bool ret = true;

    if(mapLoaded == false)
        return false;

    // L05: TODO 6: Iterate all tilesets and draw all their 
    // images in 0,0 (you should have only one tileset for now)

    return ret;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L05: TODO 2: Make sure you clean up any memory allocated from tilesets/map

    return true;
}

// Load new map
bool Map::Load(SString mapFileName)
{
    bool ret = true;
    pugi::xml_document mapFileXML;

    // L05: TODO 3: Implement LoadMap to load the map properties
    // retrieve the paremeters of the <map> node and save it into map data

    //Fill mapData variable
         
    // L05: TODO 4: Implement the LoadTileSet function to load the tileset properties
    //ret = false; // Remove this line when implementing the function

    //Iterate the Tileset

      //Load Tileset attributes

      //Load Tileset image
   
    // L05: TODO 5: LOG all the data loaded iterate all tilesetsand LOG everything
    if(ret == true)
    {
        LOG("Successfully parsed map XML file :%s", "");
        LOG("width : %d height : %d",0,0);
        LOG("tile_width : %d tile_height : %d",0,0);
        
        LOG("Tilesets----");
        
        //iterate the tilesets
            LOG("name : %s firstgid : %d","", 0);
            LOG("tile width : %d tile height : %d", 0, 0);
            LOG("spacing : %d margin : %d",0, 0);
    }

    if(mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    return ret;
}

