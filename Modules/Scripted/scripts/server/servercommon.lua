-- common routines for server and interfaces
module(...,package.seeall)
-- Creates a number value in the runtime  trees with alarm limits and recording parameters
function createNumberRuntime(p)
    common.createNumberRuntime(Runtime,p)
end
-- Creates a number value in the configuration trees with alarm limits and recording parameters
function createNumberConfiguration(p)
    common.createNumberConfiguration(Configuration,p)
end
-- Set the configuration of a number value from a table
function setNumberValueConfiguration(p, t)
    common.setNumberValueConfiguration(Configuration, p, t)
end

-- check the alarm limits of aq number value
function testNumberLimits(p)
  local v = Runtime:getNumber(p)
  local st = "Ok"
  -- increment the number of measurments made
  Runtime:setNumber(p .. ".NumberMeasurments", Runtime:getNumber(p .. ".NumberMeasurments") + 1)
  -- test the limits
  if     Configuration:getBool(p .. ".EnableHiHi") then
    if v > Configuration:getNumber(p .. ".HiHi") then
        Server:postEvent(p,"HiHi",v)
        st = "HiHi"
        -- the update time of the increment is the last time the threshold was exceeded
        Runtime:setNumber(p .. ".NumberHiHi",  Runtime:getNumber(p .. ".NumberHiHi") + 1 )
    end
  elseif Configuration:getBool(p .. "EnableHiLo") then
    if v > Configuration:getNumber(p .. "HiLo") then
        Server:postEvent(p,"HiLo",v)
        Runtime:setNumber(p .. ".NumberHiLo",  Runtime:getNumber(p .. ".NumberHiLo") + 1 )
        st = "HiLo"
    end
  elseif Configuration:getBool(p .. ".EnableLoHi") then
    if v < Configuration:getNumber(p .. ".LoHi") then
        Server:postEvent(p,"LoHi",v)
        Runtime:setNumber(p .. ".NumberLoHi",  Runtime:getNumber(p .. ".NumberLoHi") + 1 )
        st = "LoHi"
    end
  elseif Configuration:getBool(p .. ".EnableLoLo") then
    if v < Configuration:getNumber(p .. ".LoLo") then
        Server:postEvent(p,"LoLo",v)
        Runtime:setNumber(p .. ".NumberLoLo",  Runtime:getNumber(p .. ".NumberLoLo") + 1 )
        st = "LoLo"
    end
  end
  Runtime:setAttributeString(p,"state", st)
  Runtime:setAttributeString(p,"quality", QualityGood)
  -- set the update time for the item
  Runtime:setAttributeString(p,"updated",Utility.getTimeStamp())
end

-- record a number value to the database
function recordNumber(p)
    local rf = false;
    if Configuration:getBool(p .. ".EnableRecord") then
        if Configuration:getBool(p .. ".RecordAll") then
            rf = true
        else
           if (Runtime:getNumber(p .. ".LastRecordTime") - Utility.TimeFromStart()) >= Configuration:getNumber(p .. ".RecordingInterval") then
            rf = true
            Runtime:setNumber(p .. ".LastRecordTime",Utility.TimeFromStart())
           end
        end
        -- record the value by emitting a record event
        -- database tasks or objects should pick up the notification and act
        if rf then
            local result = {}
            result["Timestamp"] = Utility:getTimeStamp();
            result["Object"] = p
            result["State"] = Runtime:getAttributeString(p,"state")
            result["Value"] = Runtime:getNumber(p)
            -- save the mean, min , max and sd
            result["Mean"] = Runtime:getNumber(p .. ".Mean")
            result["Maximum"]  = Runtime:getNumber(p .. ".Maximum")
            result["Minimum"]  = Runtime:getNumber(p .. ".Minimum")
            result["Sd"] = Runtime:getNumber(p .. "Sd")
            result["Comment"] = ""
            -- poll all of the databases to process the result
            server.record(p,result)
            -- pass to the C++ server object - casting to VariantMap is problematic
            Utility.serverRecord(Server,p,result)
        end
    end
end

-- post and alert notification
function alertNotification(path)
    qxtLog:critical(Utility.getTimeStamp(),"Alert",path,"","","","","","","")
    Server:postEvent(path,"Alert","")
end

-- post an action notification
function actionNotification(path)
    qxtLog:critical(Utility.getTimeStamp(),"Action",path,"","","","","","","")
    Server:postEvent(path,"Action","")
end



