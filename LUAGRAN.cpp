#include <stdio.h>
#include <stdlib.h>
#include <ugens.h>
#include <math.h>
#include <algorithm>
#include <PField.h>
#include <Instrument.h>
#include "LUAGRAN.h"
#include <rt.h>
#include <rtdefs.h>
#include <iostream>
#include <vector>
#include <string.h>

extern "C"{
#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "lua/lualib.h"
}

#define MAXGRAINS 1500

LUAGRAN::LUAGRAN() : branch(0), L(nullptr) 
{
}



LUAGRAN::~LUAGRAN()
{
	if (!configured)
		return;
	for (size_t i = 0; i < grains->size(); i++)
	{
		delete (*grains)[i];
	}
	delete grains;
	lua_close(L);
	delete pFieldVals;
}


int LUAGRAN::init(double p[], int n_args)
{

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
	if (rtsetoutput(p[0], p[1], this) == -1)
		return DONT_SCHEDULE;

	if (outputChannels() > 2)
	      return die("LUAGRAN", "Output must be mono or stereo.");

	if (n_args < 7)
		return die("LUAGRAN", "7 arguments are required");

	grainEnvLen = 0;
	wavetableLen = 0;
	amp = p[2];

	newGrainCounter = 0;

	// init tables
	wavetable = (double *) getPFieldTable(3, &wavetableLen);
	grainEnv = (double *) getPFieldTable(4, &grainEnvLen);
	grainLimit = p[5];

	if (grainLimit > MAXGRAINS)
	{
		rtcmix_advise("STGRAN2", "user provided max grains exceeds limit, lowering to 1500");
		grainLimit = MAXGRAINS;
	}
	else
		grainLimit = MAXGRAINS;

	L = luaL_newstate();
	luaL_openlibs(L);
	

	const PField &field = getPField(6);
   	const char *str = field.stringValue(0);
   	if (str == NULL)
		return die("LUAGRAN", "Invalid file name");

	luaL_dofile(L, str);
	lua_setglobal(L, "granmodule");
    lua_settop(L, 0);
	
	lua_getglobal(L, "granmodule");
	lua_getfield(L, -1, "init");
    lua_call(L, 0, 0);

	nPFields = n_args - 7;

	

	//std::cout << "setup finished" << "\n";
	return nSamps();
}



int LUAGRAN::configure()
{
	// make the needed grains, which have no values yet as they need to be set dynamically
	grains = new std::vector<Grain*>();
	// maybe make the maximum grain value a non-pfield enabled parameter

	for (int i = 0; i < grainLimit; i++)
	{
		grains->push_back(new Grain());
	}

	pFieldVals = new float[nPFields];

	configured = true;

	return 0;	// IMPORTANT: Return 0 on success, and -1 on failure.
}



// set new parameters and turn on an idle grain
void LUAGRAN::resetgrain(Grain* grain)
{
	//std::cout << "making grain" << "\n";
	lua_getglobal(L, "granmodule");
	lua_getfield(L, -1, "generate");
    lua_call(L, nPFields, 5);

	double rate = lua_tonumber(L, -5);
    double dur = lua_tonumber(L, -4);
	double freq = lua_tonumber(L, -3);
	double amp = lua_tonumber(L, -2);
	double pan = lua_tonumber(L, -1);
	lua_pop(L, 5);

	//printf("generated grain with rate %f dur %f freq %f amp %f pan %f\n", rate, dur, freq, amp, pan);
	float grainDurSamps = SR * dur;
	grain->waveSampInc = wavetableLen * freq / SR;
	grain->ampSampInc = ((float)grainEnvLen) / grainDurSamps;
	grain->currTime = 0;
	grain->isplaying = true;
	grain->wavePhase = 0;
	grain->ampPhase = 0;
	grain->amp = amp;
	grain->panR = pan;
	grain->panL = 1 - pan; // separating these in RAM means fewer sample rate calculations
	grain->dur = (int)round(grainDurSamps);
	//std::cout<<"sending grain with freq : " << freq << " dur : " << grain->dur << " panR " << panR << "\n"; */
	newGrainCounter = (int)round(SR * rate);
}


// update pfields
void LUAGRAN::doupdate()
{
	double p[20];
	update(p, 20);
	amp = (float) p[2];
	for (int i = 0; i < nPFields; i++){
		
	}

}


int LUAGRAN::run()
{
	
	float out[2];
	for (int i = 0; i < framesToRun(); i++) {
		if (--branch <= 0)
		{
		doupdate();
		branch = getSkip();
		}

		out[0] = 0;
		out[1] = 0;
		for (size_t j = 0; j < grains->size(); j++)
		{
			Grain* currGrain = (*grains)[j];
			if (currGrain->isplaying)
			{
				if (++(*currGrain).currTime > currGrain->dur)
				{
					currGrain->isplaying = false;
				}
				else
				{
					// should include an interpolation option at some point
					float grainAmp = currGrain->amp * oscili(1, currGrain->ampSampInc, grainEnv, grainEnvLen, &((*currGrain).ampPhase));
					float grainOut = oscili(grainAmp,currGrain->waveSampInc, wavetable, wavetableLen, &((*currGrain).wavePhase));
					out[0] += grainOut * currGrain->panL;
					out[1] += grainOut * currGrain->panR;
				}
			}
			// this is not an else statement so a grain can be potentially stopped and restarted on the same frame

			if ((newGrainCounter <= 0) && !currGrain->isplaying)
			{
				resetgrain(currGrain);

			}
		}

		// if all current grains are occupied, we skip this request for a new grain
		if (newGrainCounter <= 0)
		{
			newGrainCounter = 1;
		}

		out[0] *= amp;
		out[1] *= amp;
		rtaddout(out);
		newGrainCounter--;
		increment();
	}

	// Return the number of frames we processed.

	return framesToRun();
}


Instrument *makeLUAGRAN()
{
	LUAGRAN *inst = new LUAGRAN();
	inst->set_bus_config("LUAGRAN");

	return inst;
}

#ifndef EMBEDDED
void rtprofile()
{
	RT_INTRO("LUAGRAN", makeLUAGRAN);
}
#endif

