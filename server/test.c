#include "server_functions.h"

int main(int argc, char const *argv[]){
	int i;
	char bf[BUFFERSIZE];

	Near *Tester;
	Game newGame;
	if (!(Tester = calloc(1,sizeof(Near))))
		perror("calloc");
	//memset(Tester,0,sizeof(Near));

	Player *TestList = NULL;
	for (i = 0; i < 10; i++) {
		newPlayer(&TestList);
	}

	clearListPlayer(&TestList);

	Tester->pParticle = TestList;
	free(Tester);
	return 0;
}
