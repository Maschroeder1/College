from Arc import Arc
import random
import Utils

class Graph:
    def __init__(self, empty_instance):
        self._instance = empty_instance
        self._num_arcs = 0
        self._colours = []
        self._num_vertexes = len(empty_instance)


    def add_arc(self, vertex1, vertex2, colour):
        self._num_arcs += 1
        if colour not in self._colours:
            self._colours.append(colour)

        if vertex1 > vertex2:
            if self._instance[vertex2-1][vertex1-1] == None:
                self._instance[vertex2-1][vertex1-1] = []

            self._instance[vertex2-1][vertex1-1].append(Arc(colour))
        else:
            if self._instance[vertex1-1][vertex2-1] == None:
                self._instance[vertex1-1][vertex2-1] = []

            self._instance[vertex1-1][vertex2-1].append(Arc(colour))


    def remove_arc(self, vertex1, vertex2, colour):
        if vertex1 > vertex2:
            self._instance[vertex2-1][vertex1-1].remove(colour)
            if self._instance[vertex2-1][vertex1-1] == []:
                self._instance[vertex2-1][vertex1-1] = None
        else:
            self._instance[vertex1-1][vertex2-1].remove(colour)
            if self._instance[vertex1-1][vertex2-1] == []:
                self._instance[vertex1-1][vertex2-1] = None

    def get(self, vertex1, vertex2):
        if vertex1 > vertex2:
            return self._instance[vertex2-1][vertex1-1]
        else:
            return self._instance[vertex1-1][vertex2-1]


    def _neighbours(self, vertex):
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
        vertex_queue.append([random.randint(1, self._num_vertexes-1), -1])
        while len(vertex_queue) > 0:
            current_vertex = vertex_queue[0]
            vertex_queue.pop(0)
            neighbours = self._neighbours(current_vertex[0])
            
            if current_vertex[1] != -1:
                neighbours.remove(current_vertex[1])

            if _has_loop(visited_vertexes, neighbours):
                return False
            
            visited_vertexes.append(current_vertex[0])
            vertex_queue += [[x, current_vertex[0]] for x in neighbours]

        return len(visited_vertexes) == self._num_vertexes
    

    def copy_arcs_to(self, neighbour):
        for i in range(0, self._num_vertexes):
            for j in range(i, self._num_vertexes):
                self_arc = self.get(i+1, j+1)
                # by not checking if this arc is already in neighbour if both parents have an arc, it will be doubled
                if self_arc != None:
                    for arc in self_arc:
                        neighbour.add_arc(i+1, j+1, arc.get_colour())


    def locate_cycle(self):
        vertex_stack = []

        vertex_stack.append([random.randint(1, self._num_vertexes-1), []])
        # dfs where each element is [vertex, ["history"]]
        while True:
            current_vertex = vertex_stack[-1]
            vertex_stack.pop(-1)

            # if vertex is in its own history, then it has a cycle
            if current_vertex[0] in current_vertex[1]:
                return current_vertex[1][current_vertex[1].index(current_vertex[0]):]

            neighbours = self._neighbours(current_vertex[0])
            
            # if has father, remove father from neighbours
            if current_vertex[1] != []:
                neighbours.remove(current_vertex[1][-1])

            vertex_stack += [[x, current_vertex[1] + [current_vertex[0]]] for x in neighbours]


    def spanning_treefy(self):
        while not self.is_spanning_tree():
            cycle = self.locate_cycle()
            to_be_removed = random.randint(0, len(cycle)-1)

            # treat cycle as a circular list
            vertex1 = cycle[to_be_removed % len(cycle)]
            vertex2 = cycle[(to_be_removed+1) % len(cycle)]
            colours = self.get(vertex1, vertex2)

            self.remove_arc(vertex1, vertex2, colours[random.randint(0, len(colours)-1)])

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