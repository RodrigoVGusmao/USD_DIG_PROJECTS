#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

#include<vector>

#include"object.h"

namespace degine
{
	namespace physics
	{
		class physics
		{
			public:
			physics(object::objects& objects) : objects(objects) {};
			virtual ~physics() {};
			virtual void physicsLoop(double) {};

			protected:
			object::objects& objects;

			private:
		};

		class physics2D : public physics
		{
			public:
			using physics::physics;
			virtual ~physics2D();
			virtual void physicsLoop(double frameTime);

			protected:
			object::speed2D globalSpeed;

			private:
		};
	}
}

#endif // PHYSICS_H_INCLUDED
