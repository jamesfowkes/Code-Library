"""
Error
Author: James Fowkes
Email: jamesfowkes@gmail.com
"""

class Error(Exception):

    """
    Database error class
    """

    def __init__(self, expr, msg):

        """
        Args:
            expr : Expression that caused the error to be raised
            msg : Any additional information about the error
        """

        Exception.__init__(self)
        self.expr = expr
        self.msg = msg
