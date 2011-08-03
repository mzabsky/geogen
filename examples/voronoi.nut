function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Voronoi";
		case "description":
			return "VornoiNoise usage demonstration (that actually gives guite nice results).";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, 20000, 1);
			
			GGen_AddEnumArg("peak_size","Peak Size","Size of individual peaks.", 2, "Tiny;Medium;Large;Huge");
			GGen_AddEnumArg("peak_prominence","Peak Prominence","Prominece of sharp peaks in comparison with sorrounding terrain.", 3, "Very Insignificant;Insignificant;Medium;Prominent;Very Prominent");
			GGen_AddEnumArg("peak_smoothness","Peak Smoothness","Smoothness of peak edges.", 1, "Very Sharp;Sharp;Smooth;Very Smooth");
	
			GGen_AddEnumArg("smoothness","Smoothness","Affects amount of detail on the map.", 1, "Very Rough;Rough;Smooth;Very Smooth");			
			GGen_AddEnumArg("feature_size","Terrain Feature Size","Affects size of individual terrain features (other than the sharp peaks).", 2, "Tiny;Medium;Large;Huge");
			
			GGen_AddEnumArg("water_level","Water Level","Determines how much of the map is covered by water.", 2, "Very Low;Low;Medium;High;Very High");
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetArgValue("width");
	local height = GGen_GetArgValue("height");

	local peak_size = GGen_GetArgValue("peak_size");
	local peak_prominence = GGen_GetArgValue("peak_prominence");
	local peak_smoothness = GGen_GetArgValue("peak_smoothness");

	local smoothness = 1 << GGen_GetArgValue("smoothness");
	local feature_size = GGen_GetArgValue("feature_size");
	
	local water_level = GGen_GetArgValue("water_level");
	
	local base = GGen_Data_2D(width, height, GGEN_NATURAL_PROFILE.Max());
	
	base.VoronoiNoise((-10 + (37 * (1 + peak_size))) * ((width > height) ? height : width) / 300, 2, GGEN_BUBBLES);

	base.Smooth(1 + 2 * (peak_smoothness) + 2 - peak_size / 2);
	
	local copy = base.Clone();
	local mask = base.Clone();
	
	mask.Clamp(mask.Max() / 6, 9 * mask.Max() / 10);
	
	mask.ScaleValuesTo(0, 4 * GGEN_MAX_HEIGHT / 5);

	copy.Smooth(width / 10);
	
	base.Combine(copy, mask, false);
	
	base.ScaleValuesTo(GGEN_MIN_HEIGHT / 6, GGEN_MAX_HEIGHT / 2);

	local noise = GGen_Data_2D(width, height, 0);
	
	noise.Noise(smoothness, ((width > height) ? height : width) / (60 - (15 * feature_size)) , GGEN_STD_NOISE);
	
	local prominence_fraction = 0.7 - 0.7 * (0.3 + (1 + peak_prominence) * 0.07);
	
	prominence_fraction /= (4 - feature_size);

	noise.ScaleValuesTo((-prominence_fraction * base.Max()).tointeger(), (prominence_fraction * base.Max()).tointeger());
	
	base.AddMap(noise);

	base.Add(GGEN_MAX_HEIGHT * (3 - water_level) / 15);
	
	return base;
}