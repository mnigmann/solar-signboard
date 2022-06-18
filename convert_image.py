import cv2
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
    print("The third command line is optional and specifies a crop area to be applied\n"
          "to the image before scaling it")
    print("    crop=<x0>,<y0>,<x1>,<y1>    Note that any of the values may be empty to\n"
          "                                indicate that the crop area extends to the edge")
    exit()

if len(sys.argv) < 3: bad_arguments()
if sys.argv[1] in "--help": bad_arguments()

i = cv2.imread(sys.argv[1])
if len(sys.argv) == 4 and sys.argv[3].startswith("crop"):
    x0, y0, x1, y1 = sys.argv[3][5:].split(",")
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
flat = i.flatten().tolist()
print("const uint8_t IMAGE_{}[] = {{\n    ".format(fname), end="")
print(",\n    ".join(",".join(str(k>>5) for k in flat[3*x:3*(x+i.shape[1])]) for x in range(0, i.shape[1]*i.shape[0], i.shape[1])))
print("\n};")

