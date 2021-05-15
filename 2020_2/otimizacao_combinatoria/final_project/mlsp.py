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

def populate_with(graph, input_file):
    for line in input_file:
        [start, end, colour] = [int(x) for x in line.split(' ')]
        
        graph.add_arc(start, end, colour)


def create_base_graph(input_file, total_vertexes):
    empty_matrix_with_all_vertexes = create_empty_matrix(total_vertexes)
    graph = Graph(empty_matrix_with_all_vertexes)

    populate_with(graph, input_file)
    
    return graph


def remove_arc_from_cycle(graph):
    cycle = graph.locate_cycle()
    to_be_removed = random.randint(0, len(cycle)-1)

    # treat cycle as a circular list
    vertex1 = cycle[to_be_removed % len(cycle)]
    vertex2 = cycle[(to_be_removed+1) % len(cycle)]
    colours = graph.get(vertex1, vertex2)

    graph.remove_arc(vertex1, vertex2, colours[random.randint(0, len(colours)-1)])


def create_initial_population(base_graph, total_vertexes, population_size):
    population = []
    i = 0
    while i < population_size:
        elem = Graph(create_empty_matrix(total_vertexes))
        base_graph.copy_arcs_to(elem)

        elem.spanning_treefy()
        #while not elem.is_spanning_tree():
        #    remove_arc_from_cycle(elem)
        
        i += 1
        population.append(elem)
    
    for pop in population:
        print(pop)
        print()

# crossover = copy arcs from both parents to son graph, and only break cycles if the arc is NOT on both of them (is not duplicated)

# mutacao would be add N random arcs from the base graph, and break cycles WITHOUT caring if the arc is duplicated or not


def main():
    start = time.time()
    random.seed(1)
    with open('./test_cases/complete_graph', 'r') as input_file:
        [total_vertexes, total_edges, total_labels] = read_totals(input_file)

        base_graph = create_base_graph(input_file, total_vertexes)
        create_initial_population(base_graph, total_vertexes, 10)
    print(time.time() - start)


if __name__ == '__main__':
    main()