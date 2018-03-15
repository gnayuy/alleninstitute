# sh test2.sh slice256 conv1slice256

INDIR=$1
OUTDIR=$2

mkdir $OUTDIR

echo $OUTDIR

time biconverter -s="$INDIR" -d="$OUTDIR" --sfmt="TIFF (series, 2D)" --dfmt="TIFF (tiled, 3D)" --rescale=4 --resolutions=012 --width=256 --height=256 --depth=256 --halve=max --libtiff_rowsperstrip=-1


