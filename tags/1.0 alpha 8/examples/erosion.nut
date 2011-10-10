// Demonstrates effects of erosion filter

function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Erosion";
		case "description":
			return "Demonstrates effects of erosion filter.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 512, 128, GGen_GetMaxMapSize(), 1);
			GGen_AddIntArg("height","Height","Width of the map.", 512, 128, GGen_GetMaxMapSize(), 1);
			GGen_AddEnumArg("duration", "Erosion duration", "Deteremines intensity of the erosion effect (and directly determines how long will the map take tto generate).", 3, "Very Light;Light;Medium;Heavy;Very Heavy;Extreme");
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetArgValue("width");
	local height = GGen_GetArgValue("height");
	local duration = GGen_GetArgValue("duration");

	GGen_InitProgress(3);

	local base = GGen_Data_2D(width, height, 0);
	base.RadialGradient(width / 2, height / 2, 6 * width / 5, GGEN_MAX_HEIGHT / 2, GGEN_MIN_HEIGHT, true)
	base.Clamp(GGEN_MIN_HEIGHT / 3, GGEN_MAX_HEIGHT);
	base.Smooth(5);

	GGen_IncreaseProgress();

	local noise = GGen_Data_2D(width, height, 0);
	noise.Noise(2, 2 * width / 3, GGEN_STD_NOISE);	
	//base.Noise(smoothness, ((width > height) ? height : width) / (5 * (4 - feature_size)), GGEN_STD_NOISE);
	noise.ScaleValuesTo(GGEN_MIN_HEIGHT / 12, GGEN_MAX_HEIGHT / 10);
	noise.TransformValues(GGEN_NATURAL_PROFILE, true)	

	base.AddMap(noise);
	
	base.ReturnAs("base");
	
	GGen_IncreaseProgress();

	base.Erosion((1.0 + duration) * 0.7, 15.0, 1.0);	
	
	return base;
}