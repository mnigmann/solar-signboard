import cv2
import numpy
import sys
import os.path

def bad_arguments():
    print("The first command line argument is the file name")
    print("The second command line argument specifies the scaling:")
    print("    scale-nearest=<n>       Scale the image by a factor of n")
    print("    scale-nearest=<w>x<h>   Scale the image so it fits inside a rectangle\n"
          "                            with width w and height h")
    print("    scale-integer=<w>x<h>   Scale the image by an integer amount so it fits\n"
          "                            inside a w by h rectangle")
    print("Optional command line arguments")
    print("    Specifies a crop area to be applied to the image before scaling it")
    print("        crop=<x0>,<y0>,<x1>,<y1>    Note that any of the values may be empty to\n"
          "                                    indicate that the crop area extends to the edge")
    print("    Create #define statements for every color")
    print("        name-colors")
    exit()

if len(sys.argv) < 3: bad_arguments()
if sys.argv[1] in "--help": bad_arguments()

i = cv2.imread(sys.argv[1])
crop = [x for x in sys.argv[1:] if x.startswith("crop")]
if crop:
    x0, y0, x1, y1 = crop[0][5:].split(",")
    x0 = int(x0.strip()) if x0.strip() else None
    y0 = int(y0.strip()) if y0.strip() else None
    x1 = int(x1.strip()) if x1.strip() else None
    y1 = int(y1.strip()) if y1.strip() else None
    i = i[y0:y1, x0:x1]

mode = sys.argv[2]
if mode.startswith("scale-nearest"):
    if "x" in mode:
        w, h = mode.split("=")[1].split("x")
        w, h = int(w), int(h)
        tw, th = w, w * i.shape[0] // i.shape[1]
        if th > h:
            tw, th = h * i.shape[1] // i.shape[0], h
        i = cv2.resize(i, (w, h), interpolation=cv2.INTER_NEAREST);
    else:
        n = float(mode.split("=")[1])
        i = cv2.resize(i, (int(i.shape[1] * n), int(i.shape[0] * n)), interpolation=cv2.INTER_NEAREST)
if mode.startswith("scale-integer"):
    w, h = mode.split("=")[1].split("x")
    w, h = int(w), int(h)
    sf = int(min(w/i.shape[1], h/i.shape[0]))
    i = cv2.resize(i, (i.shape[1] * sf, i.shape[0] * sf), interpolation=cv2.INTER_NEAREST)

fname = os.path.basename(os.path.splitext(sys.argv[1])[0])
print("#define IMAGE_{}_WIDTH {}".format(fname, i.shape[1]))
print("#define IMAGE_{}_HEIGHT {}".format(fname, i.shape[0]))
if "name-colors" in sys.argv[1:]:
    flat = i[:, :, ::-1].reshape((-1, 3))
    flat = flat >> 5
    colors = numpy.unique(flat, axis=0)
    flat = flat.tolist()
    colors = colors.tolist()
    for q, x in enumerate(colors):
        print("#define IMAGE_{}_COLOR{} {},{},{}".format(fname, q, *x))
    print("const uint8_t IMAGE_{}[] = {{\n    ".format(fname), end="")
    root = "IMAGE_{}_COLOR".format(fname)
    print(",\n    ".join(",".join(root+str(colors.index(k)) for k in flat[x:x+i.shape[1]]) for x in range(0, i.shape[1]*i.shape[0], i.shape[1])))
    print("\n};")

else:
    flat = i[:, :, ::-1].flatten().tolist()
    print("const uint8_t IMAGE_{}[] = {{\n    ".format(fname), end="")
    print(",\n    ".join(",".join(str(k>>5) for k in flat[3*x:3*(x+i.shape[1])]) for x in range(0, i.shape[1]*i.shape[0], i.shape[1])))
    print("\n};")
