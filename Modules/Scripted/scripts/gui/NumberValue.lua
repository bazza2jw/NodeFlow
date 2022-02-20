-- Resources configuration
-- resources are databases and serial ports
--
module(...,package.seeall)
--
require "clientcommon"
-- join the list of available interfaces
-- only can control the interface
Utility.registerClientHandler("NumberValue","Edit","","View")
--
-- mark it has loaded
Types["NumberValue"] =  true

-- The server object is static - server level control is possible
function menuAction(action,objId, w)
    if(action == "Edit") then
        ClientDialogues.numberValueConfiguration(objId,Client,w)
        Client:UpdateConfigureObject(objId,"")
    end
end
--
-- handle the runtime menu action
--
function menuRuntimeAction(action,objId, w)
    if(action == "View") then
        ClientDialogues.valueDataBrowser(objId,Client,w)
    end

end
