// system and SDL headers
#include <stdio.h>   
#include <stdlib.h> 
#include <string>
#include <SDL/SDL.h> 
#include <math.h>
#include <time.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

#include "ggen_support.h"
#include "ggen_data_1d.h"

int main(int argc, char *argv[]){ 
/*
	IMPORTANT: The code from this below is just example of usage
*/


	srand((unsigned)6); 

	uint16 octaves[8] = {10000, 5000, 3000, 2000, 1000,750,500,250};

	/*GGen_Data_1D* a = new GGen_Data_1D(30);
	a->Gradient(0,29,-10,20,true);
	a->Monochrome(0);
	a->Print();*/

	/*GGen_Data_1D b(10);
	b.Gradient(0,9,1,10,true);
	b.Shift(-4,GGEN_DISCARD_AND_FILL);
	b.Print();*/

	if( SDL_Init(SDL_INIT_VIDEO) < 0 ){
		printf("Inicializace SDL se nezdaøila: %s", SDL_GetError());   
		exit(5);   
	}   
	
	SDL_SetVideoMode(255, 255, 16, SDL_HWSURFACE|SDL_DOUBLEBUF);  

	GGen_Data_1D* test = new GGen_Data_1D(1000, 0);
	
	GGen_Data_1D* c = new GGen_Data_1D(550);

	//test->SetValueInRange(10, 20, 5);

	test->Noise(0,6, octaves);

	

	//test->ScaleTo(100, false);

	//test->Flip();
	//test->Normalize(GGEN_ADDITIVE);
	test->Smooth(5, 512);

	//test->Add(10000);

	//test->Normalize(GGEN_SUBSTRACTIVE);
	//test->AddTo(-10,c);
	
	//c->Gradient(0,512,test->Min(),test->Max(),true);

	//c->ResizeCanvas(50,0);

	//c->Flood(0.75);
	
	//test->Normalize(GGEN_SUBSTRACTIVE);

	test->Window(200);

	//test->Add(c);

	test->Print();

	test->Window(200);


  while(1){   
    SDL_Event event;   
    while(SDL_PollEvent(&event)){ 
	}
  }

	return 0;
}