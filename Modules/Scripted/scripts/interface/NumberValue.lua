module(...,package.seeall)
require "servercommon"
--
-- Number values are children of object - so just blank place holders
--
Utility.registerServerHandler("NumberValue",Server)

function create(path)
end

-- delete a random generator object
function delete(path)
end
-- configuration updated
-- sort out any inconistancies
function update(path)
end

-- an object has been renamed
function rename(path, newName,newPath)
end
--
-- Command
--
function command(method,path,data)
end
-- END OF C++ interface
