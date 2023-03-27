import os

base_command = "(time ./maxnum {} {} input10_{}) >  outlog 2>&1"

def convert_time_to_ms(time_str):
    minutes, seconds = time_str.strip().split('m')
    seconds = seconds.strip('s')
    total_seconds = float(minutes) * 60 + float(seconds)
    return total_seconds * 1000

def extract_time(filename):
    with open(filename, 'r') as f:
        # read the file and split it into lines
        lines = f.read().splitlines()

        # extract the real, user, and sys time from the lines
        real_time = convert_time_to_ms(lines[-3].split()[1])
        user_time = convert_time_to_ms(lines[-2].split()[1])
        sys_time = convert_time_to_ms(lines[-1].split()[1])

        # return the times as a dictionary
        return {'real': real_time, 'user': user_time, 'sys': sys_time}

def print_table(data):
    # define the headers and data for the table
    headers = ['N', '1,000', '10,000', '100,000', '1,000,000', '10,000,000', '100,000,000']
    # calculate the width of each column based on the longest value
    widths = [max(len(str(row[i])) for row in data + [headers]) for i in range(len(headers))]

    # print the headers
    print('|'.join(headers))

    # print the horizontal line separator
   # print('-' * sum(widths + [len(headers) * 3 - 1]))
    print('-' * (sum(widths) + len(headers) * 3 - 1))

    # print the data rows
    for row in data:
        print('|'.join(str(row[i]).ljust(widths[i]) for i in range(len(headers))))


threads_list = [0,1,4]
len_list = [1000, 10000, 100000, 1000000, 10000000, 100000000]
suffix = 3
# for table 1, real time
data1 = [
    ['Seq'],
    ['1-thread'],
    ['4-threads']
]
#for table 2, sys + cpu time
data2 = [
    ['Seq'],
    ['1-thread'],
    ['4-threads']
]
idx = 0

for thread_num in threads_list:
    # build row by row
    os.system("echo y | rm outlog")
    for len_num in len_list:
        # build cells in one row
        # remove the old output file
        os.system("echo y | rm outlog")
        command = base_command.format(thread_num, len_num, suffix)
        os.system(command)
        times = extract_time('outlog')
        data1[idx].append(str(times['real']) + "ms")
        data2[idx].append(str(times['user'] + times['sys']) + "ms")
    idx += 1
    suffix += 1


print("\n")
print_table(data1)
print("\n")
print_table(data2)


