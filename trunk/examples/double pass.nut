function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Double Pass";
		case "description":
			return "Creates massive mountain range with three huge peaks separated by two passes.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, 20000, 1);
			GGen_AddIntArg("height","Height","Width of the map.", 1024, 128, 20000, 1);
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetParam("width");
	local height = GGen_GetParam("height");

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