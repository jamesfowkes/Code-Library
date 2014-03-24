import unittest

import sqlsoup

import os

from TestConstants import TEST_CONSTANTS
import DBRecord

path = TEST_CONSTANTS['DatabasePath']

class DBRecordTest(unittest.TestCase):
    
    def setUp(self):
        # Create a simple test database and fill with some data
        self.db = sqlsoup.SQLSoup("sqlite:///%s" % path)
        try:
            self.db.execute("DROP TABLE test")
        except:
            pass #Due to table not existing - this is fine
            
        self.db.execute("CREATE TABLE test (id INTEGER PRIMARY KEY, text1 TEXT, num1 INTEGER)")
        
    def testInitialiseRecord(self):
        record = self.getValidRecord()
        
        self.assertEqual(record.Get('id'), 0)
        self.assertEqual(record.Get('text1'), 'ABC')
        self.assertEqual(record.Get('num1'), 9)
    
    def testCreateRecord(self):
        
        self.assertEqual(self.recordCount(), 0)
        record = self.getValidRecord()
        self.assertTrue( record.Create(self.db) )
        self.assertEqual(self.recordCount(), 1)
    
    def testReadRecordFromValidPK(self):
        
        self.getValidRecord().Create(self.db)
        
        record = self.getBlankRecord()      
        self.assertTrue( record.ReadFromPK(0) )
        
        self.assertEqual(record.Get('id'), 0)
        self.assertEqual(record.Get('text1'), 'ABC')
        self.assertEqual(record.Get('num1'), 9) 
    
    def testReadRecordFromInvalidPK(self):
        
        self.getValidRecord().Create(self.db)
        
        record = self.getBlankRecord()      
        self.assertFalse( record.ReadFromPK(1) )
        
    def testReadRecordFromNonPKData(self):
        
        self.getValidRecord().Create(self.db)
        
        record = self.getBlankRecord()      
        self.assertTrue( record.Read({'text1':'ABC'}) )
        
        self.assertEqual(record.Get('id'), 0)
        self.assertEqual(record.Get('text1'), 'ABC')
        self.assertEqual(record.Get('num1'), 9)
        
    def testModifyRecord(self):
        
        record = self.getValidRecord()
        record.Create(self.db)
        
        record.Set('text1', "DEF")
        record.Set('num1', 18)
        self.assertTrue( record.Update(self.db, {'id':0}) )
        
        record = self.getBlankRecord()      
        record.Read({'id':0})
        
        self.assertEqual(record.Get('id'), 0)
        self.assertEqual(record.Get('text1'), 'DEF')
        self.assertEqual(record.Get('num1'), 18)
        
    def testDeleteRecord(self):
        
        record = self.getValidRecord()
        record.Create(self.db)
        
        self.assertEqual(self.recordCount(), 1)

        self.assertTrue( record.Delete(self.db) )
        self.assertEqual(self.recordCount(), 0)
        
    def getBlankRecord(self):
        return DBRecord.DBRecord(self.db.test, {'id':None, 'text1':None, 'num1':None}, 'id')
        
    def getValidRecord(self):
        return DBRecord.DBRecord(self.db.test, {'id':0, 'text1':'ABC', 'num1':9}, 'id')

    def recordCount(self):
        result = self.db.execute("SELECT COUNT(*) AS COUNT FROM test")
        return result.scalar()