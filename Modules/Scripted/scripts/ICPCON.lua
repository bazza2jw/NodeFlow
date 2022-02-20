--
-- This is the ICPCON master module. This module loads the other ICPCON MODULE modules
-- There is one ICPCON defintion class per module
--
package.path = "/Development/Projects/AirMonitors/Applications/InterfaceHarness/scripts/?.lua"
-- list of ICPCON module types - the table is keyed by the module ID - the value is the constructor for the interface object
IcpConTypes = {}
--
-- IcpConObjects currently active - the key for this table is the node id the value is the interface object
IcpConObjects = {}

-- Checksum calculation
function checksum(buf)
    local l
    local i
    local sum
    sum = 0
    l = string.len(buf)
    for i = 1,l do
        sum = sum + buf:byte(i,i)
    end
    return sum & 0xFF;
end

-- make a packet
function makePacket(node, op , data)
    local buf
    local sum
    local packet
    buf = string.format("%s%02X%s",op,node,data)
    sum = checksum(buf);
    packet = string.format("%s%02X\r",buf,sum);
    print("makePacket ",packet)
    return packet
end



--
-- Serial services are required and assumed
-- serial port is closed on stop
--
-- start the interface
function start()
    local i
    local res
    local req
    local modtype
    print("Starting ICPCON")
--
-- parameters will be accessed from the configuration
-- for now we fix the serial parameters - Only for development
   if Serial.Open("ttyUSB0",57600,8,1,78) then
    print("Serial port opened - Search for modules")
    -- response should arrive within 30 ms
    Serial.SetTimeout(30)
    --
    -- search for modules on interface - if we look over all the possible nodes this can take a minute
    -- here we only look for the first 8
    --
    for i = 1,8 do
        -- module ID
        req = makePacket(i,"$","M")
        res = Serial.DoPacket(req)
        print("Module Request",i,res)
        if res:len() > 0 then
            -- found a module
            modtype = res:sub(3,6)
            print("Found module node,type ",i, modtype)
            if IcpConTypes[modtype] ~= nil then
            -- create an interface instance
            print("Added Instance")
                IcpConObjects[i] = IcpConTypes[modtype](i)
            else
                print("Module not supported",modtype)
            end
        end
    end
    -- Now start each object - start up actions
    print("Completed auto detection")
    for key, obj in ipairs(IcpConObjects) do
        obj:start()
    end

   else
    print("Serial port not opened")
   end
end




function process()
    -- iterate the table
    for key, obj in ipairs(IcpConObjects) do
        obj:process()
    end
end


function stop()
    -- stop actions
    for key, obj in ipairs(IcpConObjects) do
        obj:stop()
        IcpConObjects[key] = nil
    end
    print("ICPCON Stopped")
end
-- load modules
require "ICPCON7017"
print("ICPCON Loaded")

