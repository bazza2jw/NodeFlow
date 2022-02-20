module(...,package.seeall)

require "servercommon"

-- use this as a template
-- Random number generating interface
-- Configuration has the following properties
-- Interval
-- RangeMin
-- RangeMax
-- Runtime has the following properties
-- Value

-- register with the server handler map
Utility.registerServerHandler("RandomGenerator",Server)

-- set up the metatable
RandomGenerator_mt = { __index = RandomGenerator }
-- Constructor
-- use as  obj = RandomGenerator.new("path")
function new(obj)
    --
    -- declare and construct task object
    -- all objects should have a path member
    --
    return setmetatable({Timer = 0, path =  obj, enabled = false, value = Utility.createStatistic()},RandomGenerator_mt)
end

-- mark it has loaded - register the constructor in the Types table
Types["RandomGenerator"] =  RandomGenerator.new

--
-- Declare the standard functions that interface to the ServerHandler class
--
-- create a random number object in the configuration and runtime trees
-- This is the C++ interface
--
function create(path)
    if(not Configuration:exists(path)) then
        Configuration:setString(path,"RandomGenerator")
        -- set parameters specific for the object type
        Configuration:setNumber(path .. ".RangeMin",0)
        Configuration:setNumber(path .. ".RangeMax",100)
        -- the value of the object
        servercommon.createNumberConfiguration(path .. ".Value")
        -- set the object type (class)
        Configuration:setAttributeString(path,"type","RandomGenerator")
        -- when start is called runtime values are created
        -- create the task object
        Objects[path] = RandomGenerator.new(path)
        -- now start it
        Objects[path]:start()
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
--
--
-- initialise the object runtime parts - This is called when the object is started
--
function RandomGenerator:initialiseRuntime()
    -- mark the object as not running
    Runtime:setBool(self.path .. ".Active", false)
    Runtime:setAttributeNumber(self.path,"quality",QualityBad)
    Runtime:setAttributeString(self.path,"type","RandomGenerator")
    Runtime:setAttributeNumber(self.path .. ".Value","quality",QualityBad)
    -- initialise the runtime values and mark them as bad (invalid)
    servercommon.createNumberRuntime(self.path .. ".Value")
    -- notify the runtime object has been created
    Server:runtimeCreated(self.path)
end


function RandomGenerator:generateNumber()
    local path = self.path
    -- print("generateNumber",path)
    return math.random(Configuration:getNumber(path .. ".RangeMin"),Configuration:getNumber(path .. ".RangeMax"))
end

-- This is the number generator
function RandomGenerator:task()
    if(self.Timer <= 0) then
        local path = self.path
        self.Timer = Configuration:getNumber(path .. ".Value.MeasureInterval")
        -- when a value is updated change notify is generated
        local n = self:generateNumber()
        self:valueUpdate("Value", n )
    else
        -- count down to next measurment
        self.Timer = self.Timer - 1
    end
end
-- start the object
function RandomGenerator:start()
    -- add the object to the one second table
    local path = self.path
    OneSecondTable[path] = self
    self.value.clear = 0
    self:initialiseRuntime()
    -- set a new value
    Runtime:setAttributeNumber(path,"quality",QualityGood)
    Runtime:setAttributeString(path,"state","Ok")
    self.Timer = 0
    Runtime:setBool(path .. ".Active", true)
    -- load the rolling buffer that has the numbers in it
    common.loadRollingBufferConfig(Configuration,path .. ".Value", self.value)
    local n = self:generateNumber()
    self:valueUpdate("Value", n )
    self.enabled = true
    Server:runtimeChanged(path)
end
-- stop the object
function RandomGenerator:stop()
    local path = self.path
    OneSecondTable[path] = nil
    Runtime:setBool(path .. ".Active", false)
    Runtime:setAttributeString(path,"state","None")
    self.Timer = 0
    -- set the quality to bad
    Runtime:setAttributeNumber(path,"quality",QualityBad)
    Runtime:setAttributeNumber(path .. ".Value","quality",QualityBad)
    Server:runtimeChanged(path)
end
--
-- command handler
-- op is the operation
-- data is a table with the arguments
-- here we return true or false as a result but can return any value or table
function RandomGenerator:objectCommand(op,data)
    -- say we succeed by default
    -- print("objectCommand",op)
    local ret = true
    if     op == "Start" then  self:start()
    elseif op == "Stop" then self:stop()
    elseif op == "Configure" then self:setConfiguration(data);self:configurationChanged()
    elseif op == "Changed" then self:configurationChanged()
    else ret = false
    end
    return ret
end
--
-- Set a configuration from a table - this is a message handler
-- Get and set requests can read/write any field
--
function RandomGenerator:setConfiguration(t)
    local path = self.path
    Configuration:setNumber(path .. ".Interval",tonumber(t["Interval"]))
    Configuration:setNumber(path .. ".RangeMin",tonumber(t["RangeMin"]))
    Configuration:setNumber(path .. ".RangeMax",tonumber(t["RangeMax"]))
    -- call the core configuration set up - this is for alarms and recording as required
    if(t["Value"] ~= nil) then
        servercommon.setNumberValueConfiguration(path .. ".Value", t["Value"])
    end
end
--
-- This method is post runtime value update handling
-- typically this is alarm limit testing/event and recording event generation/handling
--
function RandomGenerator:valueUpdate(item, value)
     if(item == "Value") then
        local path = self.path
        local itemPath = path .. ".Value"
        self.value.value = value;
        Runtime:setNumber(itemPath,value)
        common.setRollingBufferRuntime(Runtime, itemPath, self.value)
        -- set the top object display value as the mean of the rolling buffer - ie time averaged
        Runtime:setNumber(path,self.value.mean)
        -- propogate the value status to the object status
        Runtime:setAttributeString(path,"state",Runtime:getAttributeString(itemPath,"state"))
        Runtime:setAttributeString(path,"updated",Utility.getTimeStamp())
        -- now do the recording - this generates database events
        servercommon.recordNumber(itemPath)
        -- notify the value has changed
        Server:runtimeNewNumberValue(itemPath,value)
        -- notify the object has possibly changed
        Server:runtimeChanged(path)
    end
end
--
-- Notification handlers
--
function RandomGenerator:configurationChanged()
-- do this after a configuration change
-- when a value is updated change notify is generated
    self:stop()
    self:start()
    self:generateNumber()
end





