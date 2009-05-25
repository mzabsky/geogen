function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Highlands lake";
		case "description":
			return "Creates a lake in middle of hilly country.";
		case "min_width":
			return 128;
		case "min_height":
			return 128;
		case "max_width":
			return 99999999;
		case "max_height":
			return 99999999;
	}
}

function Generate(width, height){
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