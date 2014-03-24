class DBRecord:

    def __init__(self, table, data, *pkfields):
        self.table = table
        self.data = data
        
        self.pkfields = pkfields
        
    def Get(self, field):
        return self.data[field]
        
    def Set(self, field, value):
        self.data[field] = value
        
    def Create(self, db):
        result = True

        try:
            self.table.insert(**self.data)  
            db.commit()
            
        except:
            raise
            
        return result
    
    def ReadFromPK(self, *pk_list):
        return self.Read(dict(zip(self.pkfields, pk_list)))
        
    def Read(self, filter_dict):
    
        result = True
        try:
            record = self.table.filter_by(**filter_dict).one()
        
            for k in self.data.keys():
                self.Set(k, getattr(record, k))
        except:
            result = False
            
        return result
            
    def Delete(self, db):
    
        result = True
        
        try:
            filter_dict = dict(zip(self.pkfields, self.__getPKAttrs()))
            record = self.table.filter_by(**filter_dict).one()
            db.delete( record )
            db.commit()
            
        except:
            result = False
            
        return result
        
    def Update(self, db, filter_dict):
    
        result = True
        
        try:
            record = self.table.filter_by(**filter_dict).one()
            for k, v in self.data.iteritems():
                setattr(record, k, v)
            db.commit()
            
        except:
            result = False
            
        return result

    def __getPKAttrs(self):
        return [self.data[k] for k in self.pkfields]
        
        