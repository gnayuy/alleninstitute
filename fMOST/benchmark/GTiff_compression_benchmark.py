__author__ = 'kersten.clauss'

"""Benchmark different GeoTiff compression algorithms.

Usage: GTiff_compression_benchmark.py some_geo.tif

Requires the GDAL tools to be present and executable from the command line of your system.

This script will take a GeoTiff as input and create copies with different compression algorithms from it.
It measures the filesize, compression and decompression times and returns them as a table.

Author:
Kersten Clauss
kersten.clauss@gmail.com
"""

# pip install hurry.filesize
# gdal ./configure --prefix=/usr --with-python=yes --with-libtiff=/usr

import os
import sys
import time
from hurry.filesize import size, si
#from osgeo import gdal
import pandas
import gdal


if __name__ == "__main__":
    if len(sys.argv) < 2:  # check if directory is given
        raise Exception("Missing input GeoTiff.")

    src_img = os.path.abspath(sys.argv[1])

    # create tmp dir
    tmp_dir = os.path.join(os.path.dirname(src_img), "tmp")
    if not os.path.exists(tmp_dir):
        os.makedirs(tmp_dir)

    # generate filenames
    input = os.path.join(tmp_dir, "input.tif")
    uncompressed = os.path.join(tmp_dir, "uncompressed.tif")
    deflate_1 = os.path.join(tmp_dir, "deflate_1.tif")
    deflate_2 = os.path.join(tmp_dir, "deflate_2.tif")
    lzw_1 = os.path.join(tmp_dir, "lzw_1.tif")
    lzw_2 = os.path.join(tmp_dir, "lzw_2.tif")
    packbits = os.path.join(tmp_dir, "packbits.tif")

    # make sure to have uncompressed input
    command = "gdal_translate -of GTiff " + src_img + " " + input
    print "## Make sure input is uncompressed ##"
    os.system(command)

    # generate commands
    command_uncompressed = "gdal_translate -of GTiff " + input + " " + uncompressed
    command_packbits = "gdal_translate -of GTiff -co \"COMPRESS=PACKBITS\" -co \"TILED=YES\" " + input + " " + packbits
    command_deflate_1 = "gdal_translate -of GTiff -co \"COMPRESS=DEFLATE\" -co \"PREDICTOR=1\" -co \"TILED=YES\" " + input + " " + deflate_1
    command_deflate_2 = "gdal_translate -of GTiff -co \"COMPRESS=DEFLATE\" -co \"PREDICTOR=2\" -co \"TILED=YES\" " + input + " " + deflate_2
    command_lzw_1 = "gdal_translate -of GTiff -co \"COMPRESS=LZW\" -co \"PREDICTOR=1\" -co \"TILED=YES\" " + input + " " + lzw_1
    command_lzw_2 = "gdal_translate -of GTiff -co \"COMPRESS=LZW\" -co \"PREDICTOR=2\" -co \"TILED=YES\" " + input + " " + lzw_2

    # execute and time compression
    print "## Execute and time compressions ##"
    start_time = time.time()
    os.system(command_uncompressed)
    write_uncompressed = time.time() - start_time

    start_time = time.time()
    os.system(command_packbits)
    write_packbits = time.time() - start_time

    start_time = time.time()
    os.system(command_deflate_1)
    write_deflate_1 = time.time() - start_time

    start_time = time.time()
    os.system(command_deflate_2)
    write_deflate_2 = time.time() - start_time

    start_time = time.time()
    os.system(command_lzw_1)
    write_lzw_1 = time.time() - start_time

    start_time = time.time()
    os.system(command_lzw_2)
    write_lzw_2 = time.time() - start_time

    # check filesizes
    size_uncompressed = size(os.path.getsize(uncompressed), system=si)
    size_packbits = size(os.path.getsize(packbits), system=si)
    size_deflate_1 = size(os.path.getsize(deflate_1), system=si)
    size_deflate_2 = size(os.path.getsize(deflate_2), system=si)
    size_lzw_1 = size(os.path.getsize(lzw_1), system=si)
    size_lzw_2 = size(os.path.getsize(lzw_2), system=si)

    # check read times
    def read_tif(img):
        return gdal.Open(img).ReadAsArray()

    print "## Execute and time decompressions ##"
    start_time = time.time()
    img = read_tif(uncompressed)
    img = None
    read_uncompressed = time.time() - start_time

    start_time = time.time()
    img = read_tif(packbits)
    img = None
    read_packbits = time.time() - start_time

    start_time = time.time()
    img = read_tif(deflate_1)
    img = None
    read_deflate_1 = time.time() - start_time

    start_time = time.time()
    img = read_tif(deflate_2)
    img = None
    read_deflate_2 = time.time() - start_time

    start_time = time.time()
    img = read_tif(lzw_1)
    img = None
    read_lzw_1 = time.time() - start_time

    start_time = time.time()
    img = read_tif(lzw_2)
    img = None
    read_lzw_2 = time.time() - start_time

    # remove tmp directory
    files = [input, uncompressed, packbits, deflate_1, deflate_2, lzw_1, lzw_2]
    #for file in files:
    #    os.remove(file)
    #os.removedirs(tmp_dir)

    # print results
    names = ["Uncompressed", "Packbits", "Deflate pred=1", "Deflate pred=2", "LZW pred=1", "LZW pred=2"]
    sizes = [size_uncompressed, size_packbits, size_deflate_1, size_deflate_2, size_lzw_1, size_lzw_2]
    writes = [write_uncompressed, write_packbits, write_deflate_1, write_deflate_2, write_lzw_1, write_lzw_2]
    reads = [read_uncompressed, read_packbits, read_deflate_1, read_deflate_2, read_lzw_1, read_lzw_2]

    print "## Benchmark results ##"
    print pandas.DataFrame([sizes, writes, reads], ["Size", "Write time", "Read time"], names)
