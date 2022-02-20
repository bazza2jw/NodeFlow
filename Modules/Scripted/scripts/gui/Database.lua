-- Resources configuration
-- resources are databases and serial ports
--
module(...,package.seeall)
--
--
require "clientcommon"
-- join the list of available interfaces
-- only can control the interface
Utility.registerClientHandler("Database","Add")
--
-- mark it has loaded
Types["Database"] =  true

-- The server object is static - server level control is possible
function menuAction(action,  objId, w)
    print("Database menuAction")
    if(action == "Add") then
        -- create a child object
        ClientDialogues.defaultCreate("",Client,w,"Resources.Databases","Database")
    end
end
