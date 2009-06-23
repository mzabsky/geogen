function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Tournament Islands II";
		case "description":
			return "Tournament style map emhasizing fair starting locations, for 4-64 players.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddIntArg("num_players","Number of players","How many starting islands should be generated.", 16, 4, 64, 1);
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetParam("width");
	local height = GGen_GetParam("height"); 
	local num_players = GGen_GetParam("num_players"); 
	
	local grid = ceil(sqrt(num_players));

	local pattern = GGen_Data_2D(width / grid, height / grid);
	
	local profile = GGen_Data_1D(2);
	
	profile.SetValue(0, 100); 
	profile.SetValue(1, -130); 
	
	pattern.RadialGradient(width / (2 * grid), height / (2 * grid), (width > height ? height : width) / (2 * grid), profile, true);

	local noise = GGen_Data_2D(width / grid, height / grid);
	noise.Noise(1,  (width > height ? height : width) / 20);
	
	noise.ScaleValuesTo(-110, 110);
	
	pattern.Add(noise);
	
	local base = GGen_Data_2D(width, height);
	
	base.Pattern(pattern);
	
	base.Clamp(0, GGEN_MAX_HEIGHT);
	base.ScaleValuesTo(0,255);
	
	return base;

}