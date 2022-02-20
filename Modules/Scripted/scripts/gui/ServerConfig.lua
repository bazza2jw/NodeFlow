-- Server Client Interface
module(...,package.seeall)
--

--
require "clientcommon"
-- join the list of available interfaces
-- only can control the interface
Utility.registerClientHandler("ServerConfig","Control")
--
-- mark it has loaded
Types["ServerConfig"] =  true

-- The server object is static - server level control is possible
function menuAction(action,  objId, w)
    if(action == "Control") then
    -- to do add server level commands
        ClientDialogues.defaultControl(server,w)
    end
end

