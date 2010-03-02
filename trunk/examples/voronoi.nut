function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Map Name";
		case "description":
			return "Your map description.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, 20000, 1);
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetParam("width");
	local height = GGen_GetParam("height");

	local base = GGen_Data_2D(width, height, 0);

	base.VoronoiNoise(width / 5, 1, GGEN_BUBBLES);
	
	base.ReturnAs("VoronoiNoise")
	
	base.Smooth(2);
	
	local copy = base.Clone();
	local mask = base.Clone();
	
	mask.Clamp(mask.Max() / 3, 9 * mask.Max() / 10);

	mask.ReturnAs("mask");
	
	mask.ScaleValuesTo(0, 200);

	copy.Smooth(width / 10);
	
	base.Combine(copy, mask, false);

	local noise = GGen_Data_2D(width, height, 0);
	
	noise.Noise(2, 400, GGEN_STD_NOISE);
	
	noise.ScaleValuesTo(- 2 * base.Max() / 3, 2 * base.Max() / 3);
	
	base.AddMap(noise);

	return base;
}