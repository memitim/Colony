#include <windows.h>
#include "..\headers\timer_struct.h"

bool openDisplay(int width, int height);
void updateDisplay();
void closeDisplay();
void onQuit();
float time();
double PCFreq = 0.0;
__int64 CounterStart = 0;
float t = 0.0f;
float dt = 0.1f;
float currentTime = 0.0f;
float accumulator = 0.0f;
bool quit = false;

void StartCounter()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
        //cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart);

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
}

void onQuit()
{
	quit = true;
}

State interpolate(const State &previous, const State &current, float alpha)
{
	State state;
	state.x = current.x*alpha + previous.x*(1-alpha);
	state.v = current.v*alpha + previous.v*(1-alpha);
	return state;
}

float acceleration(const State &state, float t)
{
	const float k = 10;
	const float b = 1;
	return - k*state.x - b*state.v;
}

Derivative evaluate(const State &initial, float t)
{
	Derivative output;
	output.dx = initial.v;
	output.dv = acceleration(initial, t);
	return output;
}

Derivative evaluate(const State &initial, float t, float dt, const Derivative &d)
{
	State state;
	state.x = initial.x + d.dx*dt;
	state.v = initial.v + d.dv*dt;
	Derivative output;
	output.dx = state.v;
	output.dv = acceleration(state, t+dt);
	return output;
}

void integrate(State &state, float t, float dt)
{
	Derivative a = evaluate(state, t);
	Derivative b = evaluate(state, t, dt*0.5f, a);
	Derivative c = evaluate(state, t, dt*0.5f, b);
	Derivative d = evaluate(state, t, dt, c);
	
	const float dxdt = 1.0f/6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
	const float dvdt = 1.0f/6.0f * (a.dv + 2.0f*(b.dv + c.dv) + d.dv);
	
	state.x = state.x + dxdt*dt;
	state.v = state.v + dvdt*dt;
}

void runTimeStep() {

//Interpolated timing mechanism flagrantly stolen from
//http://gafferongames.com/game-physics/fix-your-timestep/
		
	//Prepping timestep
	State current;
	current.x = 100;
	current.v = 0;
	
	State previous = current;

	const float newTime = GetCounter();
	float deltaTime = newTime - currentTime;
	currentTime = newTime;
		
	if (deltaTime>0.25f)
		deltaTime = 0.25f;
		
	accumulator += deltaTime;
		
	while (accumulator>=dt)
	{
		accumulator -= dt;
		previous = current;
		integrate(current, t, dt);
		t += dt;
	}
		
	State state = interpolate(previous, current, accumulator/dt);

}