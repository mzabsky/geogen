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

/** 
 * @file ggen_progress.h Scripts might sometimes take a lot of time to process. Showing some sort of progress to the user may help to mitigate the "what is happening in the magic box?" effect. Progress in GeoGen is defined by two integers: the <em>max_progress</em> and the <em>current_progress</em>. The <em>max_progress</em> represents the total length of the "progress bar" while the <em>current_progress</em> represents its finished portion.
 **/

#pragma once

#include "ggen_support.h"
#include "ggen.h"
#include <assert.h>

/**
 * Sets the maximum progress value.
 * @param max_progress New maximum progress value.
 **/
void GGen_InitProgress(uint32 max_progress);

/**
 * Sets the current progress value.
 * @param current_progress New current progress value.
 * @note The GGen_InitProgress function must be called before calling GGen_SetProgress.
 **/
void GGen_SetProgress(uint32 current_progress);

/**
 * Increases the current progress value by 1.
 * @note The GGen_InitProgress function must be called before calling GGen_IncreaseProgress.
 **/
void GGen_IncreaseProgress();
