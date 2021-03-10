original = "backpack.obj"
processed = "backpack - processed.obj"

STAHP = False
if original == processed:
    print ("Warning, overriding original. Comment next line if you wish to proceed.")
    STAHP = True

if not STAHP:
    fopen = open(original,"r")
    lines = fopen.readlines()
    fopen.close()

    fopen = open(processed,"w")
    for l in lines:
        t = l.split(" ")
        if len(t) >= 1 and t[0] == "vt":
            values = list(map(float, t[1:]))
            values[1] = 1 - values[1]
            fopen.write("vt %0.6f %0.6f\n" % (values[0], values[1]))
        else:
            fopen.write(l)
    fopen.close()
