CREATE_TABLE_TEMPLATE = "CREATE TABLE IF NOT EXISTS %s (%s)"
INSERT_INTO_TEMPLATE = "INSERT INTO %s (%s) VALUES (%s)"

class SqlSoupDBCreator:
   
    def __init__(self, db, tabledefs):
        
        if db is None:
            raise DBError("DBCreator.__init__", "No database object provided")
        
        self.__clearErrors()
        self.db = db
        self.tabledefs = tabledefs
        
        self.logger = logging.getLogger(self.__class__.__name__)
        ch = logging.StreamHandler(sys.stdout)
        ch.setLevel(logging.DEBUG)
        self.logger.addHandler(ch)
    
    def __clearErrors(self):
        self.errors = []
        
    def createMissingTables(self):

        """Create any missing tables"""

        self.__clearErrors()
        
        for name, tabledef in self.tabledefs.items():
            columndefs = getColumnDefString(tabledef)
            try:
                _sql = GetCreateTableSQL(name, tabledef)
                self.logger.info("Creating table %s..." % name)
                self.logger.info("SQL: '%s'" % _sql)
                self.db.execute(_sql)
            except Exception as e:
                self.errors.append(str(e))
                
    if len(self.errors) > 0:
        raise DBError("createMissingTables", "Error(s) occured while creating tables")
        
    def populateDefaults(self, table, columns, values):
    
        self.__clearErrors()
        
        try:
            if tableIsEmpty(self.db, table):
                for value in values:
                    _sql = INSERT_INTO_TEMPLATE % (table, ", ".join(columns), ", ".join(values))
                    self.db.execute(_sql)
        except Exception as e:
            self.errors.append(str(e))
            
def getCreateTableSQL(name, tabledef)
    return CREATE_TABLE_TEMPLATE % (name, getColumnDefString(tabledef))

def getColumnDefString(tabledef):
    """Creates SQL compliant column definition
    for entire table from table defintion"""

    columndefs = ", ".join( [createColumnDef(column) for column in tabledef] )
    return columndefs

def createColumnDef(columndef):
    """Creates SQL compliant column definition 
    from single local column definition"""
    try:
        sqlcolumndef = columndef[1] % columndef[0]
    except:
        print columndef
        
    return sqlcolumndef
    
def tableIsEmpty(db, table):
    _sql = "SELECT COUNT(*) AS row_count FROM %s" % table
    result = db.execute(_sql).fetchone()
    return result['row_count'] == 0