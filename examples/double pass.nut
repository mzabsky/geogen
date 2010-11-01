function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Double pass";
		case "description":
			return "Two valley areas separated by a massive mountain range.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 10, GGen_GetMaxMapSize(), 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 10, GGen_GetMaxMapSize(), 1);
			GGen_AddEnumArg("smoothness","Smoothness","Affects amount of detail on the map.", 1, "Very Rough;Rough;Smooth;Very Smooth");
			return 0;
	}
}

function Generate(){
	local width = GGen_GetArgValue("width");
	local height = GGen_GetArgValue("height");
	local smoothness = 1 << GGen_GetArgValue("smoothness");

	local flipped = false;
	
	GGen_InitProgress(11);
	
	if(width < height){
		flipped = true;
		
		local temp = width;
		width = height;
		height = temp;
	}

	local base = GGen_Data_2D(width, height, 0);
	
	GGen_IncreaseProgress();
	
	// create one peak into the middle of the map
	local pass = GGen_Data_2D(height / 3, height, 0);
	
	pass.RadialGradient(1, height / 2, width > height ? height / 3 : width / 3, 1200, 0, true);

	pass.Gradient(1, 0, 1, height / 6, 800, 0, false);
	pass.Gradient(0, 5 * height / 6, 0, height - 1, 0, 800, false);

	GGen_IncreaseProgress();

	local copy = pass.Clone();
	copy.Flip(GGEN_VERTICAL);

	pass.Union(copy);
	
	copy = null;
	
	GGen_IncreaseProgress();
	
	local profile = pass.GetProfile(GGEN_VERTICAL, 0);

	base.Project(profile, GGEN_HORIZONTAL);
	
	profile = null;

	GGen_IncreaseProgress();

	base.IntersectionTo(pass, width / 3 - pass.GetWidth() / 2, 0);
	base.IntersectionTo(pass, 2 * width / 3 - pass.GetWidth() / 2, 0);
	
	pass = null;

	GGen_IncreaseProgress();

	base.Smooth((width > height ? height : width) / 10); 

	GGen_IncreaseProgress();

	local profile_shift = GGen_Data_1D(width, 0);
	profile_shift.Noise(width / 10, width / 2, GGEN_STD_NOISE);
	profile_shift.ScaleValuesTo(-height / 24, height / 24);

	base.Shift(profile_shift, GGEN_VERTICAL, GGEN_DISCARD_AND_FILL);
	
	base.Smooth(20);

	profile_shift = null;

	GGen_IncreaseProgress();

	local mask = base.Clone();
	mask.Clamp(200, GGEN_MAX_HEIGHT);	
	mask.ScaleValuesTo(0, GGEN_MAX_HEIGHT);
	mask.Clamp(GGEN_MAX_HEIGHT / 5, GGEN_MAX_HEIGHT);

	GGen_IncreaseProgress();

	local noise = GGen_Data_2D(width, height, 0);
	noise.Noise(smoothness, width > height ? height / 8 : width / 8, GGEN_STD_NOISE);
	
	GGen_IncreaseProgress();
	
	noise.ScaleValuesTo(-300, 300);
		
	base.AddMapMasked(noise, mask, false);
	
	base.Add(-base.Min() + 1);

	GGen_IncreaseProgress();

	if(flipped) base.Rotate(270, false);
	
	return base;
}