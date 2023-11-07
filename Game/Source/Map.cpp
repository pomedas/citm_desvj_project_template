
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"
#include "Scene.h"

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

    //Initialize the path
    frontier.Push(iPoint(4,4));
    visited.Add(iPoint(4,4));

    return ret;
}

bool Map::Start() {

    //Calls the functon to load the map, make sure that the filename is assigned
    SString mapPath = path;
    mapPath += name;
    Load(mapPath);

    //Loads texture to draw the path
    pathTex = app->tex->Load("Assets/Maps/MapMetadataIso.png");

    //Load texture to show the path
    tileX = app->tex->Load("Assets/Maps/x.png");

    // Find the navigation layer
    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.layers.start;
    navigationLayer = mapLayerItem->data;

    //Search the layer in the map that contains information for navigation
    while (mapLayerItem != NULL) {

        if (mapLayerItem->data->properties.GetProperty("Navigation") != NULL && mapLayerItem->data->properties.GetProperty("Navigation")->value) {
            navigationLayer = mapLayerItem->data;
            break;
        }
        mapLayerItem = mapLayerItem->next;
    }

    return true;
}

void Map::ResetPath()
{
    frontier.Clear();
    visited.Clear();

    frontier.Push(iPoint(4,4));
    visited.Add(iPoint(4,4));

    //initailize the cost matrix
    memset(costSoFar, 0, sizeof(uint) * COST_MAP_SIZE * COST_MAP_SIZE);
}

//Draw the visited nodes
void Map::DrawPath()
{
    iPoint point;

    // Draw visited
    ListItem<iPoint>* item = visited.start;

    while (item)
    {
        point = item->data;
        iPoint pos = MapToWorld(point.x, point.y);
        SDL_Rect rect = { 0,0,64,64 };
        app->render->DrawTexture(pathTex, pos.x, pos.y, &rect);

        item = item->next;
    }

    // Draw frontier
    for (uint i = 0; i < frontier.Count(); ++i)
    {
        point = *(frontier.Peek(i));
        iPoint pos = MapToWorld(point.x, point.y);
        SDL_Rect rect = { 64,0,64,64 };
        app->render->DrawTexture(pathTex, pos.x, pos.y, &rect);
    }

    // Draw path
    for (uint i = 0; i < pathTiles.Count(); ++i)
    {
        iPoint pos = MapToWorld(pathTiles[i].x, pathTiles[i].y);
        app->render->DrawTexture(tileX, pos.x, pos.y);
    }

}

void Map::ComputePath(int x, int y)
{
    path.Clear();
    iPoint goal = iPoint(x, y);

    // L10: TODO 2: Follow the breadcrumps to goal back to the origin
    // at each step, add the point into "pathTiles" dyn array (it will then draw automatically)

}

bool Map::IsWalkable(int x, int y) const
{
    bool isWalkable = false;
    
    // L10: DONE 3: return true only if x and y are within map limits
    // and the tile is walkable (tile id 0 in the navigation layer)
    int gid = navigationLayer->Get(x, y);
    int blockedGid = 26;
    if (x >= 0 && y >= 0 && x < mapData.width && y < mapData.height && gid != blockedGid) {
        isWalkable = true;
    }

    return isWalkable;
}


void Map::PropagateBFS()
{
    // L10 DONE 4: Check if we have reach a destination
    bool foundDestination = false;

    if (frontier.Count() > 0) {
        iPoint frontierPoint = *(frontier.Peek(0));
        iPoint playerPos = app->scene->GetPLayerPosition();
        if (frontierPoint == WorldToMap(playerPos.x,playerPos.y)) {
            foundDestination = true;

            // L10: TODO 2: When the destination is reach, call the function ComputePath
        }
    }

    // L10: DONE 1: If frontier queue contains elements
    // pop the last one and calculate its 4 neighbors
    if (frontier.Count() > 0 && !foundDestination) {

        iPoint frontierPoint;
        frontier.Pop(frontierPoint);

        List<iPoint> neighbors;
        if (IsWalkable(frontierPoint.x + 1, frontierPoint.y)) {
            iPoint p;
            neighbors.Add(p.Create(frontierPoint.x + 1, frontierPoint.y));
        }
        if (IsWalkable(frontierPoint.x, frontierPoint.y + 1)) {
            iPoint p;
            neighbors.Add(p.Create(frontierPoint.x, frontierPoint.y + 1));
        }
        if (IsWalkable(frontierPoint.x - 1, frontierPoint.y)) {
            iPoint p;
            neighbors.Add(p.Create(frontierPoint.x - 1, frontierPoint.y));
        }
        if (IsWalkable(frontierPoint.x, frontierPoint.y - 1)) {
            iPoint p;
            neighbors.Add(p.Create(frontierPoint.x, frontierPoint.y - 1));
        }

        // L10: DONE 2: For each neighbor, if not visited, add it
        // to the frontier queue and visited list
        ListItem<iPoint>* item = neighbors.start;

        while (item != NULL)
        {
            if (visited.Find(item->data) == -1)
            {
                frontier.Push(item->data);
                visited.Add(item->data);

                // L10: TODO 1: Record the direction to the previous node 
                // with the new list "breadcrumps"
            }
            item = item->next;
        }
    }
}

int Map::MovementCost(int x, int y) const
{
    int ret = -1;

    if ((x >= 0) && (x < mapData.width) && (y >= 0) && (y < mapData.height))
    {
        int gid = navigationLayer->Get(x, y); 

        if (gid == 25) ret = 10;
        else ret = 1;
    }

    return ret;
}

void Map::PropagateDijkstra()
{
    // L10: TODO 3: Taking BFS as a reference, implement the Dijkstra algorithm
    // use the 2 dimensional array "costSoFar" to track the accumulated costs
    // on each cell (is already reset to 0 automatically)

}

bool Map::Update(float dt)
{
    bool ret = true;

    if(mapLoaded == false)
        return false;

    ListItem<MapLayer*>* mapLayer; 
    mapLayer = mapData.layers.start;

    // L06: DONE 5: Prepare the loop to draw all tiles in a layer + DrawTexture()

    // iterates the layers in the map
    while (mapLayer != NULL) {
        //Check if the property Draw exist get the value, if it's true draw the lawyer
        if (mapLayer->data->properties.GetProperty("Draw") != NULL && mapLayer->data->properties.GetProperty("Draw")->value) {
            //iterate all tiles in a layer
            for (int i = 0; i < mapData.width; i++) {
                for (int j = 0; j < mapData.height; j++) {
                    //Get the gid from tile
                    int gid = mapLayer->data->Get(i, j);

                    //L08: DONE 3: Obtain the tile set using GetTilesetFromTileId
                    //Get the Rect from the tileSetTexture;
                    TileSet* tileSet = GetTilesetFromTileId(gid);
                    SDL_Rect tileRect = tileSet->GetRect(gid);
                    //SDL_Rect tileRect = mapData.tilesets.start->data->GetRect(gid); // (!!) we are using always the first tileset in the list

                    //Get the screen coordinates from the tile coordinates
                    iPoint mapCoord = MapToWorld(i, j);

                    // L06: DONE 9: Complete the draw function
                    app->render->DrawTexture(tileSet->texture, mapCoord.x, mapCoord.y, &tileRect);

                }
            }
        }
        mapLayer = mapLayer->next;
    }

    //Draw the visited tiles
    DrawPath();

    return ret;
}

// L08: DONE 2: Implement function to the Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int gid) const
{
    TileSet* set = NULL;

    ListItem<TileSet*>* tileSet;
    tileSet = mapData.tilesets.start;
    while (tileSet != NULL) {
        set = tileSet->data;
        if (gid >= tileSet->data->firstgid && gid < (tileSet->data->firstgid + tileSet->data->tilecount)) break;
        tileSet = tileSet->next;
    }

    return set;
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

    // L06: DONE 2: clean up all layer data
    ListItem<MapLayer*>* layerItem;
    layerItem = mapData.layers.start;

    while (layerItem != NULL) {
        RELEASE(layerItem->data->tiles);
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }

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

        // L09: DONE 2: Define a property to store the MapType and Load it from the map
        SString orientationStr = mapFileXML.child("map").attribute("orientation").as_string();
        if (orientationStr == "orthogonal") {
            mapData.orientation = MapOrientation::ORTOGRAPHIC;
        }
        else if (orientationStr == "isometric") {
            mapData.orientation = MapOrientation::ISOMETRIC;
        }
        else {
            LOG("Map orientation not found");
            ret = false; 
        }

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
            tileset->tilecount = tilesetNode.attribute("tilecount").as_int();

            //Load Tileset image
            SString mapTex = path;
            mapTex += tilesetNode.child("image").attribute("source").as_string();
            tileset->texture = app->tex->Load(mapTex.GetString());

            mapData.tilesets.Add(tileset);

        }

        // L06: DONE 3: Iterate all layers in the TMX and load each of them
        for (pugi::xml_node layerNode = mapFileXML.child("map").child("layer"); layerNode != NULL; layerNode = layerNode.next_sibling("layer")) {

            // L06: DONE 4: Implement a function that loads a single layer layer
            //Load the attributes and saved in a new MapLayer
            MapLayer* mapLayer = new MapLayer();
            mapLayer->id = layerNode.attribute("id").as_int();
            mapLayer->name = layerNode.attribute("name").as_string();
            mapLayer->width = layerNode.attribute("width").as_int();
            mapLayer->height = layerNode.attribute("height").as_int();

            //L08: DONE 6 Call Load Layer Properties
            LoadProperties(layerNode, mapLayer->properties);

            //Reserve the memory for the data 
            mapLayer->tiles = new uint[mapLayer->width * mapLayer->height];
            memset(mapLayer->tiles, 0, mapLayer->width * mapLayer->height);

            //Iterate over all the tiles and assign the values in the data array
            int i = 0;
            for (pugi::xml_node tileNode = layerNode.child("data").child("tile"); tileNode != NULL; tileNode = tileNode.next_sibling("tile")) {
                mapLayer->tiles[i] = tileNode.attribute("gid").as_uint();
                i++;
            }

            //add the layer to the map
            mapData.layers.Add(mapLayer);
        }


        // L07 DONE 3: Create colliders      
        // L07 DONE 7: Assign collider type
        // Later you can create a function here to load and create the colliders from the map
        
        // CALL TO CREATE COLLIDERS FROM MAP

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

            LOG("Layers----");

            ListItem<MapLayer*>* mapLayer;
            mapLayer = mapData.layers.start;

            while (mapLayer != NULL) {
                LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
                LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
                mapLayer = mapLayer->next;
            }
        }

        if (mapFileXML) mapFileXML.reset();
    }

    mapLoaded = ret;
    return ret;
}

// L06: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    // L09: DONE 3: Get the screen coordinates of tile positions for isometric maps 
    if (mapData.orientation == MapOrientation::ORTOGRAPHIC) {
        ret.x = x * mapData.tilewidth;
        ret.y = y * mapData.tileheight;
    }

    if (mapData.orientation == MapOrientation::ISOMETRIC) {
        ret.x = x * mapData.tilewidth / 2 - y * mapData.tilewidth / 2;
        ret.y = x * mapData.tileheight / 2 + y * mapData.tileheight / 2;
    }

    return ret;
}

// L08: DONE 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.propertyList.Add(p);
    }

    return ret;
}

// L08: DONE 7: Implement a method to get the value of a custom property
Properties::Property* Properties::GetProperty(const char* name)
{
    ListItem<Property*>* property = propertyList.start;
    Property* p = NULL;

    while (property)
    {
        if (property->data->name == name) {
            p = property->data;
            break;
        }
        property = property->next;
    }

    return p;
}

// L09: DONE 5: Add method WorldToMap to obtain  map coordinates from screen coordinates 
iPoint Map::WorldToMap(int x, int y) {

    iPoint ret(0, 0);

    if (mapData.orientation == MapOrientation::ORTOGRAPHIC) {
        ret.x = x / mapData.tilewidth;
        ret.y = y / mapData.tileheight;
    }

    if (mapData.orientation == MapOrientation::ISOMETRIC) {
        float half_width = mapData.tilewidth / 2;
        float half_height = mapData.tileheight / 2;
        ret.x = int((x / half_width + y / half_height) / 2);
        ret.y = int((y / half_height - (x / half_width)) / 2);
    }

    return ret;
}

int Map::GetTileWidth() {
    return mapData.tilewidth;
}

int Map::GetTileHeight() {
    return mapData.tileheight;
}

