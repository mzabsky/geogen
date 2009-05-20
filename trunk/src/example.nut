// This script creates a strip of of land locked between sea and high mountain range.

function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Seaside country";
		case "description":
			return "kghvfmhvfhmvmgh\nkfjhcmhcmcmhch\nhjgvhghg";
		case "max_width":
			return 1024;
		case "max_height":
			return 1024;
	}
}

function Generate(width, height){
  local profile_height = GGen_Data_1D(10);
	
	profile_height.SetValue(0, -500);
	profile_height.SetValue(1, -250);
	profile_height.SetValue(2, 0);
	profile_height.SetValue(3, 250);
	profile_height.SetValue(4, 250);
	profile_height.SetValue(5, 500);
	profile_height.SetValue(6, 500);
	profile_height.SetValue(7, 1000);
	profile_height.SetValue(8, 1500);
	profile_height.SetValue(9, 1500);
	
	profile_height.Smooth(2);
	
	local base = GGen_Data_2D(width, height);
	base.Project(profile_height, GGEN_VERTICAL);;
	
	local profile_shift = GGen_Data_1D(800);
	profile_shift.Noise(1, 128);
	profile_shift.Smooth(40);
	profile_shift.ScaleValuesTo(-width / 5, width / 5);
	
	base.Shift(profile_shift, GGEN_HORIZONTAL, GGEN_DISCARD_AND_FILL);
	
	local noise = GGen_Data_2D(width, height);
	noise.Noise(2, width > height ? height / 16 : width / 16);
	
	noise.ScaleValuesTo(-1000, 1000);
	
	noise.ReturnAs("noise");
	
	local profile_mask = GGen_Data_1D(3, 60);
	profile_mask.SetValue(2, 255);
	
	local mask = GGen_Data_2D(3, 2);
	mask.Project(profile_mask, GGEN_VERTICAL);
	
	base.AddMasked(noise, mask, false);

	base.Clamp(0, GGEN_MAX_HEIGHT);
	base.ScaleValuesTo(0,255);
	
	return base;
}