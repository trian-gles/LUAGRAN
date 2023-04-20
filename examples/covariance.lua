local granmodule = {}
granmodule.state = {}
local matrix = require "./lib/matrix"
local sf = require "./lib/LuaSF"

function cholesky(cov)
    return matrix{{1, 0},{cov, (1 - cov^2)^0.5}}
end

function granmodule.init()
    print("Sup")
    
end

function granmodule.generate()
    local u = matrix{sf.normalVA(), sf.normalVA()}

    local x = granmodule.L * u

    rate = .01
    dur = 0.05 + (x[1][1] / 20)
    while dur <= 0 
    do
        u = matrix{sf.normalVA(), sf.normalVA()}
        dur = 0.05 + (x[1][1] / 60)
        x = granmodule.L * u
    end

    
    freq = 2^(10 + x[2][1])
    amp = 1
    pan = math.random()

    return rate, dur, freq, amp, pan
end

-- takes the 
function granmodule.update(cov)
    granmodule.L = cholesky(cov)
end



return granmodule

