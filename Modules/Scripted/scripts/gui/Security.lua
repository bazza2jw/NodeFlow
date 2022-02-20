-- Resources configuration
-- resources are databases and serial ports
--
module(...,package.seeall)
--
--
require "clientcommon"
-- join the list of available interfaces
-- only can control the interface
Utility.registerClientHandler("Security","")
--
-- mark it has loaded
Types["Security"] =  true

-- The server object is static - server level control is possible
function menuAction(action, objId, w)

end
