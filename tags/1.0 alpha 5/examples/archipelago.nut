function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Archipelago";
		case "description":
			return "Your map description.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, 20000, 1);
			
			GGen_AddEnumArg("smoothness","Smoothness","Affects amount of detail on the map.", 1, "Very Rough;Rough;Smooth;Very Smooth");
			GGen_AddEnumArg("archipelago_size","Archipelago Size","Size of clumps of islands.", 1, "Very Small;Small;Medium;Large;Very Large");
			GGen_AddEnumArg("island_size","Island Size","Size of individual islands.", 1, "Very Small;Small;Medium;Large;Very Large");
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetParam("width");
	local height = GGen_GetParam("height");
	local smoothness = 1 << GGen_GetParam("smoothness");
	local archipelago_size = GGen_GetParam("archipelago_size");
	local island_size = GGen_GetParam("island_size");

	// create a base archipelago layout
	local base = GGen_Data_2D(width, height, 0);

	base.Noise(width / 10, width / (8 - archipelago_size), GGEN_STD_NOISE);
	base.ScaleValuesTo(GGEN_MIN_HEIGHT / 2, GGEN_MAX_HEIGHT / 2);
	base.Clamp(GGEN_MIN_HEIGHT / 16, GGEN_MAX_HEIGHT / 16);
	base.Add(- 5 * GGEN_MAX_HEIGHT / 36);
	base.Smooth(width / 20);
	
	// add smaller islands on top of the base shape
	local islands = GGen_Data_2D(width, height, 0);
	
	islands.Noise(smoothness, width / (60 - 8 * island_size), GGEN_STD_NOISE);
	islands.ScaleValuesTo(0, GGEN_MAX_HEIGHT / 8);
	
	base.AddMap(islands);
	
	base.TransformValues(GGEN_NATURAL_PROFILE, true);

	return base;
}