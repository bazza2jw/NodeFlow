-- Common routines and constants for client and server
--
-- Quality types
QualityNone = 0
QualityGood = 1
QualityBad = 2
--
Db =  plugin("./plugins/common/libLuaPcDatabase")
--
module(...,package.seeall)
function createNumberRuntime(runtime, p)
    -- create the number
    runtime:setNumber(p,0)
    runtime:setAttributeNumber(p, "quality", QualityNone)
    -- create current status
    runtime:setString(p .. ".State","None")
    runtime:setNumber(p .. ".NumberMeasurements",0)
    -- track the last time a value was recorded
    runtime:setNumber(p .. ".LastRecordTime",0)
    runtime:setAttributeString(p,"type","NumberValue")
end
-- Creates a number value in the configuration trees with alarm limits and recording parameters
function createNumberConfiguration(config,p)
    -- rolling buffer width
    config:setNumber(p .. ".Width",8)
    -- create the alarm limits
    config:setNumber(p .. ".HiHi",0)
    config:setNumber(p .. ".HiLo",0)
    config:setNumber(p .. ".LoHi",0)
    config:setNumber(p .. ".LoLo",0)
    -- create the enable bits
    config:setBool(p .. ".EnableHiHi",false)
    config:setBool(p .. ".EnableHiLo",false)
    config:setBool(p .. ".EnableLoHi",false)
    config:setBool(p .. ".EnableLoLo",false)
    -- SPC
    config:setBool(p .. ".TrackSpc",false)
    config:setNumber(p .. ".UpperControl",10)
    config:setNumber(p .. ".LowerControl",0)
    config:setBool(p .. ".UpperControlEnable",false)
    config:setBool(p .. ".LowerControlEnable",false)
    -- create the recording settings
    -- enable recording of values
    config:setBool(p .. ".EnableRecord", true)
    -- if true all values are recorded
    config:setBool(p .. ".RecordAll", true)
    -- number of seconds between recording values
    config:setNumber(p .. ".RecordInterval",60)
    config:setNumber(p .. ".MeasureInterval",30)
    -- set the attributes
    config:setAttributeString(p,"type","NumberValue")
    config:setAttributeString(p,"display","Dial")
    config:setAttributeNumber(p,"precision",1)
end
-- Set the configuration of a number value from a table
function setNumberValueConfiguration(config, p, t)
    -- create the alarm limits
    config:setNumber(p .. ".HiHi",t["HiHi"])
    config:setNumber(p .. ".HiLo",t["HiLo"])
    config:setNumber(p .. ".LoHi",t["LoHi"])
    config:setNumber(p .. ".LoLo",t["LoLo"])
    -- create the enable bits
    config:setBool(p .. ".EnableHiHi", t["EnableHiHi"])
    config:setBool(p .. ".EnableHiLo", t["EnableHiLo"])
    config:setBool(p .. ".EnableLoHi", t["EnableLoHi"])
    config:setBool(p .. ".EnableLoLo", t["EnableLoLo"])
    -- create the recording settings
    -- enable recording of values
    config:setBool(p .. ".EnableRecord", t["EnableRecord"])
    -- if true all values are recorded
    config:setBool(p .. ".RecordAll", t["RecordAll"])
    -- number of seconds between recording values
    config:setNumber(p .. ".RecordInterval", t["RecordingInterval"])
end

function loadRollingBufferConfig(config,path,r)
    if(config:exists(path)) then
        r.width = config:getNumber(path .. ".Width")
        r.ThresholdHiHi = config:getNumber(path .. ".HiHi")
        r.ThresholdHiLo = config:getNumber(path .. ".HiLo")
        r.ThresholdLoHi = config:getNumber(path .. ".LoHi")
        r.ThresholdLoLo = config:getNumber(path .. ".LoLo")
        --
        r.ThresholdHiHiEnable = config:getBool(path .. ".EnableHiHi")
        r.ThresholdHiLoEnable = config:getBool(path .. ".EnableHiLo")
        r.ThresholdLoHiEnable = config:getBool(path .. ".EnableLoHi")
        r.ThresholdLoLoEnable = config:getBool(path .. ".EnableLoLo")
        -- SPC
        r.trackSpc = config:getBool(path .. ".TrackSpc")
        r.UpperControl = config:getNumber(path .. ".UpperControl")
        r.LowerControl = config:getNumber(path .. ".LowerControl")
        r.UpperControlEnabled = config:getBool(path .. ".UpperControlEnabled")
        r.LowerControlEnabled = config:getBool(path .. ".LowerControlEnabled")
        --
    end
end



-- put the values from the rolling buffer stats
-- to runtime
function setRollingBufferRuntime(runtime, path, r)
    runtime:setNumber(path .. ".LastValue",r.lastValue)
    runtime:setNumber(path .. ".Mean",r.mean)
    runtime:setNumber(path .. ".Minimum",r.minimum)
    runtime:setNumber(path .. ".Maximum", r.maximum)
    runtime:setNumber(path .. ".Sd", r.sd)
    runtime:setNumber(path .. ".Range",r.range)
    runtime:setString(path .. ".LastRecordTime",Utility.getTimeStamp())
    --
    local s = "Ok"
    local a = r.MaxState
    if (a == 1) or (a== 2) then
        s = "Action"
        servercommon.actionNotification(path)
    elseif (a == 3) or (a==4) then
        s = "Alert"
        servercommon.alertNotification(path)
    end
    runtime:setAttributeString(path,"state",s)
    --
    -- Now report the SPC values
    --
    if r.trackSpc then
        runtime:setNumber(path .. ".TrendCount"  ,r.TrendCount)
        runtime:setNumber(path .. ".TriggerCount",r.TriggerCount)
        runtime:setNumber(path .. ".MeanCrowding",r.MeanCrowding)
    end
    runtime:setAttributeNumber(path,"quality",QualityGood)
end



