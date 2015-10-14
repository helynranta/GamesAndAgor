#include "server_functions.h"

int main(int argc, char const *argv[]){
	int i;
	Near *Tester;
	if (!(Tester = malloc(sizeof(Near))))
		perror("malloc");
	memset(Tester,0,sizeof(Near));
	free(Tester);


	Player *TestList = NULL, *p;
	for (i = 0; i < 10; i++) {
		if (!(p = malloc(sizeof(Player))))
			perror("malloc");
		memset(p, 0, sizeof(Player));
		append2ListPlayer(&TestList,p);
	}
	clearListPlayer(TestList);
	// Tester->pParticle = TestPlayer;
	return 0;
}
