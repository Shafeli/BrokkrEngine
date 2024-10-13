
print("Hello, From Lua!")

Hello()

Test = Test + 100

BrokkrLogInfo("BrokkrLogInfo Log Test")
BrokkrLogDebug("BrokkrLogDebug Log Test")
BrokkrLogWarning("BrokkrLogWarning Log Test")
BrokkrLogError("BrokkrLogError Log Test")


function TestLuaCalling()
    BrokkrLogWarning("Test Passed: Brokkr Game Called Lua Function")
end