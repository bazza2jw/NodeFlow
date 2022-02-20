-- Core routines for the client side
--
module(...,package.seeall)
require "common"

function objectPath(objId)
    return "Objects" .. objId
end

function getNextObjectId(server)
-- get the next object id for a given server
-- get the property tree
-- get the next id
   return  Client:GetNextObjectId(server)
end


-- default dialogues

function defaultCreate(w)
-- create
   return ClientDialogues.defaultCreate(Client,w)

end

function defaultDelete(path,w)
-- delete the object
   return ClientDialogues.defaultDelete(path,Client,w)
end

function defaultRename(path,w)
-- rename (change the display name)
   return ClientDialogues.defaultRename(path,Client,w)
end

-- control an object
function defaultControl(objId,w)
    return ClientDialogues.defaultControl(path,Client,w)
end

