function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Atoll";
		case "description":
			return "A ring shaped archipelago of small islands.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddEnumArg("circle_width","Circle Width","Width of the circle of the islands.", 1, "Narrow;Medium;Wide");
			GGen_AddEnumArg("feature_size","Island Size","Size of individual islands.", 1, "Tiny;Medium;Large");
			
			return 0;
	}
}
function Generate(){
	local width = GGen_GetParam("width");
	local height = GGen_GetParam("height");
	local circle_width = GGen_GetParam("circle_width");
	local feature_size = GGen_GetParam("feature_size");

	local size = height > width ? width / 2 : height / 2;
  
	local profile = GGen_Data_1D(70);
	
	profile.SetValueInRange(48 - 12 * circle_width, 54, 1250);
	//profile.SetValueInRange(0, 2, 350);
	profile.Smooth(8);
	profile.ScaleTo(size, false);
	
	local base = GGen_Data_2D(width, height);
	base.RadialGradient(size, size, size, profile, true);
	
	local noise = GGen_Data_2D(width, height);
	noise.Noise(1, ((width > height) ? height : width) / (6 * (4 - feature_size)));
	
	noise.ScaleValuesTo(-500, 500);
	
	base.Add(noise);
	
	base.Flood(0.03 + 0.02 * circle_width);
	base.Clamp(0, GGEN_MAX_HEIGHT)
	base.ScaleValuesTo(0,89);
	
	return base;

}