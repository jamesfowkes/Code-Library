"""
dbcreator
Author: James Fowkes
Email: jamesfowkes@gmail.com
"""

# Python imports
import logging
import sys

# Local imports
from db.error import Error

CREATE_TABLE_TEMPLATE = "CREATE TABLE IF NOT EXISTS %s (%s)"
INSERT_INTO_TEMPLATE = "INSERT INTO %s (%s) VALUES (%s)"

class DBCreator:

    """
    Creates a database
    """

    def __init__(self, db, tabledefs):
        """
        Args:
        db : The database object to use
        tabledefs : List of table definition objects
        """
        if db is None:
            raise Error("DBCreator.__init__", "No database object provided")

        self.errors = []
        self.db = db
        self.tabledefs = tabledefs

        self.logger = logging.getLogger(self.__class__.__name__)
        logging_handler = logging.StreamHandler(sys.stdout)
        logging_handler.setLevel(logging.DEBUG)
        self.logger.addHandler(logging_handler)

    def __clear_errors(self):

        """ Clear the errors list """
        self.errors = []

    def create_missing_tables(self):

        """ Create any missing tables """

        self.__clear_errors()

        for name, tabledef in self.tabledefs.items():
            try:
                _sql = get_create_table_sql(name, tabledef)
                self.logger.info("Creating table %s...", name)
                self.logger.info("SQL: '%s'", _sql)
                self.db.execute(_sql)
            except Exception as err: #pylint: disable=broad-except
                self.errors.append(str(err))

        if len(self.errors) > 0:
            raise Error("create_missing_tables", "Error(s) occured while creating tables")

    def populate_defaults(self, table, columns, values):

        """
        Populates a table with default values
        Args:
        table : Table to populate with default values
        columns : List of column names
        values : List of lists of values (one list per row)
        """

        self.__clear_errors()

        try:
            if table_is_empty(self.db, table):
                for value in values:
                    _sql = INSERT_INTO_TEMPLATE % (table, ", ".join(columns), ", ".join(value))
                    self.db.execute(_sql)
        except Exception as err: #pylint: disable=broad-except
            self.errors.append(str(err))

def get_create_table_sql(name, tabledef):

    """ Generates a create table statement
    Args:
    name : The name of the new table
    tabledef : List of column definitions
    """
    return CREATE_TABLE_TEMPLATE % (name, get_column_def_string(tabledef))

def get_column_def_string(tabledef):

    """Creates SQL compliant column definition
    for entire table from table definition
    Args:
    tabledef : List of column definitions
    """

    columndefs = ", ".join([create_column_def(column) for column in tabledef])
    return columndefs

def create_column_def(columndef):

    """Creates SQL compliant column definition
    from single local column definition
    Args:
    columndef : List of column names
    """
    try:
        sqlcolumndef = columndef[1] % columndef[0]
    except Exception: #pylint: disable=broad-except
        print(columndef)

    return sqlcolumndef

def table_is_empty(db, table):

    """ Returns true if given table has a rowcount of zero """
    _sql = "SELECT COUNT(*) AS row_count FROM %s" % table
    result = db.execute(_sql).fetchone()
    return result['row_count'] == 0
