class MinMaxError(Exception):
    def __init__(self, expr, msg):
        self.expr = expr
        self.msg = msg
        
class MinMax:

    def __init__(self, initialMin = None, initialMax = None):
        if initialMin > initialMax:
            raise MinMaxError(initialMin > initialMax, "If setting the min and max, max must be >= min")
        else:
            self.min = initialMin
            self.max = initialMax

    def newValue(self, value):
        if self.min is None:
            self.min = value
            
        if self.max is None:
            self.max = value
            
        if value < self.min:
            self.min = value
            
        if value > self.max:
            self.max = value
            
    def reset(self, initialMin = None, initialMax = None):
        self.__init__(initialMin, initialMax)