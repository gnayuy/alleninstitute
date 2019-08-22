
args = split(getArgument(), ",");
input = args[0];
output = args[1];
run("Image Sequence...", "open="+input+" sort");
run("AVI... ", "compression=JPEG frame=7 save="+output);

close();
run("Quit");
