#include"physics.h"

degine::physics::physics2D::~physics2D() {}

void degine::physics::physics2D::physicsLoop(double frameTime)
{
	for(size_t i = 0; i < objects.objects.size(); ++i)
		for(size_t j = 0; j < objects.objects[i].size(); ++j)
			for(size_t k = 0; k < objects.objects[i][j].size(); ++k)
			{
				objects.objects[i][j]
				objects.objects[i][j].position[k].x +=
					(objects.objects[i][j].speed[k].x+globalSpeed.x)*frameTime;
				objects.objects[i][j].position[k].y +=
					(objects.objects[i][j].speed[k].y+globalSpeed.y)*frameTime;
			}
}
