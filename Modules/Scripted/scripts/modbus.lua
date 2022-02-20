-- MODBUS Test Interface
-- Auto detection is problematic
-- Refer to ICPCON for comprehensive solution
-- Will have to scan configuration tree to identify which modbus module to load and where they are
--
-- Array of devices indexed by slave address - value is the interface object
MODBUS_DEVICES = {}

testValue = 100
--
function start()
-- open the  port
    if Modbus.OpenTcp("localhost", "5000") then
        print("Opened TCP port")
    else
        print("Failed to open TCP port")
    end
end

function process()
    local i
-- read input registers
   if Modbus.ReadRegisters(1,0,3) then
        print("Holding Registers")
        for i = 0,2 do
            print(Modbus.GetRegister(i))
        end
   else
        print("Failed to read holding registers")
   end
   Modbus.WriteRegister(1,5,testValue)
   testValue = testValue + 9


end



function stop()

end



-- load modules using require
--

print("MODBUS modules loaded")
