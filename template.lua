-- basic template for usage

local granmodule = {}
granmodule.state = {}

function granmodule.init()
    print("Running init method")
    -- setup initial values for the state at the start
end

function granmodule.generate(...) -- receives external state control as args
    
    -- create parameters for a grain and modify state if needed
    rate = .01
    dur = 0.01
    freq = 440
    amp = 1
    pan = 0.5

    return rate, dur, freq, amp, pan
end
return granmodule