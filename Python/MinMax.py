"""
MinMax
Author: James Fowkes
Email: jamesfowkes@gmail.com
"""

class MinMax:

    """ Stores the minimum and maximum extent of a variable.
    For example:

    minmax = MinMax()
    minmax.new_value(1)
    minmax.new_value(-1)
    minmax.new_value(2)
    minmax.new_value(-3)

    minmax.max #2
    minmax.min #-3
    """

    def __init__(self, initial_min=None, initial_max=None):
        """
        Args:
        initial_min : If provided, sets the initial minimum
        initial_max : If provided, sets the initial maximum
        """
        if initial_min > initial_max:
            raise Exception(initial_min > initial_max, "If setting the min and max, max must be >= min")
        else:
            self.min = initial_min
            self.max = initial_max

    def new_value(self, value):
        """
        Args:
        value : New value. Sets a new minimum or maximum if outside those bounds.
        """
        if self.min is None:
            self.min = value

        if self.max is None:
            self.max = value

        self.min = min(self.min, value)
        self.max = max(self.max, value)

    def reset(self, initial_min=None, initial_max=None):
        """
        Resets the minimum and maximum
        Args: as per module constructor
        """
        self.__init__(initial_min, initial_max)
