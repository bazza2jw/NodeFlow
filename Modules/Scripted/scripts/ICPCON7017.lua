ICPCON7017 = {}
-- ICPCON 7017 module handler
-- set up the metatable
-- the ICPCON has 8 analogue inputs
--
ICPCON7017_mt = { __index = ICPCON7017 }
-- Constructor
--
function ICPCON7017.new(n)
    --
    return setmetatable({node = n },ICPCON7017_mt)
end


-- register the constructor
IcpConTypes["7017"] = ICPCON7017.new


function ICPCON7017:start()
    local packet
    -- enable all channels
    packet = makePacket(self.node,"$","5FF")
    Serial.DoPacket(packet)
end

-- argument is the active object

function ICPCON7017:stop()
end

-- Process
function ICPCON7017:process()
   local packet
   local res
   local i
   packet = makePacket(self.node,"#","")
   res = Serial.DoPacket(packet)
   if res:len() > 0 then
       for i = 0,7 do
       -- set values
           j = i * 7 + 1
           s = string.sub(res,j,j + 6)
           -- now post the value to the run time property tree as a number
           print("Value ",i,s)
       end
   end
end

print("Loaded ICPCON7017")
return ICPCON7017
