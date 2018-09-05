#ifndef __GRAPHICS4_SCENELOADER__
#define __GRAPHICS4_SCENELOADER__

#include "tracer/Tracer.h"

class SceneLoader
{
	Tracer* tracer;
public:
	SceneLoader(Tracer*);
	void LoadScene(int) const;
};

#endif
