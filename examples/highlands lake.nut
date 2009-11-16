function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Highlands lake";
		case "description":
			return "Creates a lake in middle of hilly country.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddEnumArg("lake_size","Lake size","Radius of the central lake.", 2, "Tiny;Small;Medium;Large;Huge");
			
			return 0;
	}
}

function Generate(){
	// load map parameters
	local width = GGen_GetParam("width");
	local height = GGen_GetParam("height");
	local lake_size = GGen_GetParam("lake_size");

	// create a radial gradient with height 1 in the center and height 1200 on the outer rim
	local base = GGen_Data_2D(width, height);
	base.RadialGradient(width / 2, height / 2, width > height ? height / 2 : width / 2, 1, 1200, true);

	// create a separate noise map
	local noise = GGen_Data_2D(width, height);
	noise.Noise(2, width > height ? height / 8 : width / 8, GGEN_STD_NOISE);

	// adjust the range of the noise
	noise.ScaleValuesTo(-500, 500);

	// combine the maps
	base.Add(noise);

	// raise the water level so 9% of the map is under level 0
	base.Flood(0.93 - 0.03 * lake_size);

	base.TransformValues(GGEN_STD_PROFILE, true);

	return base;
}