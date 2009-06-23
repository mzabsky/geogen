// Elementary script generating just a random noise with given parameters

function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Basic";
		case "description":
			return "Elementary script generating just a random noise with given parameters.";
		case "min_width":
			return 64;
		case "min_height":
			return 64;
		case "max_width":
			return 99999999;
		case "max_height":
			return 99999999;
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, 2048, 1, false);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, 2048, 1, false);
			GGen_AddEnumArg("smoothness","Smoothness","Affects amount of detail on the map.", 2, "Very Rough;Rough;Smooth;Very Smooth");
			GGen_AddEnumArg("feature_size","Feature Size","Affects size of individual hills/mountains.", 2, "Tiny;Medium;Large;Huge");
			GGen_AddIntArg("water_level","Water percentage","How much of the map should  be covered by sea.", 60, 5, 95, 5, false);
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetParam("width");
	local height = GGen_GetParam("height");
	local smoothness = 1 << GGen_GetParam("smoothness");
	local feature_size = GGen_GetParam("feature_size");
	local water_level = GGen_GetParam("water_level") / 100.;

	local base = GGen_Data_2D(width, height);
	
	base.Noise(smoothness, ((width > height) ? height : width) / (5 * (4 - feature_size)));
	
	base.Flood(1 - water_level);
	base.Clamp(0, GGEN_MAX_HEIGHT);
	base.ScaleValuesTo(0,255);
	
	base.Scatter(false);
	
	base.ScaleValuesTo(0,255);
		
	return base;
}