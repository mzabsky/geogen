function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Atoll";
		case "description":
			return "A ring shaped archipelago of small islands.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddEnumArg("feature_size","Island Size","Size of individual islands.", 1, "Tiny;Medium;Large");
			GGen_AddEnumArg("smoothness","Smoothness","Affects amount of detail on the map.", 2, "Very Rough;Rough;Smooth;Very Smooth");
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetParam("width");
	local height = GGen_GetParam("height");
	local feature_size = GGen_GetParam("feature_size");
	local smoothness = 1 << GGen_GetParam("smoothness");

	GGen_InitProgress(6);

	// we must decide the smaller dimension to fit the circle into the map
	local size = height > width ? width / 2 : height / 2;
  
	// set up radial profile of the archipelago
	local profile = GGen_Data_1D(70);
	
	profile.SetValueInRange(32, 54, 1250);
	profile.Smooth(8);
	
	GGen_IncreaseProgress();
	
	local base = GGen_Data_2D(width, height);
	base.RadialGradient(size, size, size, profile, true);
	
	GGen_IncreaseProgress();
	
	// noise overlay
	local noise = GGen_Data_2D(width, height);
	noise.Noise(smoothness, ((width > height) ? height : width) / (6 * (4 - feature_size)), GGEN_STD_NOISE);
	
	GGen_IncreaseProgress();
	
	noise.ScaleValuesTo(-500, 500);
	
	base.Add(noise);
	
	GGen_IncreaseProgress();
	
	base.Flood(0.05);

	GGen_IncreaseProgress();

	base.TransformValues(GGEN_NATURAL_PROFILE, true);
	
	return base;
}