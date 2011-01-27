mkdir "temp/testOut"
mkdir "temp/testOut/archipelago"
mkdir "temp/testOut/atoll"
mkdir "temp/testOut/basic"
mkdir "temp/testOut/clutch"
mkdir "temp/testOut/double pass"
mkdir "temp/testOut/highlands lake"
mkdir "temp/testOut/seaside country"
mkdir "temp/testOut/template"
mkdir "temp/testOut/test"
mkdir "temp/testOut/tournament islands I"
mkdir "temp/testOut/tournament islands II"
mkdir "temp/testOut/valley"
mkdir "temp/testOut/voronoi"
cd bin
geogen -i "../examples/archipelago.nut" -o "../temp/testOut/archipelago.bmp" -d "../temp/testOut/archipelago" -Vv "../overlays/Topographic.bmp" %1 %2 %3 %4 %5
geogen -i "../examples/atoll.nut" -o "../temp/testOut/atoll.bmp" -d "../temp/testOut/atoll" -Vv "../overlays/Topographic.bmp" %1 %2 %3 %4 %5
geogen -i "../examples/basic.nut" -o "../temp/testOut/basic.bmp" -d "../temp/testOut/basic" -Vv "../overlays/Topographic.bmp" %1 %2 %3 %4 %5
geogen -i "../examples/clutch.nut" -o "../temp/testOut/clutch.bmp" -d "../temp/testOut/clutch" -Vv "../overlays/Topographic.bmp" %1 %2 %3 %4 %5
geogen -i "../examples/double pass.nut" -o "../temp/testOut/double pass.bmp" -d "../temp/testOut/double pass" -Vv "../overlays/Topographic.bmp" %1 %2 %3 %4 %5
geogen -i "../examples/highlands lake.nut" -o "../temp/testOut/highlands lake.bmp" -d "../temp/testOut/highlands lake" -Vv "../overlays/Topographic.bmp" %1 %2 %3 %4 %5
geogen -i "../examples/seaside country.nut" -o "../temp/testOut/seaside country.bmp" -d "../temp/testOut/seaside country" -Vv "../overlays/Topographic.bmp" %1 %2 %3 %4 %5
geogen -i "../examples/template.nut" -o "../temp/testOut/template.bmp" -d "../temp/testOut/template" -Vv "../overlays/Topographic.bmp" %1 %2 %3 %4 %5
geogen -i "../examples/test.nut" -o "../temp/testOut/test.bmp" -d "../temp/testOut/test" -Vv "../overlays/Topographic.bmp" %1 %2 %3 %4 %5
geogen -i "../examples/tournament islands I.nut" -o "../temp/testOut/tournament islands I.bmp" -d "../temp/testOut/tournament islands I" -Vv "../overlays/Topographic.bmp" %1 %2 %3 %4 %5
geogen -i "../examples/tournament islands II.nut" -o "../temp/testOut/tournament islands II.bmp" -d "../temp/testOut/tournament islands I" -Vv "../overlays/Topographic.bmp" %1 %2 %3 %4 %5
geogen -i "../examples/valley.nut" -o "../temp/testOut/valley.bmp" -d "../temp/testOut/valley" -Vv "../overlays/Topographic.bmp" %1 %2 %3 %4 %5
geogen -i "../examples/voronoi.nut" -o "../temp/testOut/voronoi.bmp" -d "../temp/testOut/voronoi" -Vv "../overlays/Topographic.bmp" %1 %2 %3 %4 %5
cd ..

