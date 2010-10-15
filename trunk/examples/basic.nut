// Elementary script generating just a random noise with given parameters

function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Basic";
		case "description":
			return "Elementary script generating just a random noise with given parameters.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, GGen_GetMaxMapSize(), 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, GGen_GetMaxMapSize(), 1);
			GGen_AddEnumArg("water_distribution", "Water distribution", "Detrmines overall shape of terrain and water areas.", 0, "Uniform;Canals");
			GGen_AddEnumArg("smoothness","Smoothness","Affects amount of detail on the map.", 1, "Very Rough;Rough;Smooth;Very Smooth");
			GGen_AddEnumArg("feature_size","Feature Size","Affects size of individual hills/mountains.", 1, "Tiny;Medium;Large;Huge");
			GGen_AddIntArg("water_level","Water percentage","How much of the map should  be covered by sea.", 60, 5, 95, 5);
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetArgValue("width");
	local height = GGen_GetArgValue("height");
	local water_distribution = GGen_GetArgValue("water_distribution");
	local smoothness = 1 << GGen_GetArgValue("smoothness");
	local feature_size = GGen_GetArgValue("feature_size");
	local water_level = GGen_GetArgValue("water_level") / 100.;

	GGen_InitProgress(2);

	local base = GGen_Data_2D(width, height, 0);
	
	base.Noise(smoothness, ((width > height) ? height : width) / (5 * (4 - feature_size)), GGEN_STD_NOISE);
	
	if(water_distribution == 1) base.Abs();
	
	base.Flood(1 - water_level);
	
	GGen_IncreaseProgress();
	
	base.TransformValues(GGEN_NATURAL_PROFILE, true)
			
	return base;
}