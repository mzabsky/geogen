function Generate(){
	local profile = GGen_Data_1D(2);
	
	profile.SetValue(1, 120); 
	
	local base = GGen_Data_2D(800, 800);
	base.RadialGradient(400, 400, 400, profile, true);

	local noise = GGen_Data_2D(800, 800, 29);
	noise.Noise(1, 64);
	
	noise.ScaleValuesTo(-50, 50);
	
	base.Add(noise);
	
	base.Flood(0.91);
	base.Clamp(0, 5000)
	base.ScaleValuesTo(0,255);
	
	return base;

}