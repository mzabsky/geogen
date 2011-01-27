 /*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    GeoGen is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "ggen_progress.h"

void GGen_InitProgress(uint32 max_progress){
	// show a script assert if we are in wrong script mode, ordinary assert otherwise
	if(GGen::GetInstance()->GetStatus() == GGEN_LOADING_MAP_INFO) {GGen_Script_Assert(GGen::GetInstance()->GetStatus() == GGEN_GENERATING);}
	else assert(GGen::GetInstance()->GetStatus() == GGEN_GENERATING);

	GGen::GetInstance()->max_progress = max_progress;
	GGen::GetInstance()->current_progress = 0;

	if(GGen::GetInstance()->progress_callback != NULL) GGen::GetInstance()->progress_callback(0, max_progress);
}

void GGen_SetProgress(uint32 current_progress){
	// show a script assert if we are in wrong script mode, ordinary assert otherwise
	if(GGen::GetInstance()->GetStatus() == GGEN_LOADING_MAP_INFO) {GGen_Script_Assert(GGen::GetInstance()->GetStatus() == GGEN_GENERATING);}
	else assert(GGen::GetInstance()->GetStatus() == GGEN_GENERATING);

	GGen_Script_Assert(current_progress <= GGen::GetInstance()->max_progress);

	GGen::GetInstance()->current_progress = current_progress;

	if(GGen::GetInstance()->progress_callback != NULL) GGen::GetInstance()->progress_callback(current_progress, GGen::GetInstance()->max_progress);
}

void GGen_IncreaseProgress(){
	// show a script assert if we are in wrong script mode, ordinary assert otherwise
	if(GGen::GetInstance()->GetStatus() == GGEN_LOADING_MAP_INFO) {GGen_Script_Assert(GGen::GetInstance()->GetStatus() == GGEN_GENERATING);}
	else assert(GGen::GetInstance()->GetStatus() == GGEN_GENERATING);

	GGen_Script_Assert(GGen::GetInstance()->current_progress + 1 < GGen::GetInstance()->max_progress);

	GGen::GetInstance()->current_progress++;

	if(GGen::GetInstance()->progress_callback != NULL) GGen::GetInstance()->progress_callback(GGen::GetInstance()->current_progress, GGen::GetInstance()->max_progress);
}