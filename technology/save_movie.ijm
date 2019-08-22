
/*

save_movie.ijm
8/21/2019 Yang Yu, gnayuy@gmail.com

Example:

Fiji.app/ImageJ-linux64 --ij2 --headless --console -macro save_movie.ijm images/img-100000.tif,motionpreview10x.avi

*/

args = split(getArgument(), ",");
input = args[0];
output = args[1];
run("Image Sequence...", "open="+input+" sort");
run("AVI... ", "compression=JPEG frame=7 save="+output);

close();
run("Quit");
