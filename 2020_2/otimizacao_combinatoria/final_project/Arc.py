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
        return type(self) == type(o) and self.colour == o.colour
    
    def __hash__(self) -> int:
        return hash(self.colour)