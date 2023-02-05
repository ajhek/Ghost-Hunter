#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_STR            64
#define FEAR_RATE           5
#define MAX_FEAR          100
#define MAX_HUNTERS         4
#define USLEEP_TIME     50000
#define BOREDOM_MAX        99

extern pthread_t tid[MAX_HUNTERS+1];
extern pthread_mutex_t lock;

// You may rename these types if you wish
typedef enum { EMF, TEMPERATURE, FINGERPRINTS, SOUND } EvidenceClassType;
typedef enum { POLTERGEIST, BANSHEE, BULLIES, PHANTOM } GhostClassType;
typedef struct Room RoomType;
typedef struct RoomNode RoomNodeType;

typedef struct{
	EvidenceClassType type;
	float data;
} EvidenceType;

typedef struct EvidenceNode{
	EvidenceType *ev;
	struct EvidenceNode *next;
} EvidenceNodeType;

typedef struct{
	GhostClassType type;
	RoomType *room;
	int boredTimer;
} GhostType;

typedef struct{
	RoomType *room;
	EvidenceClassType canRead;
	EvidenceNodeType *evCollected;
	char name[MAX_STR];
	int fear;
	int boredTimer;
	int id;
} HunterType;

struct Room{
	char name[MAX_STR];
	RoomNodeType *connected;
	GhostType *ghost;
	EvidenceNodeType *evidence;
	int hunters[MAX_HUNTERS];
};

struct RoomNode{
	RoomType *room;
	struct RoomNode *next;
};

typedef struct RoomList{
	RoomNodeType *head;
	RoomNodeType *tail;
	int count;
} RoomListType;

typedef struct{
	GhostType *ghost;
	HunterType *hunters[MAX_HUNTERS];
	RoomListType rooms;
} BuildingType;

int randInt(int, int);          // Generates a pseudorandom integer between the parameters
float randFloat(float, float);  // Generates a pseudorandom float between the parameters
void getNames(BuildingType*);
void *computeHunter(void*);
void *computeGhost(void*);
int determineGhostType(HunterType*);

void populateRooms(BuildingType*);   // Populates the building with sample data for rooms
void initBuilding(BuildingType*, char**);
void freeBuilding(BuildingType*);

void initRoomList(RoomListType*);
void initRoom(RoomType*, char*, BuildingType*);
void appendRoom(RoomListType*, RoomNodeType*);
void connectRooms(RoomType*, RoomType*);
void printRooms(RoomListType*);
void manageEvidence(RoomType*, BuildingType*, int, int);
void freeRooms(RoomListType*);

void initHunter(HunterType**, char*, EvidenceClassType, BuildingType*, int);
void initHunterArray(char**, BuildingType*);
void placeHunters(BuildingType*);
void updateHunterToRoom(RoomType*, HunterType*);
void hunterBehavior(HunterType*, BuildingType*);
void hunterMove(HunterType*);
int isHunterInRoom(HunterType**, RoomType*);
int isTwoHunters(HunterType**, RoomType*);
int checkData(HunterType*);
void printHunters(HunterType**);
void freeHunters(HunterType**);

void initGhost(GhostType*, BuildingType*);
int isGhostInRoom(GhostType*, RoomType*);
void placeGhost(GhostType*, BuildingType*);
void ghostBehavior(GhostType*, BuildingType*);
void ghostMove(GhostType*);
void leaveEvidence(GhostType*);
void freeGhost(GhostType*);

void initEvidence(EvidenceType*, EvidenceClassType, float);
void addToRoom(RoomType*, EvidenceType*, EvidenceNodeType*);
void genEv(RoomType*, EvidenceClassType);
void genReg(HunterType*);
void getEvidence(HunterType*, RoomType*);
void appendEvidence(EvidenceType*, HunterType*);
void communicate(HunterType*, HunterType*);
int isGhostly(EvidenceType*);
void printEvidence(EvidenceNodeType*);
void freeEvidence(EvidenceNodeType*);





