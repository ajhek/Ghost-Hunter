#include "defs.h"

/*
	Function: initHunter
  Purpose: Initializes given hunter with given parameters
       in: hunter, name, canRead, building, id
   return: hunter with initialized parameters
*/
void initHunter(HunterType **hunter, char* name, EvidenceClassType e, BuildingType *b, int id){
	*hunter = calloc(1, sizeof(HunterType));
	strcpy((*hunter)->name, name);
	(*hunter)->canRead = e;
	(*hunter)->fear = 0;
	(*hunter)->boredTimer = BOREDOM_MAX;
	(*hunter)->id = id;
}

/*
	Function: initHunterArray
  Purpose: initializes an array of hunters in building
       in: names, building
   return: building with initialized array of hunters
*/
void initHunterArray(char** names, BuildingType *b){
	initHunter(&b->hunters[0], names[0], EMF, b, 0);
	initHunter(&b->hunters[1], names[1], TEMPERATURE, b, 1);
	initHunter(&b->hunters[2], names[2], FINGERPRINTS, b, 2);
	initHunter(&b->hunters[3], names[3], SOUND, b, 3);
	for (int i = 0; i < MAX_HUNTERS; i++) {
		free(names[i]);
	}
	free(names);
}

/*
	Function: placeHunters
  Purpose: places hunters at the head of building roomList
       in: building
   return: hunters placed in building roomList head
*/
void placeHunters(BuildingType *b){
	for(int i=0; i<MAX_HUNTERS; i++){
		printf("%s\n",b->hunters[i]->name);
		b->hunters[i]->room = b->rooms.head->room;
		updateHunterToRoom(b->rooms.head->room, b->hunters[i]);
	}
}

/*
	Function: updateHunterToRoom
  Purpose: updates the bool array of a room to reflect if hunter is there or not
       in: room, hunter
   return: room with updated hunter data
*/
void updateHunterToRoom(RoomType *room, HunterType *hunter){
	room->hunters[hunter->id] = !room->hunters[hunter->id];
}

/*
	Function: hunterBehavior
  Purpose: dictates hunter behavior by random choice
  				 can choose to move, collect evidence or communicate
  				 communicate option is only possible if more than one hunter is present
  				 if boredom reaches 0 or fear reaches MAX_FEAR, loop is closed
       in: hunter, building
   return: hunter with completed run cycle
*/
void hunterBehavior(HunterType *hunter, BuildingType *b){
	int rand, options;
	while(hunter->boredTimer > 0 && hunter->fear < MAX_FEAR){
		options = 2;
		if(isGhostInRoom(b->ghost, hunter->room)){
			printf("Hunter %s found the ghost.\n", hunter->name);
			hunter->fear += FEAR_RATE;
			hunter->boredTimer = BOREDOM_MAX;
		}else{
			hunter->boredTimer--;
		}if(isTwoHunters(b->hunters, hunter->room)){
			options = 3;
		}
		rand = randInt(0, options);
		if(rand == 0){
			manageEvidence(hunter->room, b, 1, hunter->id);
		}else if(rand == 1){
			manageEvidence(hunter->room, b, 3, hunter->id);
		}else if(rand == 2 && isTwoHunters(b->hunters, hunter->room)){
			manageEvidence(hunter->room, b, 2, hunter->id);
		}
		if(checkData(hunter)){
			updateHunterToRoom(hunter->room, hunter);
			hunter->room = b->rooms.head->room;
			printf("Hunter %s got what they came for.\n", hunter->name);
			return;
		}
		if(hunter->fear == MAX_FEAR){
			updateHunterToRoom(hunter->room, hunter);
			hunter->room = b->rooms.head->room;
			printf("Hunter %s flees in fear!\n", hunter->name);
			return;
		}
		if(hunter->boredTimer == 0){
			updateHunterToRoom(hunter->room, hunter);
			hunter->room = b->rooms.head->room;
			printf("Hunter %s got bored and left.\n", hunter->name);
		}
		usleep(1000);
	}
	updateHunterToRoom(hunter->room, hunter);
	hunter->room = b->rooms.head->room;
}

/*
	Function: hunterMove
  Purpose: Moves the hunter to an adjacent room
       in: hunter
   return: hunter with updated position
*/
void hunterMove(HunterType *hunter){
	RoomNodeType *currNode = hunter->room->connected;
	int rand = randInt(0, 10);
	while(rand >= 0){
		if(currNode->next == NULL){
			currNode = hunter->room->connected;
		}else{
			currNode = currNode->next;
		}
		rand--;
	}
	updateHunterToRoom(hunter->room, hunter);
	hunter->room = currNode->room;
	updateHunterToRoom(hunter->room, hunter);
	printf("Hunter %s has moved to %s.\n", hunter->name, hunter->room->name);
}

/*
	Function: isHunterInRoom
  Purpose: detects if a room currently contains a hunter
       in: hunterList, room
   return: returns 1 if a hunter is in room, 0 if not
*/
int isHunterInRoom(HunterType **hunter, RoomType *room){
	for(int i=0; i<MAX_HUNTERS; i++){
		if(!memcmp(hunter[i]->room->name, room->name, sizeof(room->name))){
			return 1;
		}
	}
	return 0;
}

/*
	Function: isTwoHunters
  Purpose: detects if more than one hunter is in a room
       in: hunterList, room
   return: returns 1 if 2 or more hunters, 0 if 1 or less
*/
int isTwoHunters(HunterType **hunter, RoomType *room){
	int count = room->hunters[0] + room->hunters[1] + room->hunters[2] + room->hunters[3];
	if(count >= 2){
		return 1;
	}
	return 0;
}

/*
	Function: checkData
  Purpose: Checks if a hunter has enough data to classify the ghost
       in: hunter
   return: returns 1 if enough data, 0 if not
*/
int checkData(HunterType *hunter){
	int emf = 0;
	int temp = 0;
	int prints = 0;
	int sound = 0;
	EvidenceNodeType *evNode = hunter->evCollected;
	while(evNode != NULL){
		if(evNode->ev->type == EMF && isGhostly(evNode->ev)){
			emf = 1;
		}
		if(evNode->ev->type == TEMPERATURE && isGhostly(evNode->ev)){
			temp = 1;
		}
		if(evNode->ev->type == FINGERPRINTS && isGhostly(evNode->ev)){
			prints = 1;
		}
		if(evNode->ev->type == SOUND && isGhostly(evNode->ev)){
			sound = 1;
		}
		evNode = evNode->next;
	}
	if((emf + temp + prints + sound) >= 3){
		return 1;
	}
	return 0;
}

/*
	Function: printHunters
  Purpose: prints hunter information to console
       in: hunters
   return: print output detailing info about all hunters
*/
void printHunters(HunterType** hunters){
	for(int i=0; i<MAX_HUNTERS; i++){
		printf("Hunter %d: %s ", i+1, hunters[i]->name);
		printf("(Scans %d)\n", hunters[i]->canRead);
	}
}

/*
	Function: freeHunters
  Purpose: free allocated memory for an array of hunters
       in: hunters
   return: N/A
*/
void freeHunters(HunterType** hunters){
	for(int i=0; i<MAX_HUNTERS; i++){

		freeEvidence(hunters[i]->evCollected);
		free(hunters[i]);
	}
}
