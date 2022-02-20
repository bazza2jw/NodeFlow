ICPCON7018 = {}
--
-- ICPCON 7018 module handler
-- set up the metatable
-- the ICPCON has 8 analogue inputs
--
ICPCON7018_mt = { __index = ICPCON7018 }
-- Constructor
-- use as  obj = class.new(tree, "path")
function ICPCON7018.new(p,t,obj)
    --
    -- declare and construct task object
    -- all objects should have a parent , path member and a tree
    -- the LUA scripts run in the Type state
    -- the parent is assumed to be (must be) a IcpConModuleActiveObject
    --
    return setmetatable({parent = p,  path =  obj, tree = t, state = 0},ICPCON7018_mt)
end

function ICPCON7018.status()
    print ("Status")
end

-- register the constructor
ICPCON.IcpConTypes["ICPCON7018"] = ICPCON7018.new
Tree:setBool("ICPCON.ICPCON7018",1)
-- argument is the active object
function ICPCON7018:start()
    self.state = 0
end

-- argument is the active object

function ICPCON7018:stop()
    self.state = 0
end

-- argument is the active object

function ICPCON7018:updated()

end


function ICPCON7018:process()
    if (self.state == 0) then
        --
        --request configuration
        self.parent:send(1,"$","2")
        --otherwise do nothing
        self.state = 1
    elseif (self.state == 2) then
      self.parent:send(3, "#","")
      self.state = 1
    end

end

function ICPCON7018:haveResponse(caller, rId, response, errorId)
    local i
    local s
    local j
    local p

    local requestId = tonumber(rId)
    if(errorId == 0) then
          if(requestId == 1) then
              -- Configuration
              -- extract the format
              -- get the module id
              self.parent:send(2,"$","M")
          elseif (requestId == 2) then
              -- module id
              print(response)
              s = string.sub(response,4,7)
              print(s)
              if(s == "7018") then
              -- is it correct  -- enable all channels
                  self.parent:send(0,"$","5FF")
              -- request the readings
                  self.parent:send(3,"#","")
              else
              -- try again
                  self.state = 0
              end
              -- if so enable all channels and request data
          elseif (requestId == 3) then
              -- data
              -- parse data
              for i = 0,7 do
              -- set values
                  j = i * 7 + 2
                  s = string.sub(response,j,j + 6)
                  p = self.path .. ".Input" .. i
                  self.tree:setNumber(p, tonumber(s))
              end
              -- request data again
	      self.state = 2
              
          else
              -- ignore
              i = 0
          end
      else
          -- restart
          self.state = 0
      end
end


function ICPCON7018:create()
    self.tree:del(self.path .. ".Options")
    self.tree:setBool(self.path .. ".Options.4to20mA",false)
end

function ICPCON7018:edit(w)
    local ui = qt.ui.load_ui(Root .. "/ICPCON7018.ui",w)
    local p = self.path .. ".Options.4to20mA"
    ui.is4to20mA.checked = self.tree:getBool(p)
    if( ui:exec()) then
        self.tree:setBool(p,ui.is4to20mA.checked)
    end
end



print("Loaded ICPCON7018")
return ICPCON7018
