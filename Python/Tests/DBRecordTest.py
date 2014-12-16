"""
DBRecordTest
Tests the functionality of the record module
Author: James Fowkes
Email: jamesfowkes@gmail.com
"""

import unittest
import sqlsoup

from tests.testconstants import TEST_CONSTANTS
from db.record import Record

#pylint: disable=too-many-public-methods
class RecordTest(unittest.TestCase):

    """
    Tests the functionality of the Record module
    """

    def setUp(self):
        """.create a simple test database and fill with some data """
        self.db = sqlsoup.SQLSoup("sqlite:///%s" % TEST_CONSTANTS['DatabasePath'])
        try:
            self.db.execute("DROP TABLE test")
        except: #pylint: disable=bare-except
            pass #Due to table not existing - this is fine

        self.db.execute("CREATE TABLE test (id INTEGER PRIMARY KEY, text1 TEXT, num1 INTEGER)")

    def test_initialise_record(self):
        """ Test that a valid record object can be initialised """
        record = self.get_valid_record()

        self.assertEqual(record.get('id'), 0)
        self.assertEqual(record.get('text1'), 'ABC')
        self.assertEqual(record.get('num1'), 9)

    def test_create_record(self):
        """ Test that a valid record object can be stored """
        self.assertEqual(self.record_count(), 0)
        record = self.get_valid_record()
        self.assertTrue(record.create(self.db))
        self.assertEqual(self.record_count(), 1)

    def test_read_record_from_valid_pk(self):
        """ Test that a valid record object can be read based on primary key """
        self.get_valid_record().create(self.db)

        record = self.get_blank_record()
        self.assertTrue(record.read_from_pk(0))

        self.assertEqual(record.get('id'), 0)
        self.assertEqual(record.get('text1'), 'ABC')
        self.assertEqual(record.get('num1'), 9)

    def test_read_record_from_invalid_pk(self):
        """ Test that an invalid primary key does not read a record """
        self.get_valid_record().create(self.db)

        record = self.get_blank_record()
        self.assertFalse(record.read_from_pk(1))

    def test_read_record_from_non_pk_data(self):
        """ Test that an record can be read using non primary key data """
        self.get_valid_record().create(self.db)

        record = self.get_blank_record()
        self.assertTrue(record.read({'text1':'ABC'}))

        self.assertEqual(record.get('id'), 0)
        self.assertEqual(record.get('text1'), 'ABC')
        self.assertEqual(record.get('num1'), 9)

    def test_modify_record(self):
        """ Test that a record can be modified """
        record = self.get_valid_record()
        record.create(self.db)

        record.set('text1', "DEF")
        record.set('num1', 18)
        self.assertTrue(record.update(self.db, {'id':0}))

        record = self.get_blank_record()
        record.read({'id':0})

        self.assertEqual(record.get('id'), 0)
        self.assertEqual(record.get('text1'), 'DEF')
        self.assertEqual(record.get('num1'), 18)

    def test_delete_record(self):
        """ Test that a record can be deleted """
        record = self.get_valid_record()
        record.create(self.db)

        self.assertEqual(self.record_count(), 1)

        self.assertTrue(record.delete(self.db))
        self.assertEqual(self.record_count(), 0)

    def get_blank_record(self):
        """ Returns a record object with None data """
        return Record(self.db.test, {'id':None, 'text1':None, 'num1':None}, 'id')

    def get_valid_record(self):
        """ Returns a record object with valid data """
        return Record(self.db.test, {'id':0, 'text1':'ABC', 'num1':9}, 'id')

    def record_count(self):
        """ Returns the current count of records in the test table """
        result = self.db.execute("SELECT COUNT(*) AS COUNT FROM test")
        return result.scalar()
