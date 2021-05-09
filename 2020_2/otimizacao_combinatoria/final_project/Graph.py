from Arc import Arc

class Graph:
    def __init__(self, empty_instance):
        self._instance = empty_instance
        self._num_arcs = 0
    
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