from datetime import datetime, date, timedelta
from random import randint

class DateUtils:

    @staticmethod
    def randomDate(start, end):
        return start + timedelta( seconds = randint(0, int((end - start).total_seconds())) )
