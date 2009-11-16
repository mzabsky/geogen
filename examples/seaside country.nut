function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Seaside country";
		case "description":
			return "This script creates a strip of of land locked between sea and high mountain range.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, 20000, 1);
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetParam("width");
	local height = GGen_GetParam("height");

	// set up height profile of the map
	local profile_height = GGen_Data_1D(10);
	
	profile_height.SetValue(0, -500);
	profile_height.SetValue(1, -250);
	profile_height.SetValue(2, -250);
	profile_height.SetValue(3, 0);
	profile_height.SetValue(4, 250);
	profile_height.SetValue(5, 250);
	profile_height.SetValue(6, 250);
	profile_height.SetValue(7, 500);
	profile_height.SetValue(8, 1500);
	profile_height.SetValue(9, 1500);
	
	profile_height.Smooth(2);
	
	// project the profile
	local base = GGen_Data_2D(width, height);
	base.Project(profile_height, GGEN_VERTICAL);
	
	// create the "wavy" profile along the vertical axis (creating the illusion of long valleys separated by hill ranges)
	local profile_shift = GGen_Data_1D(800);
	profile_shift.Noise(height / 15, height / 8);
	profile_shift.ScaleValuesTo(-width / 8, width / 8);
	
	base.Shift(profile_shift, GGEN_HORIZONTAL, GGEN_DISCARD_AND_FILL);
	
	// create the noise overlay
	local noise = GGen_Data_2D(width, height);
	noise.Noise(1, width > height ? height / 8 : width / 8, GGEN_STD_NOISE);
	
	noise.ScaleValuesTo(-1000, 1000);
	
	// make the noise be more significant in the higher mountainous part of the map than on the coast
	local mask = GGen_Data_2D(3, 2, 40);
	mask.SetValueInRect(2, 0, 2, 1, 154);
	
	base.AddMasked(noise, mask, false);
	
	return base;
}