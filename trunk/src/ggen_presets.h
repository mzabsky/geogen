/* Presets are defined as locals, so there is no 

/* GGEN_STD_NOISE */

GGen_Amplitudes ggen_std_noise(4096);

ggen_std_noise.AddAmplitude(1, 3 * 15);
ggen_std_noise.AddAmplitude(2, 7 * 15);
ggen_std_noise.AddAmplitude(4, 10 * 15);
ggen_std_noise.AddAmplitude(8, 20 * 15);
ggen_std_noise.AddAmplitude(16, 50 * 15);
ggen_std_noise.AddAmplitude(32, 75 * 15);
ggen_std_noise.AddAmplitude(64, 150 * 15);
ggen_std_noise.AddAmplitude(128, 250 * 15);
ggen_std_noise.AddAmplitude(256, 400 * 15);
ggen_std_noise.AddAmplitude(512, 600 * 15);
ggen_std_noise.AddAmplitude(1024, 1000 * 15);
ggen_std_noise.AddAmplitude(2048, 1400 * 15);
ggen_std_noise.AddAmplitude(4096, 2000 * 15);

RegisterPreset(&ggen_std_noise, "GGEN_STD_NOISE");

/* GGEN_NATURAL_PROFILE */

GGen_Data_1D ggen_natural_profile(6);
ggen_natural_profile.SetValue(0, 1);
ggen_natural_profile.SetValue(1, 18 * 15);
ggen_natural_profile.SetValue(2, 43 * 15);
ggen_natural_profile.SetValue(3, 91 * 15);
ggen_natural_profile.SetValue(4, 163 * 15);
ggen_natural_profile.SetValue(5, 255 * 15);

RegisterPreset(&ggen_natural_profile, "GGEN_NATURAL_PROFILE");
