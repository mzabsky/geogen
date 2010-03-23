function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Voronoi";
		case "description":
			return "VornoiNoise usage demonstration (that actually gives guite nice results).";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, 20000, 1);
			
			GGen_AddEnumArg("peak_size","Peak Size","Size of individual peaks.", 1, "Tiny;Medium;Large;Huge");
			GGen_AddEnumArg("peak_prominence","Peak Prominence","Prominece of sharp peaks in comparsion with sorrounding terrain.", 2, "Very Insignificant;Insignificant;Medium;Prominent;Very Prominent");
			GGen_AddEnumArg("peak_smoothness","Peak Smoothness","Smoothness of peak edges.", 1, "Very Sharp;Sharp;Smooth;Very Smooth");
	
			GGen_AddEnumArg("smoothness","Smoothness","Affects amount of detail on the map.", 1, "Very Rough;Rough;Smooth;Very Smooth");			
			GGen_AddEnumArg("feature_size","Terrain Feature Size","Affects size of individual terrain features (other than the sharp peaks).", 1, "Tiny;Medium;Large;Huge");
			GGen_AddIntArg("water_level","Water percentage","How much of the map should  be covered by sea.", 30, 5, 95, 5);		
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetParam("width");
	local height = GGen_GetParam("height");

	local peak_size = GGen_GetParam("peak_size");
	local peak_prominence = GGen_GetParam("peak_prominence");
	local peak_smoothness = GGen_GetParam("peak_smoothness");

	local smoothness = 1 << GGen_GetParam("smoothness");
	local feature_size = GGen_GetParam("feature_size");
	local water_level = GGen_GetParam("water_level") / 100.;
	
	local base = GGen_Data_2D(width, height, 0);
	
	base.VoronoiNoise((-10 + (30 * (1 + peak_size))) * ((width > height) ? height : width) / 300, 1, GGEN_BUBBLES);

	base.Smooth(1 + 2 * peak_smoothness);
	
	local copy = base.Clone();
	local mask = base.Clone();
	
	mask.Clamp(mask.Max() / 3, 9 * mask.Max() / 10);

	mask.ReturnAs("mask");
	
	mask.ScaleValuesTo(0, 200);

	copy.Smooth(width / 10);
	
	base.Combine(copy, mask, false);

	local noise = GGen_Data_2D(width, height, 0);
	
	noise.Noise(2, 400, GGEN_STD_NOISE);
	
	local prominence_fraction = 1 - (0.3 + (1 + peak_prominence) * 0.07);

	noise.ScaleValuesTo((-prominence_fraction * base.Max()).tointeger(), (prominence_fraction * base.Max()).tointeger());
	
	base.AddMap(noise);

	base.Flood(1 - water_level);

	return base;
}