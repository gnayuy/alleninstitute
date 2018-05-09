
for i in *.v3draw;
do

mkdir /local2/IVSCC_tera/${i%*.v3draw}
time /local2/teraconverter -s="$i" -d="/local2/IVSCC_tera/${i%*.v3draw}" --halve=max --resolutions=01234 --width=256 --height=256 --depth=256 --sfmt="Vaa3D raw" --dfmt="TIFF (tiled, 3D)" --libtiff_rowsperstrip=-1;

done


