function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Valley";
		case "description":
			return "A river valley running through hilly terrain to reach a sea shore.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, 20000, 1);
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetParam("width");
	local height = GGen_GetParam("height");

	local base = GGen_Data_2D(width, height);

	// create the shore->sea profile
	local profile_height = GGen_Data_1D(10);
	
	profile_height.SetValue(0, -800);
	profile_height.SetValue(1, -200);
	profile_height.SetValue(2, 100);
	profile_height.SetValue(3, 500);
	profile_height.SetValue(4, 1000);
	profile_height.SetValue(5, 1500);
	profile_height.SetValue(6, 1500);
	profile_height.SetValue(7, 1500);
	profile_height.SetValue(8, 1500);
	profile_height.SetValue(9, 1500);


	// assemble the valley profile from several parts
	local valley_profile_width = 100;

	local profile_valley = GGen_Data_1D(valley_profile_width, 1500);

	local valley_width = 3;

	profile_valley.SetValueInRange(valley_profile_width / 2 - valley_width, valley_profile_width / 2 + valley_width, 50);

	profile_valley.Gradient(valley_profile_width / 2 - valley_profile_width / 6, valley_profile_width / 2 - valley_width - 1, 300, 1500, false);
	profile_valley.Gradient(valley_profile_width / 2 + valley_width + 1, valley_profile_width / 2 + valley_profile_width / 6, 1500, 300, false);
	
	base.Project(profile_height, GGEN_VERTICAL);
	
	local valley = GGen_Data_2D(width, height);
	valley.Project(profile_valley, GGEN_HORIZONTAL);

	// combine the two profiles
	base.Intersection(valley);

	// create some meandres on the river using the shift with random noise profile
	local profile_shift = GGen_Data_1D(800);
	profile_shift.Noise(64, height / 8);
	profile_shift.ScaleValuesTo(-width / 11, width / 11);
	
	base.Shift(profile_shift, GGEN_VERTICAL, GGEN_DISCARD_AND_FILL);
	
	// keep the original valley for later use as mask
	valley.Shift(profile_shift, GGEN_VERTICAL, GGEN_DISCARD_AND_FILL);
	
	// meld the shapes a little bit
	base.Smooth(width > height ? height / 50 : width / 50);
	
	valley.Smooth(width > height ? height / 50 : width / 50);

	// noise overlay
	local noise = GGen_Data_2D(width, height);
	noise.Noise(4, width > height ? height / 6 : width / 6);
	
	noise.ScaleValuesTo(-1400, 1400);
	
	valley.ScaleValuesTo(20, 255);
	
	base.AddMasked(noise, valley, false);
	
	// balance the water level so the valley is just filled with water
	local max = 0;
	local now = 0;
	for(local i = 0; i < width; i++){
		now = base.GetValue(i, height / 2 + profile_shift.GetValue(i, width));
		if(now > max) max = now;
	}

	base.Add(-max);

	base.TransformValues(GGEN_STD_PROFILE, true);
	
	return base;
}