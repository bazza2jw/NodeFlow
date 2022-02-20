-- Resources configuration
-- resources are databases and serial ports
--
module(...,package.seeall)
--
require "clientcommon"
-- join the list of available interfaces
-- only can control the interface
Utility.registerClientHandler("View","Add")
--
-- mark it has loaded
Types["View"] =  true

-- The server object is static - server level control is possible
function menuAction(action,  objId, w)
    if(action == "Add") then
        -- to do add server level commands
        -- create a database object
        -- put up dialogue
        -- set database types
        -- exec it
        -- save and update
    end
end
