#include <Ougens.h>
#include <vector>

extern "C"{
#include "lua/lauxlib.h"
#include "lua/lua.h"
#include "lua/lualib.h"
}
		  // the base class for this instrument
typedef struct {
	float waveSampInc; 
	float ampSampInc; 
	float wavePhase; 
	float ampPhase;
	float amp;	
	int dur; 
	float panR; 
	float panL; 
	int currTime; 
	bool isplaying;
	} Grain;


class LUAGRAN : public Instrument {

public:
	LUAGRAN();
	virtual ~LUAGRAN();
	virtual int init(double *, int);
	virtual int configure();
	virtual int run();
	void addgrain();
	void resetgrain(Grain* grain);

private:
	bool configured;
	int branch;
	float amp;

	int nPFields;
	float* pFieldVals;
	lua_State *L;

	std::vector<Grain*>* grains;
	int grainLimit;
	int newGrainCounter;


	double* wavetable;
	int wavetableLen;
	double* grainEnv;
	int grainEnvLen;
	float grainRate;
	void doupdate();
};

