"""
Record
Author: James Fowkes
Email: jamesfowkes@gmail.com
"""

from db.error import Error
from sqlalchemy.exc import SQLAlchemyError

class Record:

    """
    Database record class
    """

    def __init__(self, table, data, *pkfields):
        """
        Args:
        table : Table object that contains the record
        data : Dictionary of field:value pairs containing the record data
        *pkfields : Field names that are the primary keys of the data
        """
        self.table = table
        self.data = data
        self.pkfields = pkfields

    def get(self, field):
        """
        Returns the data for a field
        Args:
        field : fieldname to get data for
        """
        try:
            return self.data[field]
        except KeyError:
            raise Error("Field '%s' not found in table '%s" % (field, self.table.name))

    def set(self, field, value):
        """
        Sets the data for a field
        Args:
        field : fieldname to set data for
        """
        try:
            self.data[field] = value
        except KeyError:
            raise Error("", "Field '%s' not found in table '%s" % (field, self.table.name))

    def create(self, db):
        """ Create this record in a database
        Args:
        db : The database to create the record in
        """
        result = True

        try:
            self.table.insert(**self.data)
            db.commit()
        except:
            raise

        return result

    def read_from_pk(self, *pk_list):
        """ Read this record from table given primary keys
        Calls more general 'read' method
        Args:
        *pk_list : List of primary keys to search by
        """
        return self.read(dict(zip(self.pkfields, pk_list)))

    def read(self, filter_dict):
        """ Read this record from table given dictionary of filters
        Args:
        filter_dict : Dictionary of fieldnames and data to filter by
        """

        result = True
        try:
            record = self.table.filter_by(**filter_dict).one() # pylint: disable=star-args

            for k in self.data.keys():
                self.set(k, getattr(record, k))
        except SQLAlchemyError:
            result = False

        return result

    def delete(self, db):
        """ Delete this record from table
        Args:
        db :  The database to delete the from
        """
        result = True

        try:
            filter_dict = dict(zip(self.pkfields, self.__get_pk_attrs()))
            record = self.table.filter_by(**filter_dict).one() # pylint: disable=star-args
            db.delete(record)
            db.commit()

        except SQLAlchemyError:
            result = False

        return result

    def update(self, db, filter_dict):
        """ Update this record
        Args:
        db :  The database to update
        filter_dict : Dictionary of fieldnames and the data to set
        """
        result = True

        try:
            record = self.table.filter_by(**filter_dict).one() # pylint: disable=star-args
            for key, var in self.data.iteritems():
                setattr(record, key, var)
            db.commit()

        except SQLAlchemyError:
            result = False

        return result

    def __get_pk_attrs(self):
        """ Get the data associated with the primary keys of this record,
        returned as list
        """
        return [self.data[k] for k in self.pkfields]
