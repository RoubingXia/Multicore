import os

with open("1stmillion.txt", "r") as f2:
    with open("expect", "w") as f3:
        line_num = 1
        for line2 in f2:
            cells = line2.split(" ");
            for cell in cells:
                if cell.strip() == "":
                    continue
                out_line = str(line_num) + " " + cell
                f3.write(out_line)
                line_num += 1
                f3.write("\n")
with open("expect", "r") as f1:
    with open("output", "r") as f2:
        for line1, line2 in zip(f1, f2):
            if (line1 != line2):
                print("Mismatch at {} vs {}".format(line1, line2))








