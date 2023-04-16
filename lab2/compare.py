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
                f3.write("\n")








