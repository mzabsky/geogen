function Generate(width, height){
	local quarter = GGen_Data_2D(width / 2, height / 2);
	
	local profile = GGen_Data_1D(2);
	
	profile.SetValue(0, 120); 
	profile.SetValue(1, -80); 
	
	quarter.RadialGradient(width / 4, height / 4, (width > height ? height : width) / 4, profile, true);

	local noise = GGen_Data_2D(width / 2, height / 2);
	noise.Noise(1, 64);
	
	noise.ScaleValuesTo(-110, 110);
	
	quarter.Add(noise);
	
	local base = GGen_Data_2D(width, height);
	
	base.AddTo(0, 0, quarter);
	
	quarter.Flip(GGEN_VERTICAL);
	
	base.AddTo(0, height / 2, quarter);
	
	quarter.Flip(GGEN_HORIZONTAL);
	
	base.AddTo(width / 2, height / 2, quarter);
	
	quarter.Flip(GGEN_VERTICAL);
	
	base.AddTo(width / 2, 0, quarter);
	
	base.Clamp(0, GGEN_MAX_HEIGHT);
	base.ScaleValuesTo(0,255);
	
	return base;

}