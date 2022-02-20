-- Resources configuration
-- resources are databases and serial ports
--
module(...,package.seeall)
--
--
require "clientcommon"
-- join the list of available interfaces
-- only can control the interface
Utility.registerClientHandler("ReportItem","Configure,Delete,Rename")
--
-- mark it has loaded
Types["ReportItem"] =  true

-- The server object is static - server level control is possible
function menuAction(action,objId, w)
    if(action == "Configure") then
    end
    if(action == "Delete") then
    end
    if(action == "Rename") then
    end

end
