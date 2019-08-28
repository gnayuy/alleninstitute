#!/usr/bin/env python
#
# export_frames.py
#
# Copyright (c) Allen Institute for Brain Science. All rights reserved.
#

import argparse
import h5py
import numpy
import os
import subprocess


VIDEO_FRAME_RATE = 31.0  # / 8


def create_gray_ramp(maxVal, alpha=255):
    alpha = alpha << 24
    scale = 255.0 / float(max(1, maxVal))
    arr = numpy.zeros(65536, dtype=numpy.uint32)
    for i in xrange(int(maxVal)):
        val = min(int(i * scale), 255)
        arr[i] = (alpha | (val << 16) | (val << 8) | val)

    val = 255
    arr[int(maxVal):len(arr)] = (alpha | (val << 16) | (val << 8) | val)

    return arr


def get_stack_max_value(stack):
    max_vals = [numpy.max(stack[x]) for x in range(0, stack.shape[0], stack.shape[0] / 5)]
    # max_vals = [numpy.max(stack[x]) for x in range(0, stack.shape[0])]
    return numpy.max(max_vals)

def get_stack_99percentile_value(stack):
    vals = [numpy.percentile(stack[x], 99.94) for x in range(0, stack.shape[0], stack.shape[0] / 5)]
    # vals = [numpy.max(stack[x]) for x in range(0, stack.shape[0])]
    print numpy.mean(vals)
    return numpy.mean(vals)


def get_video_format(path):
    video_format = "mjpeg"

    fmt = os.path.splitext(path)
    if len(fmt) > 1:
        if fmt[1] == '.avi':
            video_format = 'mjpeg'
        elif fmt[1] == '.mp4':
            video_format = 'h264'
        else:
            raise Exception("Unsupported video format: %s" % fmt[1])

    return video_format


def open_video_coder(frame_shape, out_file):
    video_fmt = get_video_format(out_file)
    if video_fmt == "mjpeg":
        quality = 10
        codec_params = ["-vcodec", "mjpeg", "-qscale", str(quality)]
    else:
        quality = 26
        codec_params = ["-vcodec", "libx264", "-pix_fmt", "yuvj420p", "-crf", str(quality)]

    args = ["/shared/utils.x86_64/ffmpeg/bin/ffmpeg", "-r", str(VIDEO_FRAME_RATE), "-f", "rawvideo", "-pix_fmt", "rgba",
            "-s", "{0}x{1}".format(frame_shape[0], frame_shape[1]), "-i", "-"]
    args.extend(codec_params)
    args.extend(["-y", out_file])

    # print "Running {0}".format(" ".join(args))
    return subprocess.Popen(args, stdin=subprocess.PIPE)


def export_frames(stack_path, out_path, scale=1.0, maxval=0):
    stack = h5py.File(stack_path, "r")["data"]
    shape = [stack.shape[2], stack.shape[1]]

    if maxval != 0:
        colormap = create_gray_ramp(maxval)
    else:
        #colormap = create_gray_ramp(get_stack_max_value(stack) * scale)
        colormap = create_gray_ramp(get_stack_99percentile_value(stack) * scale)

    vid_proc = open_video_coder(shape, out_path)

    # for n in xrange(0, stack.shape[0], 4):
    # for n in xrange(100):
    for n in xrange(stack.shape[0]):
        frame = numpy.require(stack[n], numpy.uint16, 'F')
        frame = colormap[frame]

        vid_proc.stdin.write(frame.tostring())

    vid_proc.stdin.close()
    vid_proc.wait()


def export_dual_frames(stack_path1, stack_path2, out_path, scale=1.0, maxval=4095):
    stack1 = h5py.File(stack_path1, "r")["data"]
    stack2 = h5py.File(stack_path2, "r")["data"]

    size = stack2.shape
    shape = [size[2], size[1] * 2]

    if maxval == 0:
        colormap = create_gray_ramp(4095)
    else:
        colormap = create_gray_ramp(get_stack_max_value(stack2) * scale)

    vid_proc = open_video_coder(shape, out_path)

    # for n in xrange(0, stack.shape[0], 4):
    # for n in xrange(100):
    for n in xrange(stack2.shape[0]):
        frame1 = numpy.require(stack1[n], numpy.uint8, 'F')
        frame2 = numpy.require(stack2[n], numpy.uint8, 'F')
        frame = numpy.vstack((frame1, frame2))
        frame = colormap[frame]

        vid_proc.stdin.write(frame.tostring())

    vid_proc.stdin.close()
    vid_proc.wait()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Export an H5 file's z slices to the ffmpeg video encoder",
                                     formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('inputPath', help='input file')
    parser.add_argument('outputPath', nargs="?", help='output file name')
    parser.add_argument('--inputPath2', '-j', help='a second input file to display beneath the first file')
    parser.add_argument('--scale', '-s', default=1.0, help='scale the grayscale ramp by this value (e.g., use 0.5 to make the video twice as bright)')
    parser.add_argument('--maxval', '-m', default=0, help='set maximum value to use when convert 16 to 8-bit')

    args = parser.parse_args()

    if not args.outputPath:
        args.outputPath = os.path.splitext(args.inputPath)[0] + ".avi"
    
    scale = float(args.scale)
    maxval = float(args.maxval)

    if args.inputPath2:
        export_dual_frames(args.inputPath, args.inputPath2, args.outputPath, scale, maxval)
    else:
        export_frames(args.inputPath, args.outputPath, scale, maxval)
