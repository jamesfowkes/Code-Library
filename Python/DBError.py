class DBError(Exception):
    def __init__(self, expr, msg):
        self.expr = expr
        self.msg = msg