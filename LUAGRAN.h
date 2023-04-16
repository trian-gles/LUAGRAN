#include <Ougens.h>
#include <vector>
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

	double x1;
	double x2;

	double lastA;

	double lastB;

	double minRate;
	double maxRate;
	double minDur;
	double maxDur;
	double minFreq;
	double maxFreq;
	double minAmp;
	double maxAmp;
	double minPan;
	double maxPan;

	double lastAmp;
	double lastRate;
	double lastDur;
	double lastFreq;
	double lastPan;

	float amp;

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

