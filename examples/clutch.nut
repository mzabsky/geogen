function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "The Clutch";
		case "description":
			return "Two mountainous regions are connected only by a narrow land bridge.";
		case "args":
			GGen_AddIntArg("size","Size","Width and height of the map.", 1024, 128, GGen_GetMaxMapSize(), 1);
			GGen_AddEnumArg("smoothness","Smoothness","Affects amount of detail on the map.", 1, "Very Rough;Rough;Smooth;Very Smooth");
			GGen_AddEnumArg("feature_size","Feature Size","Affects size of individual hills/mountains.", 2, "Tiny;Medium;Large;Huge");
			return 0;
	}
}

function Generate(){
	local size = GGen_GetArgValue("size");
	local smoothness = 1 << GGen_GetArgValue("smoothness");
	local feature_size = GGen_GetArgValue("feature_size");
	
	GGen_InitProgress(9);
	
	local base = GGen_Data_2D(size, size, 0);
	
	local radial_profile = GGen_Data_1D(4, -12000);
	
	radial_profile.SetValue(2, -5000);
	radial_profile.SetValue(3, 0);
	
	// create the upper radial basin
	base.RadialGradientFromProfile(size / 4, size / 4, size / 2, radial_profile, true);

	GGen_IncreaseProgress();

	// create the land bridge
	local bridge = GGen_Data_2D(size, size, 0);
	bridge.Gradient(3* size / 7, 3 * size / 7, 45 * size / 100, 45 * size / 100, -12000, -6500, true);
	
	base.Union(bridge);
	
	// mirror the map along the axis going from bottom left corner to upper right corner
	local copy = base.Clone();
	copy.Flip(GGEN_HORIZONTAL);
	copy.Flip(GGEN_VERTICAL);
	
	base.Intersection(copy);
	
	GGen_IncreaseProgress();
	
	// make sure there is no land in the corners
	base.SetValueInRect(0, 0, size / 4, size / 4, -12000);
	base.SetValueInRect(3 * size / 4, 3 * size / 4, size - 1, size - 1, -12000);	
	
	base.ScaleValuesTo(2 * GGEN_MIN_HEIGHT / 8, GGEN_MIN_HEIGHT / 10);
	
	GGen_IncreaseProgress();
	
	base.Smooth(100);
	
	GGen_IncreaseProgress();
	
	local mask = base.Clone();
	mask.Clamp(-8000, GGEN_MAX_HEIGHT);
	
	mask.ScaleValuesTo(0,GGEN_MAX_HEIGHT / 2);
	mask.Add(GGEN_MAX_HEIGHT / 10);
	mask.Clamp(0, GGEN_MAX_HEIGHT / 2);
	mask.ScaleValuesTo(GGEN_MAX_HEIGHT / 30, GGEN_MAX_HEIGHT);

	GGen_IncreaseProgress();

	local noise = GGen_Data_2D(size, size, 0);
	noise.Noise(smoothness, size / (5 - feature_size), GGEN_STD_NOISE);
	
	GGen_IncreaseProgress();
	
	noise.ScaleValuesTo(0, 12000);

	base.AddMapMasked(noise, mask, false);

	GGen_IncreaseProgress();

	// find how much do we have to lower the water level to have as much water as possible while mantaining the land bridge
	local min;
	local max_min = GGEN_MIN_HEIGHT;
	
	// try 11 lines going along the axis of the bridge and find the most suitable one
	for(local j = - 5; j <= 5; j++){
		min = 0;
		
		for(local i = size / 4; i < 3 * size / 4; i++){
			if(base.GetValue(i + 5 * j, size - size * i / size) < min) min = base.GetValue(i + 5 * j, size - size * i / size);
		}
		
		if(max_min < min) max_min = min;
	}

	GGen_IncreaseProgress();

	base.Add(-max_min + 100);	
	
	base.TransformValues(GGEN_NATURAL_PROFILE, true);

	return base;
}