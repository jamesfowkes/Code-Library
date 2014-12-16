"""
task.py

Implements functionality for repeating tasks
"""

class Task: #pylint: disable=too-few-public-methods
    """
    Simple class for implementing a single repeating task
    """

    def __init__(self, func, time, active):
        self.function = func
        self._period = time
        self.ticks = time
        self.active = active

    def set_active(self, active):
        """ Resets to maximum period and sets task to be active """
        self.set_period(self._period)
        self.active = active

    def set_period(self, ticks):
        """ Set a new task period """
        was_active = self.active
        self.active = False
        self._period = ticks
        self.reload()
        self.active = was_active

    def reload(self):
        """ Reset the remaining ticks to the task period """
        self.ticks = self._period

    def trigger_now(self):
        """ Immediately trigger the task regardless of time """
        self.function()

class TaskHandler:

    """
    Implements a handler for registering and executing repeating tasks
    """
    def __init__(self, parent):
        self._tasks = []
        self._parent = parent

    def add_function(self, function, time, active=True):
        """
        Adds a new function to the handler, with given time period between executions.
        Active state defaults to true (task starts immediately)
        """

        new_task = Task(function, time, active)
        self._tasks.append(new_task)

        return new_task

    def tick(self):
        """
        Application must call this function every tick (tick length is application dependent)
        in order for tasks to be executed
        """
        for task in self._tasks:
            if task.active and task.ticks > 0:
                task.ticks -= 1
                if task.ticks == 0:
                    task.reload()
                    task.function()
