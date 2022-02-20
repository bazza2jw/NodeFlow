-- Resources configuration
-- resources are databases and serial ports
--
module(...,package.seeall)
--
--
require "clientcommon"
-- join the list of available interfaces
-- only can control the interface
Utility.registerClientHandler("Users","Add User")
--
-- mark it has loaded
Types["Users"] =  true

-- The server object is static - server level control is possible
function menuAction(action, objId, w)
    if(action == "Add User") then
        ClientDialogues.defaultCreate("User",Client,w,objId,"Users")
    end
end
