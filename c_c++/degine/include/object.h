#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include"vector_c.h"
#include"linkedList_c.h"

namespace degine
{
	namespace object
	{
		struct object
		{
			double x, y;
		};
		struct speed2D
		{
			double x, y;
		};

		class objects
		{
			public:
			//layeredList contains
			//programs(0) > meshList(1) > textureList(2) > objectList(3)
			void_v layeredList;
			//optimizedList contains only ordered objectList
			void_v optimizedList;
		};

		class GLObjects : public objects
		{
			public:
			std::vector<size_t> program;
		};

		class object
		{
			public:
			virtual ~object() {};

			void loop() {};
		};
	}
}

#endif // OBJECT_H_INCLUDED
