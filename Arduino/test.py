import random

class Averager:
    
    def __init__(self):
        self.average = 0
        
    def update(self, c):
        self.average = (self.average + c) / 2
        
averager = Averager()

for i in range(100):
    averager.update(random.randint(50, 100))
    print(averager.average)
