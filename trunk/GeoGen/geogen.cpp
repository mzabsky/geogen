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
#include "ggen_data_2d.h"

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

	/*GGen_Data_2D* test = new GGen_Data_2D(1000,1000,100);

	test->Gradient(500,400,600,100,255,120,true);

	GGen_Data_1D* pattern = new GGen_Data_1D(5);

	pattern->SetValue(0,0);
	pattern->SetValue(1,255);
	pattern->SetValue(2,125);
	pattern->SetValue(3,175);
	pattern->SetValue(4,0);

	test->RadialGradient(200,200,200,pattern,true);*/

	GGen_Data_1D* profile_a = new GGen_Data_1D(10, 127);
	profile_a->SetValueInRange(4, 5, 16);

	GGen_Data_1D* profile_b = new GGen_Data_1D(3, 127);
	profile_b->SetValue(0, 0);

	profile_a->Print();

	GGen_Data_2D* a = new GGen_Data_2D(512, 512);
	a->Project(profile_a, GGEN_HORIZONTAL);

	uint16 amplitudes[2] = {150, 40};

	GGen_Data_1D* shift_map = new GGen_Data_1D(512);
	shift_map->Noise(5, 6, amplitudes);
	shift_map->Smooth(3,500);
	shift_map->Print();

	//a->data[0] = 255;

	GGen_Data_2D* b = new GGen_Data_2D(512, 512);
	b->Project(profile_b, GGEN_VERTICAL);

	b->Intersection(a);

	//b->Window(true);
/*
	for(uint16 i = 0; i < b->y; i++){
		for(uint16 j = 0; j < b->x; j++){
			b->data[j + i * 255] = a->GetValue(j,i,255,255);
		}
	}*/

	//b->Window(true);

  while(1){   
    SDL_Event event;   
    while(SDL_PollEvent(&event)){ 
	}
  }

	return 0;
}