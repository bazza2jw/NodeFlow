-- User Configuration
--
--
module(...,package.seeall)
--
--
require "clientcommon"
-- join the list of available interfaces
-- only can control the interface
Utility.registerClientHandler("User","Configure,Delete,Rename","Users")
--
-- mark it has loaded
Types["User"] =  true

-- The server object is static - server level control is possible
function menuAction(action, objId, w)
    if(action == "Configure") then
        -- put up the configuration diaglogue
        dlg = qt.ui.load_ui("./modules/gui/UserConfigure.ui")
        if(dlg ~= nil) then
           -- make sure the object config is up to date
           dlg.password:setText(Configuration:getString(objId .. ".Password"))
           dlg.writeAccess:setChecked(Configuration:getBool(objId .. ".writeAccess"))
           if dlg:exec() then
            -- write the configuration back - TO DO encrypt the password
            Configuration:setString(objId .. ".password",dlg.password.text)
            Configuration:setBool(objId .. ".writeAccess",dlg.writeAccess.isChecked)
            Client:UpdateConfigureObject(objId)
           end
        end
    end
    if(action == "Delete")  then defaultDelete(objId,w)   end
    if(action == "Rename")  then defaultRename(objId,w)   end
end




-- default create is good enough
function create(config, path)
end

function User:displayObject(objId, w)
    -- create the display window for the object
    -- report on database status
end

function User:displayObjectValue(objId, valueName, w)
    -- view a single value
end

function User:graphicsObject(objId,w)
    -- returns a new widget for the object in a graphics view
end

function User:graphicsObjectValue(objId,valueName, w)
    -- returns a new widget for the object value in a graphics view
end
