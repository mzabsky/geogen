function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Atoll";
		case "description":
			return "A ring shaped archipelago of small islands.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, GGen_GetMaxMapSize(), 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, GGen_GetMaxMapSize(), 1);
			GGen_AddEnumArg("feature_size","Island Size","Size of individual islands.", 1, "Tiny;Medium;Large");
			GGen_AddEnumArg("smoothness","Smoothness","Affects amount of detail on the map.", 1, "Very Rough;Rough;Smooth;Very Smooth");
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetArgValue("width");
	local height = GGen_GetArgValue("height");
	local feature_size = GGen_GetArgValue("feature_size");
	local smoothness = 1 << GGen_GetArgValue("smoothness");

	GGen_InitProgress(6);

	// we must decide the smaller dimension to fit the circle into the map
	local size = height > width ? width / 2 : height / 2;
  
	// set up radial profile of the archipelago
	local profile = GGen_Data_1D(70, 0);
	
	profile.SetValueInRange(55, 61, 2 * GGEN_MAX_HEIGHT / 3);
	profile.Smooth(6);
	profile.Smooth(6);

	GGen_IncreaseProgress();
	
	local base = GGen_Data_2D(width, height, 0);
	base.RadialGradientFromProfile(size, size, size, profile, true);
	
	profile = null;
	
	GGen_IncreaseProgress();
	
	// noise overlay
	local noise = GGen_Data_2D(width, height, 0);
	noise.Noise(smoothness, ((width > height) ? height : width) / (60 - (15 * feature_size)), GGEN_STD_NOISE);
	
	GGen_IncreaseProgress();
	
	noise.ScaleValuesTo(-GGEN_MAX_HEIGHT / 3, GGEN_MAX_HEIGHT / 3);
	
	base.AddMapMasked(noise, base, true);
	
	GGen_IncreaseProgress();
	
	base.Flood(0.03);

	GGen_IncreaseProgress();

	base.TransformValues(GGEN_NATURAL_PROFILE, true);
	
	return base;
}