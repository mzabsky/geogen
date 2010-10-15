function GetInfo(info_type){
	switch(info_type){
		case "name":
			return "Map Name";
		case "description":
			return "Your map description.";
		case "args":
			GGen_AddIntArg("width","Width","Width of the map.", 1024, 128, GGen_GetMaxMapSize(), 1);
			GGen_AddIntArg("height","Height","Height of the map.", 1024, 128, GGen_GetMaxMapSize(), 1);
			
			return 0;
	}
}

function Generate(){
	local width = GGen_GetArgValue("width");
	local height = GGen_GetArgValue("height");

	local base = GGen_Data_2D(width, height, 0);

	return base;
}