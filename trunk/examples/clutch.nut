function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "The Clutch";
		case "description":
			return "Two mountainous regions are connected only by a narrow land bridge.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddEnumArg("smoothness","Smoothness","Affects amount of detail on the map.", 2, "Very Rough;Rough;Smooth;Very Smooth");
			GGen_AddEnumArg("feature_size","Feature Size","Affects size of individual hills/mountains.", 2, "Tiny;Medium;Large;Huge");
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetParam("width");
	local height = GGen_GetParam("height");
	local smoothness = 1 << GGen_GetParam("smoothness");
	local feature_size = GGen_GetParam("feature_size");

	// we need radial gradients, so we have to use square variant of the map for some time and squish it later
	local dimension = width > height ? width : height; 

	local base = GGen_Data_2D(dimension, dimension);
	
	local radial_profile = GGen_Data_1D(4, -12000);
	
	radial_profile.SetValue(2, -5000);
	radial_profile.SetValue(3, 0);
	
	// create the upper radial basin
	base.RadialGradient(dimension / 4, dimension / 4, dimension / 2, radial_profile, true);

	// create the land bridge
	local bridge = GGen_Data_2D(dimension, dimension);
	bridge.Gradient(3* dimension / 7, 3 * dimension / 7, 45 * dimension / 100, 45 * dimension / 100, -12000, -6500, true);
	
	base.Union(bridge);
	
	// mirror the map along the axis going from bottom left corner to upper right corner
	local copy = GGen_Data_2D(base);
	copy.Flip(GGEN_HORIZONTAL);
	copy.Flip(GGEN_VERTICAL);
	
	base.Intersection(copy);
	
	base.ScaleTo(width, height, false);
	base.Smooth(100);
	
	local mask = GGen_Data_2D(base);
	mask.Clamp(-8000, GGEN_MAX_HEIGHT);
	
	mask.ScaleValuesTo(0,255);
	mask.Add(100);
	mask.Clamp(0, 255);

	local noise = GGen_Data_2D(width, height);
	noise.Noise(smoothness, dimension / (5 - feature_size));
	noise.ScaleValuesTo(-2000, 12000);
	
	base.AddMasked(noise, mask, false);

	// find how much do we have to lower the water level to have as much water as possible while mantaining the land bridge
	local min;
	local max_min = GGEN_MIN_HEIGHT;
	
	// try 11 lines going along the axis of the bridge and find the most suitable one
	for(local j = - 5; j <= 5; j++){
		min = 0;
		
		for(local i = dimension / 4; i < 3 * dimension / 4; i++){
			if(width > height){
				if(base.GetValue(i + 5 * j, width - height * i / width) < min) min = base.GetValue(i + 5 * j, width - height * i / width);
			}
			else{
				if(base.GetValue(i + 5 * j, width - width * i / height) < min) min = base.GetValue(i + 5 * j, width - width * i / height);
			}
		}
		
		if(max_min < min) max_min = min;
	}

	base.Add(-max_min + 10);
	
	base.SetValueInRect(0, 0, width / 4, height / 4, 0);
	base.SetValueInRect(3 * width / 4, 3 * width / 4, width - 1, height - 1, 0);

	return base;

}