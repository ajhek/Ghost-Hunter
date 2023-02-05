#include "defs.h"

/*
	Function: initGhost
  Purpose: Initializes ghost with given parameters and random ghostType
       in: ghost, building
   return: Initialized ghost with random type
*/
void initGhost(GhostType *ghost, BuildingType *b){
	int rand = randInt(1, 5);
	ghost->boredTimer = BOREDOM_MAX;
	if (rand == 1){
		ghost->type = POLTERGEIST;
	} else if(rand == 2){
		ghost->type = BANSHEE;
	} else if(rand == 3){
		ghost->type = BULLIES;
	} else if(rand == 4){
		ghost->type = PHANTOM;
	}
	
}

/*
	Function: placeGhost
  Purpose: Places the ghost in a random room in building
       in: ghost, building
   return: ghost with new position
*/
void placeGhost(GhostType *ghost, BuildingType *b){
	int count = b->rooms.count;
	int rand = randInt(1, count);
	RoomNodeType *currNode = b->rooms.head;
	for(int i=0; i < rand; i++){
		currNode = currNode->next;
	}
	ghost->room = currNode->room;
}

/*
	Function: isGhostInRoom
  Purpose: Detect if ghost is currently in given room
       in: ghost, room
   return: returns 1 if ghost is in room, 0 if it is not
*/
int isGhostInRoom(GhostType *ghost, RoomType *room){
	if(!memcmp(ghost->room->name, room->name, sizeof(room->name))){
		return 1;
	}
	return 0;
}

/*
	Function: ghostBehavior
  Purpose: dictates ghost behavior by random choice
  				 can choose to move, do nothing, or leave evidence
  				 if hunter is present in room, choice to move is removed
  				 if boredom reaches 0 or all hunters have left, loop closes
       in: ghost, building
   return: ghost with completed run cycle
*/
void ghostBehavior(GhostType *ghost, BuildingType *b){
	int options, rand;
	while(ghost->boredTimer > 0){
		options = 3;
		if(isHunterInRoom(b->hunters, ghost->room) && memcmp(ghost->room->name, b->rooms.head->room->name, sizeof(ghost->room->name))){
			ghost->boredTimer = BOREDOM_MAX;
			options = 2;
		}else{
			ghost->boredTimer--;
		}
		rand = randInt(0, options);
		if(rand == 1){
			manageEvidence(ghost->room, b, 0, MAX_HUNTERS+1);
		}else if(rand == 2){
			manageEvidence(ghost->room, b, 3, MAX_HUNTERS+1);
		}
		if(ghost->boredTimer == 0){
			ghost->room = NULL;
			printf("Ghost got bored and left.\n");
			return;
		}
		usleep(1000);
	}
}

/*
	Function: ghostMove
  Purpose: Moves the ghost to an adjacent room
       in: ghost
   return: ghost moved to new room
*/
void ghostMove(GhostType *ghost){
	RoomNodeType *currNode = ghost->room->connected;
	int rand;
	while(currNode->next != NULL){
		rand = randInt(0, 2);
		if(rand == 0){
			break;
		}else{
			currNode = currNode->next;
		}
	}
	ghost->room = currNode->room;
	printf("Ghost has moved to %s.\n", ghost->room->name);
}

/*
	Function: leaveEvidence
  Purpose: makes a call to generate ghostly evidence based on ghost type
       in: ghost
   return: ghost's current room with new evidence added
*/
void leaveEvidence(GhostType *ghost){
	int rand = randInt(0, 3);
	if(ghost->type == POLTERGEIST){
		if(rand == 0){
			genEv(ghost->room, EMF);
		}else if(rand == 1){
			genEv(ghost->room, TEMPERATURE);
		}else if(rand == 2){
			genEv(ghost->room, FINGERPRINTS);
		}
	}
	if(ghost->type == BANSHEE){
		if(rand == 0){
			genEv(ghost->room, EMF);
		}else if(rand == 1){
			genEv(ghost->room, TEMPERATURE);
		}else if(rand == 2){
			genEv(ghost->room, SOUND);
		}
	}
	if(ghost->type == BULLIES){
		if(rand == 0){
			genEv(ghost->room, EMF);
		}else if(rand == 1){
			genEv(ghost->room, FINGERPRINTS);
		}else if(rand == 2){
			genEv(ghost->room, SOUND);
		}
	}
	if(ghost->type == PHANTOM){
		if(rand == 0){
			genEv(ghost->room, TEMPERATURE);
		}else if(rand == 1){
			genEv(ghost->room, FINGERPRINTS);
		}else if(rand == 2){
			genEv(ghost->room, SOUND);
		}
	}
}
/*
	Function: freeGhost
  Purpose: frees allocated memory for ghost
       in: ghost
   return: N/A
*/
void freeGhost(GhostType *ghost){
	free(ghost);
}
