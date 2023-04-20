-- basic stochastic example

local granmodule = {}
granmodule.state = {}

function granmodule.init()
end

function granmodule.generate()
    rate = randrange(0.00001, 0.005)
    dur = randrange(0.0001, 0.05)
    freq = octfreq(randrange(7, 14))
    amp = randrange(0.01, 1)
    pan = math.random()

    return rate, dur, freq, amp, pan
end

function granmodule.update(...)
    -- receives updates from pFields as args
end

function randrange(m, n)
    return math.random() * (n - m) + m
end

function octfreq(linocts)
    return 2^linocts
end

return granmodule

