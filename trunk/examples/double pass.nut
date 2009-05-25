function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Double Pass";
		case "description":
			return "Creates massive mountain range with three huge peaks separated by two passes.";
		case "min_width":
			return 512;
		case "min_height":
			return 512;
		case "max_width":
			return 99999999;
		case "max_height":
			return 99999999;
	}
}

function Generate(width, height){
	local base = GGen_Data_2D(width, height);
	
	// create one peak into the middle of the map
	base.RadialGradient(width / 2, height / 2, width > height ? height / 3 : width / 3, 1200, 0, true);

	// create a copy of the single peak
	local peak = GGen_Data_2D(base);

	// copy the peak to two more places
	base.IntersectionTo( - width / 3, 0, peak);
	base.IntersectionTo(   width / 3, 0, peak);

	base.Smooth((width > height ? height : width) / 10); 

	local mask = GGen_Data_2D(base);
	mask.Add(230);

	local noise = GGen_Data_2D(width, height);
	noise.Noise(2, width > height ? height / 8 : width / 8);
	
	noise.ScaleValuesTo(-300, 300);
	
	base.AddMasked(noise, mask, true);
	
	base.ScaleValuesTo(0,255);
	
	return base;

}