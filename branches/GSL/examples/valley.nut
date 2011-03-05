function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Valley";
		case "description":
			return "A river valley running through hilly terrain to reach a sea shore.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, GGen_GetMaxMapSize(), 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, GGen_GetMaxMapSize(), 1);
			GGen_AddEnumArg("valley_width","Valley width","Width of the central valley.", 2, "Very Narrow;Narrow;Moderate;Wide;Very Wide");
			GGen_AddEnumArg("smoothness","Smoothness","Affects amount of detail on the map.", 1, "Very Rough;Rough;Smooth;Very Smooth");
			GGen_AddEnumArg("feature_size","Feature Size","Affects size of individual hills/mountains.", 1, "Tiny;Medium;Large;Huge");
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetArgValue("width");
	local height = GGen_GetArgValue("height");
	local valley_width = GGen_GetArgValue("valley_width");
	local smoothness = 1 << GGen_GetArgValue("smoothness");
	local feature_size = GGen_GetArgValue("feature_size");

	GGen_InitProgress(9);

	local base = GGen_Data_2D(width, height, 0);

	// create the shore->sea profile
	local profile_height = GGen_Data_1D(10, 0);
	
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

	GGen_IncreaseProgress();

	// assemble the valley profile from several parts
	local valley_profile_width = 100;

	local profile_valley = GGen_Data_1D(valley_profile_width, 1500);

	local valley_width = 3 + valley_width;

	profile_valley.SetValueInRange(valley_profile_width / 2 - valley_width, valley_profile_width / 2 + valley_width, 50);

	profile_valley.Gradient(valley_profile_width / 2 - valley_profile_width / 6, valley_profile_width / 2 - valley_width - 1, 300, 1500, false);
	profile_valley.Gradient(valley_profile_width / 2 + valley_width + 1, valley_profile_width / 2 + valley_profile_width / 6, 1500, 300, false);
	
	base.Project(profile_height, GGEN_VERTICAL);
	
	GGen_IncreaseProgress();
	
	local valley = GGen_Data_2D(width, height, 0);
	valley.Project(profile_valley, GGEN_HORIZONTAL);

	// combine the two profiles
	base.Intersection(valley);

	GGen_IncreaseProgress();

	// create some meandres on the river using the shift with random noise profile
	local profile_shift = GGen_Data_1D(800, 0);
	profile_shift.Noise(64 < width / 10 ? 64 : width / 10, height / 8, GGEN_STD_NOISE);
	profile_shift.ScaleValuesTo(-width / 11, width / 11);
	
	base.Shift(profile_shift, GGEN_VERTICAL, GGEN_DISCARD_AND_FILL);
	
	// keep the original valley for later use as mask
	valley.Shift(profile_shift, GGEN_VERTICAL, GGEN_DISCARD_AND_FILL);
	
	GGen_IncreaseProgress();
	
	// meld the shapes a little bit
	base.Smooth(width > height ? height / 50 : width / 50);
	
	valley.Smooth(width > height ? height / 50 : width / 50);

	GGen_IncreaseProgress();

	// noise overlay
	local noise = GGen_Data_2D(width, height, 0);
	noise.Noise(smoothness, width > height ? height / (18 - 4 * feature_size) : width / (18 - 4 * feature_size), GGEN_STD_NOISE);
	
	GGen_IncreaseProgress();
	
	noise.ScaleValuesTo(-1400, 1400);
	
	valley.ScaleValuesTo(2 * GGEN_MAX_HEIGHT / 25, GGEN_MAX_HEIGHT);

	base.AddMapMasked(noise, valley, false);

	GGen_IncreaseProgress();
	
	
	local path = profile_shift.ToPath(width);
	path.Move(0, height / 2 - (valley_width - 2) * height / 100);
	
	local max = base.GetMaxValueOnPath(path);
	
	GGen_IncreaseProgress();

	base.Add(-max);

	base.TransformValues(GGEN_NATURAL_PROFILE, true);
	
	return base;
}