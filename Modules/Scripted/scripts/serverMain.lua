-- This is the server start routine - load and run this to start the server process
-- after the timers and objects have been created it should be event driven or polled on timer events

-- set the package search path for require calls

package.path = "./modules/server/?.lua;./modules/common/?.lua;./modules/interfaces/?.lua"
-- get the plugins
-- load the utility functions
Utility = plugin("./plugins/common/libLuaUtilityPlugin")

-- Declare global tables
--
-- The objects table - the data is in the configuration and runtime tables
Objects = {}
-- the co-routines table
CoRoutines = {}
-- the object types table - table of constructors
Types = {}
-- One second table
OneSecondTable = {}
-- Ten second table
TenSecondTable = {}
-- The Resource objects
Resources = {}
-- set to false to terminate server
serverRunning = true
-- load the base code
require "common"
-- load the server routines
require "servercommon"
-- load the server routines
require "server"
-- load the server side interface objects
Utility.requireDirectory("./modules/interfaces")
--
-- This is the entry point for the server
-- Assume the configuration file has been loaded
-- and the resources set
function serverMain()
    -- create the basic runtime structure
    --
    Runtime:setString("Objects","")
    Runtime:setAttribute("Objects","type","Object")
    Runtime:setString("System","Running")
    Runtime:setAttribute("System","type","System")
    Runtime:setString("Resources","")
    Runtime:setAttribute("Resources","type","Resource")
    Runtime:setString("Resources.Databases","")
    Runtime:setAttributeString("Resources.Databases","type","Database")
    Runtime:setString("Resources.PacketDrivers","")
    Runtime:setAttributeString("Resources.PacketDrivers","type","PacketDriver")
    --
    -- the spread configuration is in the Configuration tree
    -- set the default group for posting and receiving events
    server.startSpread()
    -- create the database objects,loggers and packet drivers
    server.createResources()
    -- create the runtime objects and load interface modules
    server.createObjects()
    -- start (initialise) tasks
    server.startAll()
    -- connect the timers
    qt.connect(Timer1s,"timeout()",server.oneSecondTimer)
    qt.connect(Timer10s,"timeout()",server.tenSecondTimer)
    -- while serverRunning do
    -- now drive the co-routine table
    -- let the application process events between each co routine call
    --    Server:yield()
    --end
    --server.stopAll()
end



