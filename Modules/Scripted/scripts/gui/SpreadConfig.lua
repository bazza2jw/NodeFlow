-- Spread object - only has configuration and restart
module(...,package.seeall)
--

--
require "clientcommon"
-- join the list of available interfaces
-- only can control the interface
Utility.registerClientHandler("SpreadConfig","Configure,Restart")
--
-- mark it has loaded
Types["SpreadConfig"] =  true

-- The server object is static - server level control is possible
function menuAction(action,  objId, w)
    print("Spread Menu Action",action,objId,w)
    if(action == "Configure") then
        -- put up the config dialog
        local dlg = qt.ui.load_ui("./modules/gui/SpreadConfigure.ui")
        if(dlg ~= nil) then
            dlg.spreadHost:setText(Configuration:getString("Spread.Host"))
            dlg.spreadGroup:setText(Configuration:getString("Spread.Group"))
            dlg.spreadPort:setValue(Configuration:getNumber("Spread.Port"))
            dlg:exec()
            conf:setString("Spread.Host",dlg.spreadHost.text)
            conf:setString("Spread.Group",dlg.spreadGroup.text)
            conf:setNumber("Spread.Port",dlg.spreadPort.value)
            --
            -- Now update the spread configuration
            local msg ={}
            msg["method"] = "Spread.Update"
            local params = {}
            params["Host"]  = dlg.spreadHost.text
            params["Port"]  = dlg.spreadPort.value
            params["Group"]  = dlg.spreadGroup.text
            -- complete the message
            msg["params"] = params
            -- convert to JSON
            local txt = JsonValueToString(msg)
            Spread:sendMessage(Spread:group(),txt)
        end
    end
    if(action == "Restart" ) then
        -- get confirmation
        if ClientDialogues.yesNoDialogue("Restart Spread","Restart Spread",w) then
            client.sendSpreadMessage("Spread.Restart")
        end
    end
end
