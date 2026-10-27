from time import sleep
frames = 0
with open("bad_bad_apple.stl", "r") as f:
    for ind, line in enumerate(f.readlines()):
        if "Frame" in line: frames += 1
        # if not "MOVE" in line: continue
        if ind%200 != 0: continue
        print(f"{line[:-1]} \t at frame {frames}")
        sleep(0.01)
        