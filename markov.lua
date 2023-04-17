-- random walk controlling frequency

local granmodule = {}
granmodule.state = {}

function granmodule.init()
    granmodule.state.lastlinoct = 8
end

function granmodule.generate(...)
    rate = randrange(0.0001, 0.0005)
    dur = randrange(0.0001, 0.05)

    freqmin = math.max(6, granmodule.state.lastlinoct - 0.2)
    freqmax = math.min(15, granmodule.state.lastlinoct + 0.2)
    linoct = randrange(freqmin, freqmax)
    granmodule.state.lastlinoct = linoct
    freq = octfreq(linoct)

    amp = randrange(0.01, .1)
    pan = math.random()

    return rate, dur, freq, amp, pan
end

function randrange(m, n)
    return math.random() * (n - m) + m
end

function octfreq(linocts)
    return 2^linocts
end

return granmodule

