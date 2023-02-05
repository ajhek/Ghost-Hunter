#include "defs.h"

/*
	Function: initRoomList
  Purpose: initializes a new RoomList
       in: list
   return: initialized list
*/
void initRoomList(RoomListType *list){
	list->head = NULL;
	list->tail = NULL;
	list->count = 0;
}

/*
	Function: initRoom
  Purpose: initializes a new room
       in: room, name, building
   return: room with initialized parameters
*/
void initRoom(RoomType *room, char* name, BuildingType *b){
	strcpy(room->name, name);
	room->ghost = b->ghost;
	for(int i=0; i<MAX_HUNTERS; i++){
		room->hunters[i] = 0;
	}
}
/*
	Function: appendRoom
  Purpose: adds new room to given list
       in: list, room
   return: list with new room added
*/
void appendRoom(RoomListType *list, RoomNodeType *room){
	if(list->head == NULL){
		list->head = room;
		list->tail = room;
		list->count++;
	}else {
		list->tail->next = room;
		list->tail = room;
		list->count++;
	}
}

/*
	Function: connectRooms
  Purpose: creates a connection between 2 rooms
       in: room1, room2
   return: room1 with a connection to room2, room2 with a connection to room1
*/
void connectRooms(RoomType *room1, RoomType *room2){
	RoomNodeType *connectionNode1 = calloc(1, sizeof(RoomNodeType));
	RoomNodeType *connectionNode2 = calloc(1, sizeof(RoomNodeType));
	connectionNode1->room = room2;
	connectionNode1->next = room1->connected;
	room1->connected = connectionNode1;
	connectionNode2->room = room1;
	connectionNode2->next = room2->connected;
	room2->connected = connectionNode2;
}

/*
	Function: printRooms
  Purpose: prints all rooms to console, and indicates where ghost is
       in: rooms
   return: print to console with room information
*/
void printRooms(RoomListType *rooms){
	RoomNodeType *currNode = rooms->head;
	RoomNodeType *currCon;
	while(currNode != NULL){
		printf("%s: (", currNode->room->name);
		currCon = currNode->room->connected;
		while(currCon != NULL){
			printf("%s", currCon->room->name);
			currCon = currCon->next;
			if(currCon != NULL){
				printf(", ");
			}
		}
		printf(")\n");
		printEvidence(currNode->room->evidence);
		if(isGhostInRoom(currNode->room->ghost, currNode->room)){
			printf("^^Contains Ghost^^\n");
		}
		currNode = currNode->next;
	}
}

/*
	Function: manageEvidence
  Purpose: Buffer function for all ghost and hunter actions
  				 based on user input, can leave evidence, get evidence from room,
  				 move hunters and ghosts, and communicate
       in: room, building, action, id
   return: Completed action as outlined above
*/
void manageEvidence(RoomType *room, BuildingType *b, int action, int id){
	pthread_mutex_lock(&lock);
	if(action == 0){
		leaveEvidence(room->ghost);
	}
	if(action == 1){
		getEvidence(b->hunters[id], room);
	}
	if(action == 2 && isTwoHunters(b->hunters, room)){
		int id2 = -1;
		int rand;
		while(id2 == -1){
			rand = randInt(0, 4);
			if(room->hunters[rand] && b->hunters[rand]->id != id){
				id2 = rand;
				communicate(b->hunters[id], b->hunters[id2]);
			}
		}
	}
	if(action == 3 && id == MAX_HUNTERS+1){
		ghostMove(b->ghost);
	}else if(action == 3){
		hunterMove(b->hunters[id]);
	}
	pthread_mutex_unlock(&lock);
}

/*
	Function: freeRooms
  Purpose: frees allocated memory for a list of rooms
       in: rooms
   return: N/A
*/
void freeRooms(RoomListType *rooms){
	RoomNodeType *prevNode = rooms->head;
	RoomNodeType *currNode = rooms->head;
	RoomNodeType *conNode;
	RoomNodeType *prevConNode;
	while(currNode != NULL){
		prevNode = currNode;
		currNode = currNode->next;
		freeEvidence(prevNode->room->evidence);
		conNode = prevNode->room->connected;
		while(conNode != NULL){
			prevConNode = conNode;
			conNode = conNode->next;
			free(prevConNode);
		}
		free(prevNode->room);
		free(prevNode);
	}
}
