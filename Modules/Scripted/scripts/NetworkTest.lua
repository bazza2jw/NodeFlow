-- Network connection test to echo server

testValue = 50

function start()

    if Network.Open("localhost",7) then
        print("Opened network connection")
    else
        print("Failed to open network connection")
    end
end

function process()
	print("Process")
    if Network.IsOk() then
        local response
        local pkt
        pkt = string.format("Packet %d\r",testValue)
        print("Sending" ,pkt)
        response = Network.DoPacket(pkt)
        print("Received ", response)
    end
end

function stop()

end
