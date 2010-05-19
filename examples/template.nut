function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Map Name";
		case "description":
			return "Your map description.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddIntArg("height","Height","Height of the map.", 1024, 128, 20000, 1);
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetArgValue("width");
	local height = GGen_GetArgValue("height");

	local base2 = GGen_Data_2D(width, height, 0);
	local base3 = GGen_Data_2D(width, height, 0);
	local base4 = GGen_Data_2D(width, height, 0);

	base2.Fill(5);

	base4.VoronoiNoise(20, 1, GGEN_BUBBLES);

	local base = GGen_Data_2D(width, height, 0);
	base.AddMap(base2);

	base4.Noise(10, 400, GGEN_STD_NOISE);

	local mask = base.Clone();

	base.Combine(base2, base3, true);

	return base;
}