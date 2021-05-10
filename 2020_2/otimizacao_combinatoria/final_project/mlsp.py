from Arc import Arc
from Graph import Graph
import random

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


def main():
    random.seed(1)
    with open('./test_cases/complete_graph', 'r') as input_file:
        [total_vertexes, total_edges, total_labels] = read_totals(input_file)

        base_graph = create_base_graph(input_file, total_vertexes)
        print(base_graph.is_spanning_tree())

if __name__ == '__main__':
    main()