function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Test";
		case "description":
			return "Your map description.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, GGen_GetMaxMapSize(), 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, GGen_GetMaxMapSize(), 1);
			
			return 0;
	}
}

function Generate(){
	GGen_InitProgress(21);

	local width = GGen_GetArgValue("width");
	local height = GGen_GetArgValue("height");

	local value_multiplier = 10;

	local base = GGen_Data_2D(width, height, 0);

	GGen_IncreaseProgress();

	//base.Gradient(width - 11, height - 11, 10, 10, GGEN_MAX_HEIGHT, GGEN_MIN_HEIGHT, true);
	base.Gradient(width / 8, height / 8, 7 * width / 8, 7 * height / 8, GGEN_MIN_HEIGHT, GGEN_MAX_HEIGHT, true);

	GGen_IncreaseProgress();

	local radial = GGen_Data_2D(width, height, GGEN_MIN_HEIGHT);
	radial.RadialGradient(3 * width / 4, height / 4, width / 5, 0, GGEN_MAX_HEIGHT, false);

	GGen_IncreaseProgress();

	base.Union(radial);
	
	base.UnionTo(radial, 0, height / 2);

	GGen_IncreaseProgress();
	
	radial.ResizeCanvas(width, height, width / 2, 0);
	radial.ReplaceValue(0, GGEN_MIN_HEIGHT);
	radial.Invert();

	GGen_IncreaseProgress();

	base.Intersection(radial);
	base.IntersectionTo(radial, 0, height / 2);

	base.ReturnAs("1-gradient, set operations, canvas, replaceValue");
	radial = null;

	
	GGen_IncreaseProgress();

	base.ScaleValuesTo(100, 110);

	base.ScaleValuesTo(GGEN_MIN_HEIGHT, GGEN_MAX_HEIGHT);

	GGen_IncreaseProgress();

	base.Smooth(width / 100);

	base.ReturnAs("2-scaleValues, smooth")
	GGen_IncreaseProgress();

	base.Rotate(-45, true);

	GGen_IncreaseProgress();

	local profile = GGen_Data_1D(width, 0);
	
	profile.Noise(4, width / 5, GGEN_STD_NOISE);
	profile.ScaleValuesTo(0, height / 10);
	
	base.Shift(profile, GGEN_VERTICAL, GGEN_DISCARD_AND_FILL);

	profile = null;

	base.ReturnAs("3-rotate, 1D noise, 1D scale, shift");
	GGen_IncreaseProgress();
	
	local mask = base.Clone();
	mask.SelectValue(0);
	
	GGen_IncreaseProgress();
	
	local noise = GGen_Data_2D(width, height, 0);	
	
	GGen_IncreaseProgress();
	
	noise.Noise(4, width / 8, GGEN_STD_NOISE);
	
	GGen_IncreaseProgress();
	
	mask.ReturnAs("mask");
	
	noise.ScaleValuesTo(GGEN_MIN_HEIGHT, GGEN_MAX_HEIGHT);

	base.AddMapMasked(noise, mask, true);

	mask = noise = null;
	
	base.ReturnAs("4-mask, monochrome, noise");
	GGen_IncreaseProgress();

	base.ScaleTo(width / 2, height / 2, false);

	GGen_IncreaseProgress();

	local new_base = GGen_Data_2D(width, height, 0);
	
	new_base.Pattern(base);

	GGen_IncreaseProgress();

	base = new_base;

	base.ScaleTo(width, height / 2, false);
	
	local base2 = base.Clone();
	
	base.ResizeCanvas(width, height, 0, 0);
	
	GGen_IncreaseProgress();
	
	base2.Flip(GGEN_VERTICAL);
	
	base.AddTo(base2, 0, height / 2);

	base2 = null;

	base.ReturnAs("5-scaleTo, pattern, flip, addto");
	GGen_IncreaseProgress();
	
	base.Flood(0.7);
	
	GGen_IncreaseProgress();
	
	base.Clamp(0, 3 * base.Max() / 4);
	
	base.ReturnAs("6-clamp, max, flood");
	
	GGen_IncreaseProgress();

	base.Shear(0, 1, true);
	
	return base;
}