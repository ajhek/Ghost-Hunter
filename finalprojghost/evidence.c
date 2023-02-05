#include "defs.h"
/*
	Function: initEvidence
  Purpose: Initializes an EvidenceType with a type and data value
       in: ev, type, data
   return: initialized ev
*/
void initEvidence(EvidenceType *ev, EvidenceClassType type, float data){
	ev->type = type;
	ev->data = data;
}

/*
	Function: addToRoom
  Purpose: Adds an EvidenceNode to the given rooms evidence list
       in: room, ev, evNode
   return: room with evNode (containing ev) added to it's evidencelist
*/
void addToRoom(RoomType *room, EvidenceType *ev, EvidenceNodeType *evNode){
	evNode->ev = ev;
	evNode->next = room->evidence;
	room->evidence = evNode;
}	

/*
	Function: addToHunter
  Purpose: Adds an EvidenceNode to the given hunters evidence list
       in: hunter, ev, evNode
   return: hunter with evNode (containing ev) added to it's evCollected
*/
void addToHunter(HunterType *hunter, EvidenceType *ev, EvidenceNodeType *evNode){
	evNode->ev = ev;
	evNode->next = hunter->evCollected;
	hunter->evCollected = evNode;
}

/*
	Function: genEv
  Purpose: Generates ghostly evidence according to input Evidence Type
  				 Then initializes it and adds it to given room
       in: room, EvidenceClass t
   return: room with new initialized EvidenceNode added to it's evidence
*/
void genEv(RoomType *room, EvidenceClassType t){
	float data;
	if(t==EMF){
		data = randFloat(4.70, 5.0);
		if(data >= 4.9)
			printf("The ghost left ghostly EMF data in the %s.\n", room->name);
		else
			printf("The ghost left standard EMF data in the %s.\n", room->name);
	}if(t==TEMPERATURE){
		data = randFloat(-10.0, 1.0);
		if(data <= 0)
			printf("The ghost left ghostly TEMPERATURE data in the %s.\n", room->name);
		else
			printf("The ghost left standard TEMPERATURE data in the %s.\n", room->name);
	}if(t==FINGERPRINTS){
		data = 1.0;
		printf("The ghost left ghostly FINGERPRINT data in the %s.\n", room->name);
	}if(t==SOUND){
		data = randFloat(65.0, 75.0);
		if(data>=70.0)
			printf("The ghost left ghostly SOUND data in the %s.\n", room->name);
		else
			printf("The ghost left standard SOUND data in the %s.\n", room->name);
	}
	EvidenceType* ev = calloc(1, sizeof(EvidenceType));
	EvidenceNodeType* evNode = calloc(1, sizeof(EvidenceNodeType));
	initEvidence(ev, t, data);
	addToRoom(room, ev, evNode);
}

/*
	Function: genReg
  Purpose: Generates standard non ghostly data based on hunter canRead
  				 Adds data to given hunter
       in: hunter
   return: hunter with new EvidenceType added to it's evidenceList
*/
void genReg(HunterType *hunter){
	float data;
	if(hunter->canRead==EMF){
		data = randFloat(0.0, 4.90);
		printf("Hunter %s found some random EMF data\n", hunter->name);
	}if(hunter->canRead==TEMPERATURE){
		data = randFloat(0.0, 27.0);
		printf("Hunter %s found some random TEMPERATURE data\n", hunter->name);
	}if(hunter->canRead==FINGERPRINTS){
		data = 0.0;
		printf("Hunter %s found some random FINGERPRINT data\n", hunter->name);
	}if(hunter->canRead==SOUND){
		data = randFloat(40.0, 70.0);
		printf("Hunter %s found some random SOUND data\n", hunter->name);
	}
	EvidenceType* ev = calloc(1, sizeof(EvidenceType));
	EvidenceNodeType* evNode = calloc(1, sizeof(EvidenceNodeType));
	initEvidence(ev, hunter->canRead, data);
	addToHunter(hunter, ev, evNode);
}

/*
	Function: getEvidence
  Purpose: Gets evidence for a hunter in a room
  				 If no evidence, generates standard evidence and adds to hunter
       in: hunter, room
   return: hunter with new evidence added to it, room with evidence removed from it
*/
void getEvidence(HunterType *hunter, RoomType *room){
	EvidenceNodeType *currNode = room->evidence;
	EvidenceNodeType *prevNode = NULL;
	while(currNode != NULL && !(currNode->ev->type == hunter->canRead)){
		prevNode = currNode;
		currNode = currNode->next;
	}
	if(currNode == NULL){
		genReg(hunter);
	}else{
		if(isGhostly(currNode->ev))
			printf("Hunter %s found some ghostly data in %s.\n", hunter->name, room->name);
		else
			printf("Hunter %s found some standard data in %s.\n", hunter->name, room->name);
		if(room->evidence->next == NULL){
			room->evidence = NULL;
		}else if(prevNode == NULL){
			room->evidence = currNode->next;
		}else{
			prevNode->next = currNode->next;
		}
		currNode->next = hunter->evCollected;
		hunter->evCollected = currNode;
	}
}

/*
	Function: appendEvidence
  Purpose: Creates a new evNode with given EvidenceType and adds it to hunter
       in: ev, hunter
   return: hunter with new Evidence added to their evCollected
*/
void appendEvidence(EvidenceType *ev, HunterType *hunter){
	EvidenceNodeType *evNode = calloc(1, sizeof(EvidenceNodeType));
	EvidenceType *newEv = calloc(1, sizeof(EvidenceType));
	newEv->type = ev->type;
	newEv->data = ev->data;
	evNode->ev = newEv;
	evNode->next = hunter->evCollected;
	hunter->evCollected = evNode;
}

/*
	Function: communicate
  Purpose: Adds the ghostly data from hunter1 to hunter2
       in: hunter1, hunter2
   return: hunter2 with all of hunter1's ghostly data added to it's evCollected
*/
void communicate(HunterType *hunter1, HunterType *hunter2){
	EvidenceNodeType *currNode = hunter1->evCollected;
	while(currNode != NULL){
		if(isGhostly(currNode->ev)){
			appendEvidence(currNode->ev, hunter2);
		}
		currNode = currNode->next;
	}
	printf("Hunter %s shared ghostly data with Hunter %s.\n", hunter1->name, hunter2->name);
}

/*
	Function: isGhostly
  Purpose: returns true or false if given evidence is ghostly
       in: ev
   return: returns 1 if evidence is ghostly, 0 if it is not
*/
int isGhostly(EvidenceType *ev){
	if(ev->type == EMF && ev->data >= 4.90 && ev->data <= 5.00){
		return 1;
	}
	if(ev->type == TEMPERATURE && ev->data >= -10.00 && ev->data <= 0.00){
		return 1;
	}
	if(ev->type == FINGERPRINTS && ev->data == 1.0){
		return 1;
	}
	if(ev->type == SOUND && ev->data >= 70.00 && ev->data <= 75.00){
		return 1;
	}
	return 0;
}

/*
	Function: freeEvidence
  Purpose: frees all connected evidenceNodes
       in: evNode
   return: N/A
*/
void freeEvidence(EvidenceNodeType *evNode){
	EvidenceNodeType *prevNode;
	while(evNode != NULL){
		prevNode = evNode;
		evNode = evNode->next;
		free(prevNode->ev);
		free(prevNode);
	}
}
/*
	Function: printEvidence
  Purpose: Prints all evidence in linked list to console
       in: evNode
   return: printed info to console
*/
void printEvidence(EvidenceNodeType *evNode){
	EvidenceNodeType *currNode = evNode;
	while(currNode != NULL){
		printf("%f ", currNode->ev->data);
		currNode = currNode->next;
	}
	printf("\n");
}








