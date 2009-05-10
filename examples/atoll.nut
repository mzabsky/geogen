function Generate(width, height){
	local size = width / 2;
  
  local profile = GGen_Data_1D(35);
	
	profile.SetValueInRange(23, 27, 125);
	profile.SetValueInRange(0, 23, 35);
	profile.Smooth(5);
	profile.ScaleTo(size, false);
	
	local base = GGen_Data_2D(width, height);
	base.RadialGradient(size, size, size, profile, true);
	
	local noise = GGen_Data_2D(width, height, 29);
	noise.Noise(1, 64);
	
	noise.ScaleValuesTo(-50, 50);
	
	base.Add(noise);
	
	base.Flood(0.05);
	base.Clamp(0, GGEN_MAX_HEIGHT)
	base.ScaleValuesTo(0,255);
	
	return base;

}