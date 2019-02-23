ts1 = getTime();
run("Image Sequence...", "open=/local3/paper/slice4/test_06550_mon.tif sort");
ts2 = getTime();
print("load time (seconds): ", (ts2-ts1)/1000);
ts2 = getTime();
run("Export Current Image as XML/HDF5", "manual_mipmap_setup subsampling_factors=[{ {1,1,1}, {2,2,2}, {4,4,4} }] hdf5_chunk_sizes=[{ {256,256,4}, {256,256,2}, {256,256,1} }] value_range=[Use values specified below] min=0 max=2048 timepoints_per_partition=0 setups_per_partition=0 use_deflate_compression export_path=./bdv4slicesl3.xml");
ts3 = getTime();
print("conversion time (seconds): ", (ts3-ts2)/1000);
saveAs("Text", "/local3/test1slice4l3.txt");
close();
run("Quit")
