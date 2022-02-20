-- This is the start up routine for the client side
--
package.path = "./modules/client/?.lua;./modules/common/?.lua;./modules/gui/?.lua"
-- load the utility plugin
Utility = plugin("./plugins/common/libLuaUtilityPlugin")
-- load the client dialogues
ClientDialogues =  plugin("./plugins/client/libClientDialogues")
-- The objects table - the data is in the configuration and runtime tables
Objects = {}
-- the co-routines table
CoRoutines = {}
-- the object types table - table of type interfaces
Types = {}
-- list of views that are available
Views = {}
--
-- One second table
OneSecondTable = {}
-- set to false to terminate server
serverRunning = true
-- load the base code
require "common"
-- load the server routines
require "clientcommon"
-- load the server routines
require "client"
-- create a unique client name - host name will be added to it
-- load the interfaces
-- in principle we have to fetch missing interfaces as and when, and  do updates
print("Load GUI modules")
Utility.requireDirectory("./modules/gui")
--
-- this is the start up code called after this file is loaded - second phase initialisation

function clientMain()
    -- start the spread interface
    -- client.startSpread()
    -- set up message maps

    -- connect up the timers
    -- all done ready to run

end


