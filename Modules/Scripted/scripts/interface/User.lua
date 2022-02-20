module(...,package.seeall)

require "servercommon"

-- use this as a template
-- User object. Encapsulates security
-- Configuration has the following properties

-- register with the server handler map
Utility.registerServerHandler("User",Server)

-- set up the metatable
User_mt = { __index = User }
-- Constructor
-- use as  obj = User.new("path")
function new(obj)
    --
    -- declare and construct task object
    -- all objects should have a path member
    --
    return setmetatable({Timer = 0, path =  obj, enabled = false, value = Utility.createStatistic()},User_mt)
end

-- mark it has loaded - register the constructor in the Types table
Types["User"] =  User.new

--
-- Declare the standard functions that interface to the ServerHandler class
--
-- create a random number object in the configuration and runtime trees
-- This is the C++ interface
--
function create(path)
    if(not Configuration:exists(path)) then
        Configuration:setString(path,"User")
        -- set parameters specific for the object type
        Configuration:setString(path .. ".Password","password")
        Configuration:setBool(path .. ".writeAccess",1)
        -- the value of the object
        Configuration:setAttributeString(path,"type","User")
        -- when start is called runtime values are created
        -- create the task object
        Objects[path] = User.new(path)
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
function User:initialiseRuntime()
    -- mark the object as not running
    Runtime:setBool(self.path .. ".Active", false)
    Runtime:setAttributeNumber(self.path,"quality",QualityBad)
    Runtime:setAttributeString(self.path,"type","User")
    Server:runtimeCreated(self.path)
end



-- start the object
function User:start()
    -- add the object to the one second table
    local path = self.path
    self.value.clear = 0
    self:initialiseRuntime()
    -- set a new value
    Runtime:setAttributeNumber(path,"quality",QualityGood)
    Runtime:setAttributeString(path,"state","Ok")
    Runtime:setBool(path .. ".Active", true)
    -- load the rolling buffer that has the numbers in it
    self.enabled = true
    Server:runtimeChanged(path)
end
-- stop the object
function User:stop()
    local path = self.path
    Runtime:setBool(path .. ".Active", false)
    Runtime:setAttributeString(path,"state","None")
    self.Timer = 0
    -- set the quality to bad
    Runtime:setAttributeNumber(path,"quality",QualityBad)
    Server:runtimeChanged(path)
end
--
-- command handler
-- op is the operation
-- data is a table with the arguments
-- here we return true or false as a result but can return any value or table
function User:objectCommand(op,data)
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
function User:setConfiguration(t)
    local path = self.path
    Configuration:setString(path .. ".Password",t["Password"])
    Configuration:setBool(path .. ".RangeMin",t["RangeMin"])
end

--
-- Notification handlers
--
function User:configurationChanged()
-- do this after a configuration change

    self:stop()
    self:start()

end





