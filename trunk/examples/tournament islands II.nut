function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Tournament Islands II";
		case "description":
			return "Tournament style map emphasizing fair starting locations, for 4 to 36 players.";
		case "args":
			GGen_AddIntArg("size","Size","Width and size of the map.", 1024, 1000, GGen_GetMaxMapSize(), 1);
			GGen_AddEnumArg("smoothness","Smoothness","Affects amount of detail on the map.", 1, "Rough;Smooth;Very Smooth;Extra Smooth");
			GGen_AddEnumArg("feature_size","Feature Size","Affects size of individual hills/mountains.", 2, "Tiny;Medium;Large;Huge");
			GGen_AddEnumArg("grid_size","Grid Size","Number of rows and cols in the grid.", 1, "2;3;4;5;6");
						
			return 0;
	}
}

function Generate(){
	local size = GGen_GetArgValue("size");
	local smoothness = 1 << GGen_GetArgValue("smoothness");
	local feature_size = GGen_GetArgValue("feature_size");
	local grid = GGen_GetArgValue("grid_size") + 2;
	
	GGen_InitProgress(6);
	
	// first create one small tile
	local pattern = GGen_Data_2D(size / grid, size / grid, 0);
	
	local profile = GGen_Data_1D(2, 0);
	
	profile.SetValue(0, 10000); 
	profile.SetValue(1, -3000); 
	
	GGen_IncreaseProgress();
	
	pattern.RadialGradientFromProfile(size / (2 * grid), size / (2 * grid), size / (2 * grid), profile, true);

	GGen_IncreaseProgress();

	local noise = GGen_Data_2D(size / grid, size / grid, 0);
	noise.Noise(smoothness,  size / (32 - 4 * feature_size), GGEN_STD_NOISE);
	
	GGen_IncreaseProgress();
	
	noise.ScaleValuesTo(-11000, 11000);
	
	local mask = pattern.Clone();
	mask.Clamp(-4000, GGEN_MAX_HEIGHT);
	mask.ScaleValuesTo(0, GGEN_MAX_HEIGHT);

	pattern.AddMapMasked(noise, mask, false);
	
	GGen_IncreaseProgress();
	
	// ...then clone it all over the place
	local base = GGen_Data_2D(size, size, 0);
	
	base.Pattern(pattern);
	
	GGen_IncreaseProgress();
	
	base.TransformValues(GGEN_NATURAL_PROFILE, true);
	
	return base;

}