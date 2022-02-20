-- The LUA part of the client interface
--
--
module(...,package.seeall)
-- load the server configuration file - spread configuration goes into the configration tree under the top level item spread
function loadSpreadConfiguration()
    local fs = io.open("./configuration/spread.conf")
    if(fs ~= nil) then
        local x = fs:read("*a")
        -- convert from XML to table
        print(x)
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
    -- load the spread configuration
    local t = client.loadSpreadConfiguration()
    print("Start Spread")
    return Spread:start(t["Host"],ClientName,t["Port"],t["Group"])
end


-- the one second timer tick
function oneSecondTimer()
    -- iterate the one second table
    for i,v in pairs(OneSecondTable) do
        if(type(v.task) == "function") then
            v:task()
        end
    end
end

function sendSpreadMessage(method)
    local msg ={}
    msg["method"] = method
    -- convert to JSON
    local txt = JsonValueToString(msg)
    if(txt ~= nil) then Spread:sendMessage(Spread:group(),txt) end
end




