data_file = open("output.dat")
total_runs = 5
total_sizes = 4
data_points = []

for cores in range(1,9):
    for size in range(total_sizes):
        values = None
        tot_avg = 0
	init_avg = 0
	gen_avg = 0
	mult_avg= 0
        for runs in range(total_runs):
            line = data_file.readline()
            if len(line) > 0:
                values = line.split()
                values[0] = float(values[0])
                values[1] = float(values[1])
                values[2] = float(values[2])
                values[3] = float(values[3])
                values[4] = int(values[4])
                values[5] = int(values[5])
                tot_avg += values[0]
                init_avg += values[1]
                gen_avg += values[2]
                mult_avg += values[3]
        tot_avg /= total_runs
        init_avg /= total_runs
        gen_avg /= total_runs
        mult_avg /= total_runs
        data_string = "{} {} {} {} {} {}\n".format(tot_avg, init_avg,
            gen_avg, mult_avg, values[4], values[5])
        data_points.append(data_string)

output_file = open("average.dat", 'w')
for item in data_points:
    output_file.writelines(item)
