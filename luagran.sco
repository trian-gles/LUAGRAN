rtsetparams(44100, 2)
load("./libLUAGRAN.so")

/* Args:
		p0: outskip
		p1: dur
		p2: amp*
		p3: wavetable**
		p4: grainEnv**
		p5: grainLimit (recommended:1500)
		p6: script_name
		p7-: optional user values*

		* may recieve pfield values
		** must be passed pfield maketables.
*/

outskip = 0
dur = 10
amp = maketable("line", 1000, 0, 0, 1, 1, 16, 1, 17, 0)

wave = maketable("wave", 1000, "sine")
env = maketable("window", 1000, "hanning")

grainlimit = 1500
script_name = "ex1.lua"

LUAGRAN(outskip, dur, amp * 10000, wave, env, grainlimit, script_name)