-- Server Client Interface
module(...,package.seeall)
--
require "clientcommon"
print("Client Spread Config")
-- join the list of available interfaces
-- can save the client configuration
Utility.registerClientHandler("ClientSpreadConfig","Configure")
--
-- mark it has loaded
--
Types["ClientSpreadConfig"] =  true

--
-- The server object is static - server level control is possible
function menuAction(action,  objId, w)
    if(action == "Configure") then
        -- put up the config dialog
        local dlg = qt.ui.load_ui("./modules/gui/SpreadConfigure.ui")
        if(dlg ~= nil) then
            dlg.spreadHost:setText(Configuration:getString("Client.Spread.Host"))
            dlg.spreadGroup:setText(Configuration:getString("Client.Spread.Group"))
            dlg.spreadPort:setValue(Configuration:getNumber("Client.Spread.Port"))
            if(dlg:exec()) then
                Configuration:setString("Spread.Host",dlg.spreadHost.text)
                Configuration:setString("Spread.Group",dlg.spreadGroup.text)
                Configuration:setNumber("Spread.Port",dlg.spreadPort.value)
           end
        end
    end
end


function initialise()
    -- set up the tree elements
    Configuration:setString("Client.Spread","")
    Configuration:setAttributeString("Client.Spread","type","ClientSpreadConfig")
    -- set up structure with defaults
    Configuration:setString("Client.Spread.Host","localhost")
    Configuration:setNumber("Client.Spread.Port",4803)
    Configuration:setString("Client.Spread.Group","LUAPC")
    -- load the configuration for this segment if it exists
end

function save(fd)
-- save the configuration to the file

end
