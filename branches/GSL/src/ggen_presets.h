/* GGEN_STD_NOISE */
Amplitudes* ggen_std_noise = new Amplitudes(4096);

ggen_std_noise->AddAmplitude(1, 3 * 15);
ggen_std_noise->AddAmplitude(2, 7 * 15);
ggen_std_noise->AddAmplitude(4, 10 * 15);
ggen_std_noise->AddAmplitude(8, 20 * 15);
ggen_std_noise->AddAmplitude(16, 50 * 15);
ggen_std_noise->AddAmplitude(32, 75 * 15);
ggen_std_noise->AddAmplitude(64, 150 * 15);
ggen_std_noise->AddAmplitude(128, 250 * 15);
ggen_std_noise->AddAmplitude(256, 400 * 15);
ggen_std_noise->AddAmplitude(512, 600 * 15);
ggen_std_noise->AddAmplitude(1024, 1000 * 15);
ggen_std_noise->AddAmplitude(2048, 1400 * 15);
ggen_std_noise->AddAmplitude(4096, 2000 * 15);

RegisterPreset(ggen_std_noise, StaticString("GGEN_STD_NOISE"));

/* GGEN_NATURAL_PROFILE */

Data_1D* ggen_natural_profile = new Data_1D(3, 0);
ggen_natural_profile->SetValue(1, 91 * 15);
ggen_natural_profile->SetValue(2, 255 * 15);

RegisterPreset(ggen_natural_profile, StaticString("GGEN_NATURAL_PROFILE"));
