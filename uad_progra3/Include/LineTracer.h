#pragma once
#include <vector>
#include "CVector3.h"

namespace LineTracer
{
#define LINESTEP 0.25f
#define SPETAMOUNT 16
	inline static void LineSamples(CVector3 start, CVector3 dir, std::vector<CVector3> *output) {
		output->clear();

		float deltaX = dir.getX() * LINESTEP;
		float deltaY = dir.getY() * LINESTEP;
		float deltaZ = dir.getZ() * LINESTEP;
		float x = start.getX();
		float y = start.getY();
		float z = start.getZ();

		for (size_t i = 0; i < SPETAMOUNT; i++)
		{
			x += deltaX;
			y += deltaY;
			z += deltaZ;

			CVector3 res(-round(x), round(y), round(z));
			if (std::find(output->begin(), output->end(), res) == output->end()) {
				output->push_back(res);
			}

		}
	}
}
