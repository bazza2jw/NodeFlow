module(...,package.seeall)
require "servercommon"
--
-- MySQL database value logger
--
--
-- register with the server handler map
Utility.registerServerHandler("MysqlDatabase",Server)
--
--

-- table of MySQL database object - usually exppect no more than two
MySQLDb = {}

-- set up the metatable
MysqlDatabase_mt = { __index = MysqlDatabase }
-- Constructor
-- use as  obj = RandomGenerator.new("path")
function new(obj)
    --
    -- declare and construct task object
    -- all objects should have a path member
    --
    return setmetatable({Timer = 0, path =  obj, enabled = false, RecordBuffer = ""},MysqlDatabase_mt)
end

-- mark it has loaded - register the constructor in the Types table
Types["MysqlDatabase"] =  MysqlDatabase.new
--
--
-- Declare the standard functions that interface to the ServerHandler class
--
-- Create the configuration items and set attributes
--
function create(path)
    if(not Configuration:exists(path)) then
        Configuration:setString(path,"MysqlDatabase")
        Configuration:setAttributeString(path,"type","MysqlDatabase")
        Configuration:setString(path .. ".Hostname",Configuration:getString("Server.Hostname"))
        Configuration:setString(path .. ".Database","LUAPC")
        Configuration:setString(path .. ".Username","Not Set")
        Configuration:setString(path .. ".Password","Not Set")
        Configuration:setString(path .. ".Type","QMYSQL")
        Objects[path] = MysqlDatabase.new(path)

    end
end

-- delete a random generator object
function delete(path)
    if(Objects[path] ~= nil) then
        -- stop the object
        Objects[path]:stop()
        -- remove the object from the object list
        Objects[path] = nil
    end
end
-- configuration updated
-- sort out any inconistancies
function update(path)
-- the object will be stopped and started implicitly
    Objects[path]:configurationChanged()
end


-- an object has been renamed
function rename(path, newName,newPath)
-- change the path
    Objects[path].path = newPath
-- change the object
    Objects[newPath] = Objects[path]
-- remove old reference
    Objects[path] = nil
end
--
-- Command
--
function command(method,path,data)
    Objects[path]:objectCommand(method,data)
end
-- END OF C++ interface

-- create runtime items
function MysqlDatabase:initialiseRuntime()
    Runtime:setString(self.path,"MySQL")
    Runtime:setAttributeString(self.path,"type","MysqlDatabase")
    Runtime:setAttributeString(self.path,"status","Ok")
    Server:runtimeChanged(self.path)
end

-- the task runs every 10 seconds to flush the record buffer
-- to databases
function MysqlDatabase:task()
    if self.enabled and (string.len(self.RecordBuffer) > 1) then
        -- is the database enabled for writing
        if Db.open(self.path) then
            -- open database
            -- write the record
            if not Db.exec(self.path,self.RecordBuffer) then
                -- add diagnostics
                qxtLog:error(self.path,"Failed to write to database",self.path)
            end
            self.RecordBuffer = ""
        end
    end
end

function MysqlDatabase:start()
    -- open the database to see if it is there
    -- creates if it does not exist

    local hname = Configuration:getString(self.path .. ".Hostname")
    local dname = Configuration:getString(self.path .. ".Database")
    local uname = Configuration:getString(self.path .. ".Username")
    local pass =  Configuration:getString(self.path .. ".Password")
    self.RecordBuffer = ""
    self.enabled = false
    if(Db.create(self.path,"QMYSQL",hname,dname,uname,pass)) then
        if Db.open(self.path) then
            -- create & test open if necessary
            -- create tables if required
            local hasTable = Db.hasTable(self.path,"NUMBERVALUES")
            print("hasTable",hasTable)
            if hasTable == 0 then
                print("Create tables")
                -- this table is for number values with status
                Db.exec(self.path,"CREATE TABLE NUMBERVALUES (F_SERVER VARCHAR(256),F_OBJECT VARCHAR(256), F_TIMESTAMP DATETIME, F_STATE VARCHAR(32), F_COMMENT VARCHAR(256), F_VALUE FLOAT, F_MEAN FLOAT, F_SD FLOAT, F_MINIMUM FLOAT,F_MAXIMUM FLOAT);")
                -- create an index on the table - the first key is the object name the second is the time
                -- this means data points from the same object are logically grouped together in time order
                Db.exec("CREATE INDEX NUMBERVALUES_INDEX ON NUMBERVALUES(F_OBJECT,F_TIMESTAMP)")
            end
            -- as other types are thought of create the tables here
            -- save the logging event messages here
            --Db.addLogger(self.path,"LOGGER")
            Runtime:setAttributeString(self.path,"state","Ok")
            Runtime:setAttributeNumber(self.path,"quality",QualityGood)
            Server:runtimeChanged(self.path)
            -- add to the timer table
            TenSecondTable[self.path] = self
            MySQLDb[self.path] = self
            self.enabled = true
        else
            -- database has failed to open
            qxtLog:error(self.path," Failed to open database ",self.path)
            Runtime:setAttributeString(self.path,"state","Fault")
            Runtime:setAttributeNumber(self.path,"quality",QualityBad)
            Server:runtimeChanged(self.path)
        end
    end
    Objects[self.path]:initialiseRuntime()
end

function MysqlDatabase:stop()
    -- halt the database - Make sure it is closed
    --Db.removeLogger(self.path,"LOGGER")
    Db.close(self.path)
    self.enabled = false;
    -- remove from timer table
    TenSecondTable[self.path] = nil
    Runtime:setAttributeString(self.path,"status","None")
    Runtime:setAttributeNumber(self.path,"quality",QualityBad)
    MySQLDb[self.path] = nil
    self.RecordBuffer = ""
end

function MysqlDatabase:objectCommand(op,data)
    -- say we succeed by default
    local ret = true
    if     op == "Start" then  self:start()
    elseif op == "Stop" then self:stop()
    elseif op == "Changed" then self:configurationChanged()
    else ret = false
    end
    return ret
end

--
-- Notification handlers
--
function MysqlDatabase:configurationChanged()
-- do this after a configuration change
-- when a value is updated change notify is generated

end

function MysqlDatabase:haveRecordValue (path, v)
    -- v is a table with the field set
    -- ideally we want to build a buffer of records and write many at once - this is much more effecient
    -- look up the object type - work out the type
    if Configuration:getAttributeString(path,"type") == "NumberValue" then
        -- construct the insert
        local cmd = "INSERT INTO NUMBERVALUES (F_SERVER,F_OBJECT,F_TIMESTAMP,F_STATE,F_COMMENT,F_VALUE,F_MEAN,F_SD,F_MINIMUM,F_MAXIMUM) VALUES ('%s','%s','%s','%s','%s',%g,%g,%g,%g,%g);\n"
        local insert = string.format(cmd,ServerName,path,v["Timestamp"],v["State"],v["Comment"],v["Value"],v["Mean"],v["Sd"],v["Minimum"],v["Maximum"])
        -- we could have a current values table too and generate update records - much the same really
        -- concatentate
        self.RecordBuffer = self.RecordBuffer .. insert
        -- if the buffer gets too big we must force a write
        if string.len(self.RecordBuffer) > 1000 then
                self:task()
        end
    end
end


