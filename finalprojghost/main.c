#include "defs.h"

pthread_t tid[MAX_HUNTERS+1];
pthread_mutex_t lock;

typedef struct {
	HunterType *hunter;
	BuildingType *building;
} HunterPrimeStruct;
typedef struct {
	GhostType *ghost;
	BuildingType *building;
} GhostPrimeStruct;

int main(int argc, char *argv[])
{
    // Initialize a random seed for the random number generators
    srand(time(NULL));
		int i = 0;
		int err;
    // You may change this code; this is for demonstration purposes
    pthread_mutex_init(&lock, NULL);
    printf("Building init\n");
    BuildingType building;
    getNames(&building);
    
		while(i<MAX_HUNTERS){
			HunterPrimeStruct *args = malloc(sizeof(*args));
			args->hunter = building.hunters[i];
			args->building = &building;
			err = pthread_create(&tid[i], NULL, &computeHunter, args);
			if (err != 0){
				free(args);
				printf("\ncan't create thread :[%s]", strerror(err));
			}
			i++;
		}
		
		GhostPrimeStruct *args = malloc(sizeof(*args));
		args->ghost = building.ghost;
		args->building = &building;
		err = pthread_create(&(tid[i]), NULL, &computeGhost, args);
		if (err != 0){
			printf("\ncan't create thread :[%s]", strerror(err));
			free(args);
		}
		
		pthread_join(tid[0], NULL);
		pthread_join(tid[1], NULL);
		pthread_join(tid[2], NULL);
		pthread_join(tid[3], NULL);
		pthread_join(tid[4], NULL);
		pthread_mutex_destroy(&lock);
		
		int correct = -1;
		printf("--RESULTS--\n");
		for(int k=0; k<MAX_HUNTERS; k++){
			if(checkData(building.hunters[k])){
				printf("*       Hunter %s found enough info.\n", building.hunters[k]->name);
				correct = k;
			}else
				printf("*       Hunter %s left without finding enough info.\n", building.hunters[k]->name);
			}
		if(correct != -1)
			determineGhostType(building.hunters[correct]);
		else
			printf("The ghost wins!\n");
		
		freeBuilding(&building);
		
    return 0;
}

/*
	Function: determineGhost
  Purpose: Determines the type of ghost based on hunter evidenc
       in: hunter
   return: Prints output to screen
*/
int determineGhostType(HunterType *hunter){
	EvidenceNodeType *currNode = hunter->evCollected;
	int emf = 0;
	int temp = 0;
	int prints = 0;
	int sound = 0;
	while(currNode != NULL){
		if(currNode->ev->type == EMF && isGhostly(currNode->ev)){
			emf = 1;
		}
		if(currNode->ev->type == TEMPERATURE && isGhostly(currNode->ev)){
			temp = 1;
		}
		if(currNode->ev->type == FINGERPRINTS && isGhostly(currNode->ev)){
			prints = 1;
		}
		if(currNode->ev->type == SOUND && isGhostly(currNode->ev)){
			sound = 1;
		}
		currNode = currNode->next;
	}
	printf("EVIDENCE FOUND\n");
	if(emf == 1){
		printf("- EMF\n");
	}if(temp == 1){
		printf("- TEMPERATURE\n");
	}if(prints == 1){
		printf("- FINGERPRINTS\n");
	}if(sound == 1){
		printf("- SOUND\n");
	}
	if((emf + temp + prints) == 3){
		printf("Ghost was a POLTERGEIST.\n");
	}if((emf + temp + sound) == 3){
		printf("Ghost was a BANSHEE.\n");
	}if((emf + sound + prints) == 3){
		printf("Ghost was BULLIES.\n");
	}if((sound + temp + prints) == 3){
		printf("Ghost was a PHANTOM.\n");
	}
	
}

/*
	Function: computeHunter
  Purpose: Buffer function that executes thread to hunterBehavior
       in: args
   return: N/A
*/
void *computeHunter(void *args){
	HunterPrimeStruct *obj = args;
	hunterBehavior(obj->hunter, obj->building);
	free(obj);
	return NULL;
}

/*
	Function: computeGhost
  Purpose: Buffer function that executes thread to ghostBehavior
       in: args
   return: N/A
*/
void *computeGhost(void *args){
	GhostPrimeStruct *obj = args;
	ghostBehavior(obj->ghost, obj->building);
	free(obj);
	return NULL;
}

/*
	Function: getNames
  Purpose: Retrieves names from console input, then inits the building
       in: building
   return: N/A
*/
void getNames(BuildingType *b){
	char** names = malloc(sizeof(char*) * MAX_HUNTERS);
  for (int i = 0; i < MAX_HUNTERS; i++) {
    names[i] = malloc(sizeof(char) * MAX_STR);
    printf("Enter hunter name %d: ", i + 1);
    fgets(names[i], MAX_STR, stdin);
    names[i][strcspn(names[i], "\n")] = 0;
  }

  initBuilding(b, names);
}

/*
  Function:  randInt
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to (max - 1), inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [min, max-1) 
*/
int randInt(int min, int max)
{
    return rand() % (max - min) + min;
}

/*
  Function:  randFloat
  Purpose:   returns a pseudo randomly generated number, 
             in the range min to max, inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [0, max-1) 
*/
float randFloat(float a, float b) {
    // Get a percentage between rand() and the maximum
    float random = ((float) rand()) / (float) RAND_MAX;
    // Scale it to the range we want, and shift it
    return random * (b - a) + a;
}
