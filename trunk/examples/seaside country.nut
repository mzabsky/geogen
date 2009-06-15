function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Seaside country";
		case "description":
			return "This script creates a strip of of land locked between sea and high mountain range.";
		case "min_width":
			return 128;
		case "min_height":
			return 128;
		case "max_width":
			return 99999999;
		case "max_height":
			return 99999999;
	}
}

function Generate(width, height){
  local profile_height = GGen_Data_1D(10);
	
	profile_height.SetValue(0, -500);
	profile_height.SetValue(1, -250);
	profile_height.SetValue(2, -250);
	profile_height.SetValue(3, 0);
	profile_height.SetValue(4, 250);
	profile_height.SetValue(5, 250);
	profile_height.SetValue(6, 250);
	profile_height.SetValue(7, 500);
	profile_height.SetValue(8, 1500);
	profile_height.SetValue(9, 1500);
	
	profile_height.Smooth(2);
	
	local base = GGen_Data_2D(width, height);
	base.Project(profile_height, GGEN_VERTICAL);;
	
	local profile_shift = GGen_Data_1D(800);
	profile_shift.Noise(1, height / 8);
	profile_shift.Smooth(height / 8);
	profile_shift.ScaleValuesTo(-width / 5, width / 8);
	
	base.Shift(profile_shift, GGEN_HORIZONTAL, GGEN_DISCARD_AND_FILL);
	
	local noise = GGen_Data_2D(width, height);
	noise.Noise(1, width > height ? height / 8 : width / 8);
	
	noise.ScaleValuesTo(-1000, 1000);
	
	
	local mask = GGen_Data_2D(3, 2, 40);
	mask.SetValueInRect(2, 0, 2, 1, 154);
	
	base.AddMasked(noise, mask, false);

	base.Clamp(0, GGEN_MAX_HEIGHT);
	base.ScaleValuesTo(0,255);
	
	return base;
}