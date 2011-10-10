function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Highlands lake";
		case "description":
			return "Creates a lake in middle of hilly country.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1000, 10, GGen_GetMaxMapSize(), 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1000, 10, GGen_GetMaxMapSize(), 1);
			GGen_AddEnumArg("lake_size","Lake size","Radius of the central lake.", 2, "Tiny;Small;Medium;Large;Huge");
			GGen_AddEnumArg("smoothness","Smoothness","Affects amount of detail on the map.", 1, "Very Rough;Rough;Smooth;Very Smooth");
			GGen_AddEnumArg("feature_size","Feature Size","Affects size of individual hills/mountains.", 2, "Tiny;Medium;Large;Huge");
			return 0;
	}
}

function Generate(){
	// load map parameters
	local width = GGen_GetArgValue("width");
	local height = GGen_GetArgValue("height");
	local lake_size = GGen_GetArgValue("lake_size");
	local smoothness = 1 << GGen_GetArgValue("smoothness");
	local feature_size = GGen_GetArgValue("feature_size");

	// create a radial gradient with height 1 in the center and height 1200 on the outer rim
	local base = GGen_Data_2D(width, height, 0);
	base.RadialGradient(width / 2, height / 2, width > height ? height / 2 : width / 2, 1, 1200, true);

	// create a separate noise map
	local noise = GGen_Data_2D(width, height, 0);
	noise.Noise(smoothness, width > height ? height / (17 - 4 * feature_size) : width / (14 - 3 * feature_size), GGEN_STD_NOISE);

	// adjust the range of the noise
	noise.ScaleValuesTo(-500, 500);

	// combine the maps
	base.AddMap(noise);

	// raise the water level so 9% of the map is under level 0
	base.Flood(0.93 - 0.03 * lake_size);

	base.TransformValues(GGEN_NATURAL_PROFILE, true);

	return base;
}