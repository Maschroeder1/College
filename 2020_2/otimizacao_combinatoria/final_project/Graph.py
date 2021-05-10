from Arc import Arc
import random
import Utils

class Graph:
    def __init__(self, empty_instance):
        self._instance = empty_instance
        self._num_arcs = 0
        self._num_vertexes = len(empty_instance)


    def add_arc(self, vertex1, vertex2, colour):
        self._num_arcs += 1
        if vertex1 > vertex2:
            if self._instance[vertex2-1][vertex1-1] == None:
                self._instance[vertex2-1][vertex1-1] = []

            self._instance[vertex2-1][vertex1-1].append(Arc(colour))
        else:
            if self._instance[vertex1-1][vertex2-1] == None:
                self._instance[vertex1-1][vertex2-1] = []

            self._instance[vertex1-1][vertex2-1].append(Arc(colour))


    def get(self, vertex1, vertex2):
        if vertex1 > vertex2:
            return self._instance[vertex2-1][vertex1-1]
        else:
            return self._instance[vertex1-1][vertex2-1]


    def neighbours(self, vertex):
        neighbours = []

        for i in range(0, len(self._instance)):
            if self.get(i+1, vertex) != None:
                neighbours.append(i+1)
        
        return neighbours


    def is_spanning_tree(self):
        # a graph is a spanning tree if it has no loops and if you can reach all vertexes by visiting any vertex's neighbours
        visited_vertexes = []
        vertex_queue = []
        
        # doing a simple bfs where each element is [vertex, father]
        vertex_queue.append([random.randint(0, self._num_vertexes-1), -1])
        while len(vertex_queue) > 0:
            current_vertex = vertex_queue[0]
            vertex_queue.pop(0)
            neighbours = self.neighbours(current_vertex[0])
            
            if current_vertex[1] != -1:
                neighbours.remove(current_vertex[1])

            if _has_loop(visited_vertexes, neighbours):
                return False
            
            visited_vertexes.append(current_vertex[0])
            vertex_queue += [[x, current_vertex[0]] for x in neighbours]

        return len(visited_vertexes) == self._num_vertexes


    def __repr__(self):
        return self.__str__()


    def __str__(self):
        stringfied = ''
        for line in self._instance:
            stringfied += '['
            for row in line:
                stringfied += str(row) + ', '
            stringfied = stringfied[:-2] + ']\n'
        
        return stringfied


def _has_loop(visited_vertexes, neighbours):
    return Utils.has_repeated(visited_vertexes, neighbours)