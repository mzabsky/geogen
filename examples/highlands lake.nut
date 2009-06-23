function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Highlands lake";
		case "description":
			return "Creates a lake in middle of hilly country.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, 20000, 1);
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetParam("width");
	local height = GGen_GetParam("height");

	local profile = GGen_Data_1D(2);
	
	profile.SetValue(1, 1200); 
	
	local base = GGen_Data_2D(width, height);
	base.RadialGradient(width / 2, height / 2, width > height ? height / 2 : width / 2, profile, true);

	local noise = GGen_Data_2D(width, height);
	noise.Noise(2, width > height ? height / 8 : width / 8);
	
	noise.ScaleValuesTo(-500, 500);
	
	base.Add(noise);
	
	base.Flood(0.91);
	base.Clamp(0, GGEN_MAX_HEIGHT)
	base.ScaleValuesTo(0,200);
	
	return base;

}