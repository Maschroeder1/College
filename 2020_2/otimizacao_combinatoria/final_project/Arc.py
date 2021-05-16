class Arc:
    def __init__(self, colour):
        self.colour = colour
    

    def get_colour(self):
        return self.colour

    def __repr__(self):
        return self.__str__()

    def __str__(self):
        return str(self.colour)

    def __eq__(self, o: object) -> bool:
        #print(self)
        #print(type(self))
        #print(o)
        #print(type(o))
        #print(type(self) == type(o))
        #if type(self) == type(o):
        #    print('ballz')
        #    print(self.colour)
        #    print(o.colour)
        #    print((type(self) == type(o)) and (self.colour == o.colour))
        #print('end')
        return (type(self) == type(o)) and (self.get_colour() == o.get_colour())
    
    def __hash__(self) -> int:
        return hash(self.colour)