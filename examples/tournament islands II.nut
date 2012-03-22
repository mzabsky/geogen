function GetInfo(info_type){
    switch(info_type){
        case "name":
            return "Tournament Islands II";
        case "description":
            return "Tournament style map emhasizing fair starting locations, for 16 players.";
        case "args":
            GGen_AddIntArg("width","Width","Width of the map.", 1024, 1000, 20000, 1);
            GGen_AddIntArg("height","Height","Width of the map.", 1024, 1000, 20000, 1);
            GGen_AddEnumArg("smoothness","Smoothness","Affects amount of detail on the map.", 1, "Rough;Smooth;Very Smooth;Extra Smooth");
            GGen_AddEnumArg("feature_size","Feature Size","Affects size of individual hills/mountains.", 2, "Tiny;Medium;Large;Huge");
                        
            return 0;
    }
}

function Generate(){
    local width = GGen_GetArgValue("width");
    local height = GGen_GetArgValue("height"); 
    local smoothness = 1 << GGen_GetArgValue("smoothness");
    local feature_size = GGen_GetArgValue("feature_size");
    
    local grid = 4;
    
    GGen_InitProgress(6);
    
    local pattern = GGen_Data_2D(width / grid, height / grid, 0);
    
    local profile = GGen_Data_1D(2, 0);
    
    profile.SetValue(0, 10000); 
    profile.SetValue(1, -13000); 
    
    GGen_IncreaseProgress();
    
    pattern.RadialGradientFromProfile(width / (2 * grid), height / (2 * grid), (width > height ? height : width) / (2 * grid), profile, true);

    GGen_IncreaseProgress();

    local noise = GGen_Data_2D(width / grid, height / grid, 0);
    noise.Noise(smoothness,  (width > height ? height : width) / (32 - 4 * feature_size), GGEN_STD_NOISE);
    
    GGen_IncreaseProgress();
    
    noise.ScaleValuesTo(-11000, 11000);
    
    pattern.AddMap(noise);
    
    GGen_IncreaseProgress();
    
    local base = GGen_Data_2D(width, height, 0);
    
    base.Pattern(pattern);
    
    GGen_IncreaseProgress();
    
    base.TransformValues(GGEN_NATURAL_PROFILE, true);
    
    return base;

}