local granmodule = {}
granmodule.state = {}
local matrix = require "../lib/matrix"
local sf = require "../lib/LuaSF"

function granmodule.init()
    granmodule.L = matrix{{1.00005, 0.},
    {-0.299985, 0.10054352}} -- cholensky decomposition matrix
end

function granmodule.generate(...) -- receives external state control as args
    local u = matrix{sf.normalVA(), sf.normalVA()}
    local x = granmodule.L * u
    -- create parameters for a grain and modify state if needed
    rate = .01
    dur = 0.05 + (x[1][1] / 20)
    while dur <= 0 
    do
        u = matrix{sf.normalVA(), sf.normalVA()}
        dur = 0.05 + (x[1][1] / 60)
        x = granmodule.L * u
    end

    
    freq = 2^(10 + x[2][1] * 2)
    amp = 1
    pan = 0.5

    return rate, dur, freq, amp, pan
end
return granmodule