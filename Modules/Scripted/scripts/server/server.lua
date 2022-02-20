--Server Module - contains the core routines to impliment the server side
-- This requires that the state has been initialised with an LUA Server object
-- This will provide Spread interface for networking and messaging
-- The Configuration property tree
-- The Runtime property tree
-- The Application object
-- The Server object
-- There are three timers that objects can hook on to
-- 100ms Timer100ms
-- 1s    Timer1s
-- 10s   Timer10s
--
module(...,package.seeall)
-- get the LuaPC server side routines in
require "servercommon"

function createObject(path)
    -- get the type attribute
    if Configuration:hasAttribute(path,"type") then
        local t = Configuration:getAttributeString(path,"type")
        if Types[t] ~= nil then
            -- construct
            Objects[path] = Types[t](path)
            -- initialise the run time parts and hook up to timers etc
            Objects[path]:initialiseRuntime()
        else
            print("Failed to create object - missing interface",v)
        end
    end
end
-- from the configuration create the objects
function createObjects()
    local objList = Configuration:dir("Objects")
    for i,v in pairs(objList) do
        createObject("Objects." .. v)
    end
end


function record( path, data)
    local objList = Configuration:dir("Resources.Databases")
    for i,v in pairs(objList) do
        dbpath = "Resources.Databases." .. v
        if Objects[dbpath] ~= nil then
            Objects[dbpath]:haveRecordValue(path, data)
        end
    end
end



-- from the configuration create the objects
function createResources()
    local objList = Configuration:dir("Resources.Databases")
    for i,v in pairs(objList) do
        createObject("Resources.Databases" .. v)
    end
end



-- load the configuration from file
function loadConfiguration()
    --load the configuration for ServerName from configuration/<ServerName>.conf
    local ret = false;
    if(Configuration:load(string.format("./configuration/%s.conf",ServerName))) then
       ret = true
    end
    return ret
end
-- save the configuration
function saveConfiguration()
    --load the configuration for ServerName from configuration/<ServerName>.conf
    local ret = false
    if(Configuration:save( string.format("./configuration/%s.conf",ServerName))) then
        ret = true
    end
    return ret
end

-- load the server configuration file - spread configuration goes into the configration tree under the top level item spread
function loadSpreadConfiguration()
    local fs = io.open("./configuration/spread.conf")
    if(fs ~= nil) then
        local x = fs:read("*a")
        -- convert from XML to table
        return Utility.XmlStringToValue(x)
    end
    return nil;
end
-- save the spread configration
function saveSpreadConfiguration(host,port,group)
    local fs = io.open("./configuration/spread.conf","w")
    if(fs ~= nil) then
        local t = {["Host"] = host,["Port"] = port, ["Group"] = group}
        local s = Utility.XmlValueToString(t)
        fs:write(s)
        return true
    end
    return false
end
-- connect to the spread server - the spread server will reconnect as necessary
function startSpread()
    -- do the connection
      return Spread:start(Configuration:getString("Spread.Host"),ServerName,Configuration:getString("Spread.Port"),Configuration:getString("Spread.Group"))
end


-- the one second timer tick
function oneSecondTimer()
    -- iterate the one second table
    for i,v in pairs(OneSecondTable) do
         v:task()
    end
end

-- the ten second timer tick
function tenSecondTimer()
    -- iterate the one second table
    for i,v in pairs(TenSecondTable) do
         v:task()
    end
end

-- start all objects
function startAll()
    -- call start for all objects
    for i,v in pairs(Objects) do
        v:start()
    end
    Server:start()
end
-- stop all objects
function stopAll()
    Server:stop()
    for i,v in pairs(Objects) do
        v:stop()
    end
end




