
module(...,package.seeall)
-- This is the client side of the RandomGenerator object type
-- load the common routines
--
--
require "clientcommon"
-- join the list of available interfaces
Utility.registerClientHandler("RandomGenerator","Configure,Delete,Rename,Control","Object","View")
--
-- mark it has loaded
Types["RandomGenerator"] =  true

--
-- interface methods
-- menu handler
function menuAction(action, objId, w)
    if(action == "Configure")  then RandomGenerator.configure(objId,w) end
    if(action == "Delete")  then
        ClientDialogues.defaultDelete(objId,Client,w)
    end
    if(action == "Rename")  then ClientDialogues.defaultRename(objId,Client,w)   end
    if(action == "Control") then ClientDialogues.defaultControl(objId,w)   end
end
--
-- Configure a RandomGenerator object
-- id is the object name
-- w is the parent widget
--
function configure(objId,w)
    -- configure
    --
    dlg = qt.ui.load_ui("./modules/gui/RandomGeneratorConfigure.ui")
    if(dlg ~= nil) then
       -- make sure the object config is up to date
       dlg.Name:setText(objId)
       if(Configuration:exists(objId)) then
           dlg.RangeMin:setValue(Configuration:getNumber(objId .. ".RangeMin"))
           dlg.RangeMax:setValue(Configuration:getNumber(objId .. ".RangeMax"))
       end
       -- set up the configuration dialogue
       local res = dlg:exec()
       if res then
           -- get the configuration values
            Configuration:setNumber(objId .. ".RangeMin",dlg.RangeMin.value)
            Configuration:setNumber(objId .. ".RangeMax",dlg.RangeMax.value)
           -- post the update back to the server
            Client:UpdateConfigureObject( objId,"")
       end
    end
end

-- default create is good enough
function create(config, path)
end

function displayObject(objId, w)
    -- create the display window for the object

end

function displayObjectValue(objId, valueName, w)
    -- view a single value

end

function graphicsObject(objId,w)
    -- returns a new widget for the object in a graphics view
end

function graphicsObjectValue(objId,valueName, w)
    -- returns a new widget for the object value in a graphics view

end

