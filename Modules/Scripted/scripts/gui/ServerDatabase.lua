-- Resources configuration
-- resources are databases and serial ports
--
module(...,package.seeall)
--
--
require "clientcommon"
-- join the list of available interfaces
-- only can control the interface
Utility.registerClientHandler("ServerDatabase","Configure","")
--
-- mark it has loaded
Types["ServerDatabase"] =  true

-- The server object is static - server level control is possible
function menuAction(action, objId, w)
    if(action == "Configure") then
        -- put up the configuration diaglogue
        dlg = qt.ui.load_ui("./modules/gui/ServerDatabaseConfiguration.ui")
        if(dlg ~= nil) then
           -- make sure the object config is up to date
           dlg.Type:setEditText(Configuration:getString(objId .. ".Type"))
           dlg.Hostname:setText(Configuration:getString(objId .. ".Hostname"))
           dlg.Database:setText(Configuration:getString(objId .. ".Database"))
           dlg.Username:setText(Configuration:getString(objId .. ".Username"))
           dlg.Password:setText(Configuration:getString(objId .. ".Password"))
           if dlg:exec() then
            -- write the configuration back
            Configuration:setString(objId .. ".Type",    dlg.Type.currentText)
            Configuration:setString(objId .. ".Hostname",dlg.Hostname.text)
            Configuration:setString(objId .. ".Database",dlg.Database.text)
            Configuration:setString(objId .. ".Username",dlg.Username.text)
            Configuration:setString(objId .. ".Password",dlg.Password.text)
            Client:UpdateConfigureObject(objId)
           end
        end
    end
end




-- default create is good enough
function create(config, path)
end

function ServerDatabase:displayObject(objId, w)
    -- create the display window for the object
    -- report on database status
end

function ServerDatabase:displayObjectValue(objId, valueName, w)
    -- view a single value
end

function ServerDatabase:graphicsObject(objId,w)
    -- returns a new widget for the object in a graphics view
end

function ServerDatabase:graphicsObjectValue(objId,valueName, w)
    -- returns a new widget for the object value in a graphics view
end
