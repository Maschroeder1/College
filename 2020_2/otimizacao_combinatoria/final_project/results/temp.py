base_file_name = '100-990-50-6-8_seed_'

times = []
intials = []
finals = []
for i in range(0, 10):
    with open(base_file_name + str(i) + '.txt', 'r') as f:
        file_time = 0
        for line in f:
            if line.find('top of population') != -1:
                temp1 = line.split(' ')
                file_time += float(temp1[7])
            
            if line.find('top of population 0:') != -1:
                temp2 = line.split(' ')
                intials.append(int(temp2[4]))

            if line.find('top of population 199:') != -1:
                temp3 = line.split(' ')
                finals.append(int(temp3[4]))
    
    times.append(file_time)
iter_times = 0
for x in times:
    iter_times += x
print('avg time: ' + str(iter_times / len(times)))
iter_init = 0
for x in intials:
    iter_init += float(x)
print('avg initials: ' + str(iter_init / len(intials)))
iter_fin = 0
for x in finals:
    iter_fin += float(x)
print('avg finals: ' + str(iter_fin / len(finals)))
print(finals)