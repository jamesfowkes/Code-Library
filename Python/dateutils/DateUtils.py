"""
DateUtils
Author: James Fowkes
Email: jamesfowkes@gmail.com
"""

from datetime import timedelta
from random import randint

@staticmethod
def random_date(start, end):
    """ random_date
    Produces a random date between the two given dates
    """
    seconds_after_start = randint(0, int((end - start).total_seconds()))
    return start + timedelta(seconds=seconds_after_start)
