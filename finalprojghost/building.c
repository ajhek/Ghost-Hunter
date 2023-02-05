
#include "defs.h"

/*
	Function: populateRooms
  Purpose: Creates a list of rooms based on the map
  				 from the assignment specification
       in: building
   return: populated building roomlist
*/
void populateRooms(BuildingType* building) {
    // Create each room
    RoomType* van = calloc(1, sizeof(RoomType));
    initRoom(van, "Van", building);
    RoomType* hallway = calloc(1, sizeof(RoomType));
    initRoom(hallway, "Hallway", building);
    RoomType* master_bedroom = calloc(1, sizeof(RoomType));
    initRoom(master_bedroom, "Master Bedroom", building);
    RoomType* boys_bedroom = calloc(1, sizeof(RoomType));
    initRoom(boys_bedroom, "Boy's Bedroom", building);
    RoomType* bathroom = calloc(1, sizeof(RoomType));
    initRoom(bathroom, "Bathroom", building);
    RoomType* basement = calloc(1, sizeof(RoomType));
    initRoom(basement, "Basement", building);
    RoomType* basement_hallway = calloc(1, sizeof(RoomType));
    initRoom(basement_hallway, "Basement Hallway", building);
    RoomType* right_storage_room = calloc(1, sizeof(RoomType));
    initRoom(right_storage_room, "Right Storage Room", building);
    RoomType* left_storage_room = calloc(1, sizeof(RoomType));
    initRoom(left_storage_room, "Left Storage Room", building);
    RoomType* kitchen = calloc(1, sizeof(RoomType));
    initRoom(kitchen, "Kitchen", building);
    RoomType* living_room = calloc(1, sizeof(RoomType));
    initRoom(living_room, "Living Room", building);
    RoomType* garage = calloc(1, sizeof(RoomType));
    initRoom(garage, "Garage", building);
    RoomType* utility_room = calloc(1, sizeof(RoomType));
    initRoom(utility_room, "Utility Room", building);

    // create a linked list of rooms using RoomNodeType in the Building
    RoomNodeType* van_node = calloc(1, sizeof(RoomNodeType));
    van_node->room = van;
    RoomNodeType* hallway_node = calloc(1, sizeof(RoomNodeType));
    hallway_node->room = hallway;
    RoomNodeType* master_bedroom_node = calloc(1, sizeof(RoomNodeType));
    master_bedroom_node->room = master_bedroom;
    RoomNodeType* boys_bedroom_node = calloc(1, sizeof(RoomNodeType));
    boys_bedroom_node->room = boys_bedroom;
    RoomNodeType* bathroom_node = calloc(1, sizeof(RoomNodeType));
    bathroom_node->room = bathroom;
    RoomNodeType* basement_node = calloc(1, sizeof(RoomNodeType));
    basement_node->room = basement;
    RoomNodeType* basement_hallway_node = calloc(1, sizeof(RoomNodeType));
    basement_hallway_node->room = basement_hallway;
    RoomNodeType* right_storage_room_node = calloc(1, sizeof(RoomNodeType));
    right_storage_room_node->room = right_storage_room;
    RoomNodeType* left_storage_room_node = calloc(1, sizeof(RoomNodeType));
    left_storage_room_node->room = left_storage_room;
    RoomNodeType* kitchen_node = calloc(1, sizeof(RoomNodeType));
    kitchen_node->room = kitchen;
    RoomNodeType* living_room_node = calloc(1, sizeof(RoomNodeType));
    living_room_node->room = living_room;
    RoomNodeType* garage_node = calloc(1, sizeof(RoomNodeType));
    garage_node->room = garage;
    RoomNodeType* utility_room_node = calloc(1, sizeof(RoomNodeType));
    utility_room_node->room = utility_room;

    initRoomList(&building->rooms);

    appendRoom(&building->rooms, van_node);
    appendRoom(&building->rooms, hallway_node);
    appendRoom(&building->rooms, master_bedroom_node);
    appendRoom(&building->rooms, boys_bedroom_node);
    appendRoom(&building->rooms, bathroom_node);
    appendRoom(&building->rooms, basement_node);
    appendRoom(&building->rooms, basement_hallway_node);
    appendRoom(&building->rooms, right_storage_room_node);
    appendRoom(&building->rooms, left_storage_room_node);
    appendRoom(&building->rooms, kitchen_node);
    appendRoom(&building->rooms, living_room_node);
    appendRoom(&building->rooms, garage_node);
    appendRoom(&building->rooms, utility_room_node);

    // connect the rooms
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);
}

/*
	Function: initBuilding
  Purpose: Initializes a building with given parameters
       in: building, names
   return: Building with initialized roomlist, ghost and hunters
*/
void initBuilding(BuildingType* b, char** names){
	b->ghost = calloc(1, sizeof(GhostType));
	initGhost(b->ghost, b);
	initHunterArray(names, b);
	populateRooms(b);
	placeHunters(b);
	placeGhost(b->ghost, b);
	printRooms(&b->rooms);
	printHunters(b->hunters);
}

/*
	Function: freeBuilding
  Purpose: frees allocated memory for given building
       in: building
   return: N/A
*/
void freeBuilding(BuildingType *b){
	freeGhost(b->ghost);
	freeHunters(b->hunters);
	freeRooms(&b->rooms);
}

