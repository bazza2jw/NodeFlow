-- Resources configuration
-- resources are databases and serial ports
--
module(...,package.seeall)
--
--
require "clientcommon"
-- join the list of available interfaces
-- only can control the interface
--
-- change to match the available databases
--
Utility.registerClientHandler("Resources","Detect")
--
-- mark it has loaded
Types["Resources"] =  true

-- The server object is static - server level control is possible
function menuAction(action, objId, w)
    if(action == "Detect") then
    end
end
