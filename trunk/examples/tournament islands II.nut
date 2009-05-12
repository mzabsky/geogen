// Tournament style map emhasizing fair starting locations, for 16 players.

function Generate(width, height){
	local pattern = GGen_Data_2D(width / 4, height / 4);
	
	local profile = GGen_Data_1D(2);
	
	profile.SetValue(0, 100); 
	profile.SetValue(1, -130); 
	
	pattern.RadialGradient(width / 8, height / 8, (width > height ? height : width) / 8, profile, true);

	local noise = GGen_Data_2D(width / 4, height / 4);
	noise.Noise(1,  (width > height ? height : width) / 20);
	
	noise.ScaleValuesTo(-110, 110);
	
	pattern.Add(noise);
	
	local base = GGen_Data_2D(width, height);
	
	base.Pattern(pattern);
	
	base.Clamp(0, GGEN_MAX_HEIGHT);
	base.ScaleValuesTo(0,255);
	
	return base;

}