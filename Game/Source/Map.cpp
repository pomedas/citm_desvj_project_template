
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

    // L05: DONE 6: Iterate all tilesets and draw all their 
    // images in 0,0 (you should have only one tileset for now)

    ListItem<TileSet*>* tileset;
    tileset = mapData.tilesets.start;
    while (tileset != NULL) {
        app->render->DrawTexture(tileset->data->texture,0,0);
        tileset = tileset->next;
    }
    
    return ret;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L05: DONE 2: Make sure you clean up any memory allocated from tilesets/map
    ListItem<TileSet*>* tileset;
    tileset = mapData.tilesets.start;
    
    while (tileset != NULL) {
        RELEASE(tileset->data);
        tileset = tileset->next;
    }

    mapData.tilesets.Clear();

    return true;
}

// Load new map
bool Map::Load(SString mapFileName)
{
    bool ret = true;

    // L05: DONE 3: Implement LoadMap to load the map properties
    // retrieve the paremeters of the <map> node and save it into map data

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
        ret = false;
    }
    else {

        //Fill mapData variable
        mapData.width = mapFileXML.child("map").attribute("width").as_int();
        mapData.height = mapFileXML.child("map").attribute("height").as_int();
        mapData.tilewidth = mapFileXML.child("map").attribute("tilewidth").as_int();
        mapData.tileheight = mapFileXML.child("map").attribute("tileheight").as_int();

        // L05: DONE 4: Implement the LoadTileSet function to load the tileset properties
       // Iterate the Tileset
        for (pugi::xml_node tilesetNode = mapFileXML.child("map").child("tileset"); tilesetNode != NULL; tilesetNode = tilesetNode.next_sibling("tileset")) {

            TileSet* tileset = new TileSet();

            //Load Tileset attributes
            tileset->name = tilesetNode.attribute("name").as_string();
            tileset->firstgid = tilesetNode.attribute("firstgid").as_int();
            tileset->margin = tilesetNode.attribute("margin").as_int();
            tileset->spacing = tilesetNode.attribute("spacing").as_int();
            tileset->tilewidth = tilesetNode.attribute("tilewidth").as_int();
            tileset->tileheight = tilesetNode.attribute("tileheight").as_int();
            tileset->columns = tilesetNode.attribute("columns").as_int();

            //Load Tileset image
            SString mapTex = path;
            mapTex += tilesetNode.child("image").attribute("source").as_string();
            tileset->texture = app->tex->Load(mapTex.GetString());

            mapData.tilesets.Add(tileset);

        }

        // L05: DONE 5: LOG all the data loaded iterate all tilesetsand LOG everything
        if (ret == true)
        {
            LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
            LOG("width : %d height : %d", mapData.width, mapData.height);
            LOG("tile_width : %d tile_height : %d", mapData.tilewidth, mapData.tileheight);

            LOG("Tilesets----");

            ListItem<TileSet*>* tileset;
            tileset = mapData.tilesets.start;
            while (tileset != NULL) {
                //iterate the tilesets
                LOG("name : %s firstgid : %d",tileset->data->name.GetString(), tileset->data->firstgid);
                LOG("tile width : %d tile height : %d", tileset->data->tilewidth, tileset->data->tileheight);
                LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
                tileset = tileset->next;
            }
        }

        if (mapFileXML) mapFileXML.reset();
    }

    mapLoaded = ret;
    return ret;
}


