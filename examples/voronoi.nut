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

	base.VoronoiNoise(90, 1, GGEN_BUBBLES);
	
	return base;
	
	//base.Smooth(width / 50);
	
	local copy = base.Clone();
	local mask = base.Clone();
	
	mask.Clamp(mask.Max() / 3, 9 * mask.Max() / 10);

	mask.ReturnAs("mask");

	copy.Smooth(width / 10);
	
	base.Combine(copy, mask, true);

	local noise = GGen_Data_2D(width, height, 0);
	
	noise.Noise(1, 400, GGEN_STD_NOISE);
	
	noise.ScaleValuesTo(-base.Max() / 3, base.Max() / 3);
	
	base.AddMap(noise);

	return base;
}