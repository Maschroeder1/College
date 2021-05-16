from Arc import Arc
from Graph import Graph
import random
import time # REMOVER

def read_totals(input_file):
    line = input_file.readline()
    input_file.readline() # skipping that random empty line so next read is well positioned

    return [int(x) for x in line.split(' ')]


def create_empty_matrix(size):
    matrix = []
    for i in range(size):
        row = [None for x in range(size)]
        matrix.append(row)
    
    return matrix


def create_base_graph(input_file, total_vertexes):
    empty_matrix_with_all_vertexes = create_empty_matrix(total_vertexes)
    graph = Graph(empty_matrix_with_all_vertexes)

    populate_with(graph, input_file)
    
    return graph


def populate_with(graph, input_file):
    for line in input_file:
        [start, end, colour] = [int(x) for x in line.split(' ')]
        
        graph.add_arc(start, end, colour)


def create_initial_population(base_graph, total_vertexes, population_size):
    population = []
    i = 0
    while i < population_size:
        start = time.time()
        elem = Graph(create_empty_matrix(total_vertexes))
        base_graph.copy_arcs_to(elem)

        elem.spanning_treefy()
        
        i += 1
        print('created elem ' + str(i) + ' in ' + str(time.time() - start))
        population.append(elem)
    
    return population


def simulate_generations(population, base_graph, population_size, num_generations, total_vertexes, output_file):
    with open('./results/' + output_file + '.txt', 'w') as f:
        i = 0
        current_generation = population
        elite_percentage = 0.1
        mutate_percentage = 0.2

        total_elites = int(len(current_generation) * elite_percentage)
        total_mutations = int(len(current_generation) * mutate_percentage)
        total_crossovers = population_size - total_elites - total_mutations

        while i < num_generations:
            start = time.time()
            current_generation.sort(key=lambda x: x.num_colours())
            print()
            print('top of population ' + str(i) + ': ' + str(current_generation[0].num_colours()))
            f.write('top of population ' + str(i) + ': ' + str(current_generation[0].num_colours()))

            new_generation = []
            new_generation += current_generation[:total_elites]
            new_generation += mutations(current_generation, total_mutations, total_vertexes, base_graph)
            new_generation += crossover(current_generation, total_crossovers, total_vertexes)

            current_generation = new_generation
            i += 1
            print('time for generation: ' + str(time.time() - start))
            print(len(current_generation))
            f.write(' it took ' + str(time.time() - start) + ' to process it\n')
        
        current_generation.sort(key=lambda x: x.num_colours())
        f.write(str(current_generation[0]))
        print(current_generation[0].num_colours())


def mutations(population, num_mutations, total_vertexes, base_graph):
    mutated_graphs = []
    max_random_arcs = 5

    for i in range(0, num_mutations):
        elem = Graph(create_empty_matrix(total_vertexes))

        population[random.randint(0, len(population)-1)].copy_arcs_to(elem)
        for j in range(0, random.randint(1, max_random_arcs)):
            base_graph.flat_copy_random_arc_to(elem)

        elem.spanning_treefy()
        mutated_graphs.append(elem)

    return mutated_graphs


def crossover(population, num_crossovers, total_vertexes):
    new_elems = []
    num_tiers = 5
    
    tier_border_indexes = define_tier_borders(population, num_tiers)

    for i in range(0, num_crossovers):
        parent1 = select_parent(population, tier_border_indexes)
        parent2 = select_parent(population, tier_border_indexes)

        elem = Graph(create_empty_matrix(total_vertexes))
        parent1.copy_arcs_to(elem)
        parent2.copy_arcs_to(elem)
        elem.merge_parent_arcs()

        new_elems.append(elem)
    
    return new_elems


def define_tier_borders(population, num_tiers):
    top = population[0].num_colours()
    bottom = population[-1].num_colours()
    tier_separation = int((bottom - top) / num_tiers)
    tier_border_indexes = []
    
    i = 0
    current_tier = 1
    for pop in population:
        if pop.num_colours() > top + current_tier * tier_separation:
            current_tier += 1
            tier_border_indexes.append(i)

        i += 1
    
    return list(filter(lambda x: x < len(population), tier_border_indexes))


def select_parent(population, tier_border_indexes):
    parent_population_tier = select_parent_population_tier(population, tier_border_indexes)

    i = random.randint(0, len(parent_population_tier)-1)

    return parent_population_tier[i]


def select_parent_population_tier(population, tier_border_indexes):
    number_of_tiers = 1 if len(tier_border_indexes) == 0 else len(tier_border_indexes)
    pick_chance = 1 / number_of_tiers
    i = 0

    while True:
        if random.uniform(0, 1) < pick_chance:
            tier = i % number_of_tiers
            if tier == 0:
                return population if len(tier_border_indexes) == 0 else population[:tier_border_indexes[tier]]
            elif tier == number_of_tiers - 1:
                return population[tier_border_indexes[tier]:]
            else:
                return population[tier_border_indexes[tier]:tier_border_indexes[tier+1]]
        i += 1


def main():
    start = time.time()
    file_name = '100-990-125-7-4'
    seed = 1

    if file_name.find('.col') != -1:
        output_file_name = file_name[:-4] + '_seed_' + str(seed)
    else:
        output_file_name = file_name + '_seed_' + str(seed)
    population_size = 20
    num_generations = 200
    random.seed(seed)
    with open('./test_cases/' + file_name, 'r') as input_file:
        [total_vertexes, total_edges, total_labels] = read_totals(input_file)

        base_graph = create_base_graph(input_file, total_vertexes)
        population = create_initial_population(base_graph, total_vertexes, population_size)

        simulate_generations(population, base_graph, population_size, num_generations, total_vertexes, output_file_name)
    print(time.time() - start)


if __name__ == '__main__':
    main()