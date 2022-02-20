-- Resources configuration
-- resources are databases and serial ports
--
module(...,package.seeall)
--
--
require "clientcommon"
-- join the list of available interfaces
-- only can control the interface
Utility.registerClientHandler("Object","Add")
--
-- mark it has loaded
Types["Object"] =  true

-- The server object is static - server level control is possible
function menuAction(action,objId, w)
    if(action == "Add") then
        ClientDialogues.defaultCreate("",Client,w,objId,"Object")
    end
end
