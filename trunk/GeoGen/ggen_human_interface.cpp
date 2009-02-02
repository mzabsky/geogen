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

void GGen_Data_1D::Print(){
	for(uint16 i = 0; i < length; i++) cout << /*i << ". :" << */data[i] << "\n";
}

void DrawPixel(SDL_Surface *screen, int x, int y, Uint8 R, Uint8 G, Uint8 B){

  Uint32 color = SDL_MapRGB(screen->format, R, G, B); 

  switch (screen->format->BytesPerPixel)   
  {   
    case 1:   
      {   
        Uint8 *bufp;   
        bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;   
        *bufp = color;   
      }   
      break;   
    case 2:   
      {   
        Uint16 *bufp;   
        bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;   
        *bufp = color;   
      }   
      break;   
    case 3:   
      {   
        Uint8 *bufp;   
        bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;   
        if(SDL_BYTEORDER == SDL_LIL_ENDIAN)   
        {   
          bufp[0] = color;   
          bufp[1] = color >> 8;   
          bufp[2] = color >> 16;   
        } else {   
          bufp[2] = color;   
          bufp[1] = color >> 8;   
          bufp[0] = color >> 16;   
        }   
      }   
      break;   
    case 4:   
      {   
        Uint32 *bufp;   
        bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
        *bufp = color;   
      }   
      break;   
  }   
}   

void GGen_Data_1D::Window(uint16 height){


	int16 min = Min();
	int16 max = Max();

	//stringstream label;

	//label << min << " " << max;

	int16 total = abs(min-max);

	double ratio = (double) height / (double) total;

	int16 offset = (int16) ((double) min * ratio);

	offset = -offset;

	SDL_Surface *screen;   
	screen = SDL_SetVideoMode(length, height+1, 32, SDL_HWSURFACE|SDL_DOUBLEBUF); 

	//SDL_WM_SetCaption(label.str, label.str);

	SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format, 0, 0, 0));


	SDL_LockSurface(screen);
	for(uint16 i = 0; i < length; i++) {
		DrawPixel(screen,i,(int) (data[i]*ratio + offset),255,255,255);
		//if(offset > 0) DrawPixel(screen,i,offset,125,125,125);
	}
	SDL_UnlockSurface(screen);
	SDL_Flip(screen);
}