function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Atoll";
		case "description":
			return "A ring shaped archipelago of small islands.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, 20000, 1);
			
			return 0;
	}
}
function Generate(){
	local width = GGen_GetParam("width");
	local height = GGen_GetParam("height");

	local size = height > width ? width / 2 : height / 2;
  
	local profile = GGen_Data_1D(35);
	
	profile.SetValueInRange(23, 27, 1250);
	profile.SetValueInRange(0, 23, 350);
	profile.Smooth(5);
	profile.ScaleTo(size, false);
	
	local base = GGen_Data_2D(width, height);
	base.RadialGradient(size, size, size, profile, true);
	
	local noise = GGen_Data_2D(width, height);
	noise.Noise(2, height > width ? width / 16 : height / 16);
	
	noise.ScaleValuesTo(-500, 500);
	
	base.Add(noise);
	
	base.Flood(0.05);
	base.Clamp(0, GGEN_MAX_HEIGHT)
	base.ScaleValuesTo(0,89);
	
	return base;

}