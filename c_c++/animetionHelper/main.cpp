#include<iostream>
#include<fstream>
#include<vector>

#include<math.h>

const uint8_t headerMagicNumber[] = "AHGL";

typedef struct
{
    float timeSec;
    float angleVel;
    float angleX, angleY, angleZ;
    float origX, origY, origZ;
    float transVel;
    float transX, transY, transZ;
} oldKeyFrame;

typedef struct
{
	float timeSec, radius;
    float angleX, angleY, angleZ;
    float transX, transY, transZ;
    float transAngX, transAngY, transAngZ;
    float reserved;
} newKeyFrame;

typedef struct {
	uint32_t header;
	uint32_t objectCount;
	uint32_t objectAnimOffset;
	uint32_t reserved;
} mainHeader;

typedef struct {
	uint32_t offset;
	uint32_t size;
	int32_t index;
	float angleX, angleY, angleZ;
    float transAngleX, transAngleY, transAngleZ;
    float transX, transY, transZ;
}subHeader;

void transformToTime(oldKeyFrame* okf, size_t OKFSize, std::vector<newKeyFrame>& nkf, std::vector<subHeader>& sh, size_t count, float timeWait)
{
	nkf.reserve((count*sizeof(newKeyFrame)*(2*OKFSize)));

	//OKFSize *= 2;
	size_t offset = sizeof(mainHeader) + count*sizeof(subHeader);
	for(size_t i = 0; i < count; ++i)
	{
		size_t j;
		if(i != 52)
		{
			nkf.push_back({timeWait*i});

			size_t j = 0;
			for(; j < 2; ++j)
			{
				double r = sqrt(okf[j].origX*okf[j].origX + okf[j].origY*okf[j].origY + okf[j].origZ*okf[j].origZ);

				newKeyFrame newKF = {0};

				newKF.timeSec = okf[j].timeSec;
				newKF.radius = r;
				newKF.angleX = okf[j].angleX*okf[j].angleVel;
				newKF.angleY = okf[j].angleY*okf[j].angleVel;
				newKF.angleZ = okf[j].angleZ*okf[j].angleVel;
				newKF.transX = okf[j].transX*okf[j].transVel;
				newKF.transY = okf[j].transY*okf[j].transVel;
				newKF.transZ = okf[j].transZ*okf[j].transVel;
				newKF.transAngX = okf[j].origX;
				newKF.transAngY = okf[j].origY;
				newKF.transAngZ = okf[j].origZ;

				nkf.push_back(newKF);
			}
			nkf.push_back({timeWait*(51-i)});

			for(; j < 3; ++j)
			{
				double r = sqrt(okf[j].origX*okf[j].origX + okf[j].origY*okf[j].origY + okf[j].origZ*okf[j].origZ);

				newKeyFrame newKF = {0};

				newKF.timeSec = okf[j].timeSec;
				newKF.radius = r;
				newKF.angleX = okf[j].angleX*okf[j].angleVel;
				newKF.angleY = okf[j].angleY*okf[j].angleVel;
				newKF.angleZ = okf[j].angleZ*okf[j].angleVel;
				newKF.transX = okf[j].transX*okf[j].transVel;
				newKF.transY = okf[j].transY*okf[j].transVel;
				newKF.transZ = okf[j].transZ*okf[j].transVel;
				newKF.transAngX = okf[j].origX;
				newKF.transAngY = okf[j].origY;
				newKF.transAngZ = okf[j].origZ+0.023f*i;
				nkf.push_back(newKF);
			}

			for(; j < 4; ++j)
			{
				double r = sqrt(okf[j].origX*okf[j].origX + okf[j].origY*okf[j].origY + okf[j].origZ*okf[j].origZ);

				newKeyFrame newKF = {0};
				newKF.timeSec = timeWait*(51-i);
				newKF.transAngX = okf[j].origX;
				newKF.transAngY = okf[j].origY;
				newKF.transAngZ = okf[j].origZ+0.023f*i;
				nkf.push_back(newKF);

				newKF.timeSec = okf[j].timeSec-timeWait*(51-i);
				newKF.radius = r;
				newKF.angleX = okf[j].angleX*okf[j].angleVel;
				newKF.angleY = okf[j].angleY*okf[j].angleVel;
				newKF.angleZ = okf[j].angleZ*okf[j].angleVel;
				newKF.transX = okf[j].transX*okf[j].transVel;
				newKF.transY = okf[j].transY*okf[j].transVel;
				newKF.transZ = okf[j].transZ*okf[j].transVel;
				newKF.transAngX = okf[j].origX;
				newKF.transAngY = okf[j].origY;
				newKF.transAngZ = okf[j].origZ+0.023f*i;
				nkf.push_back(newKF);
			}
			if(i == 51)
			{
				for(; j < 8; ++j)
				{
					double r = sqrt(okf[j].origX*okf[j].origX + okf[j].origY*okf[j].origY + okf[j].origZ*okf[j].origZ);

					newKeyFrame newKF = {0};

					newKF.timeSec = okf[j].timeSec;
					newKF.radius = r;
					newKF.angleX = okf[j].angleX*okf[j].angleVel*okf[j].timeSec;
					newKF.angleY = okf[j].angleY*okf[j].angleVel*okf[j].timeSec;
					newKF.angleZ = okf[j].angleZ*okf[j].angleVel*okf[j].timeSec;
					newKF.transX = okf[j].transX*okf[j].transVel*okf[j].timeSec;
					newKF.transY = okf[j].transY*okf[j].transVel*okf[j].timeSec;
					newKF.transZ = okf[j].transZ*okf[j].transVel*okf[j].timeSec;
					newKF.transAngX = okf[j].origX;
					newKF.transAngY = okf[j].origY+(i == 6 ? +0.023f*i: 0);
					newKF.transAngZ = okf[j].origZ+0.023f*i;
					nkf.push_back(newKF);
				}
			}
			sh.push_back({offset, (j+2+1)});
			offset += (j+2+1)*sizeof(newKeyFrame);
		}
		else
		{
			j = 8;
			for(; j < 13; ++j)
			{
				double r = sqrt(okf[j].origX*okf[j].origX + okf[j].origY*okf[j].origY + okf[j].origZ*okf[j].origZ);

				newKeyFrame newKF = {0};

				newKF.timeSec = okf[j].timeSec;
				newKF.radius = r;
				newKF.angleX = okf[j].angleX*okf[j].angleVel;
				newKF.angleY = okf[j].angleY*okf[j].angleVel;
				newKF.angleZ = okf[j].angleZ*okf[j].angleVel;
				newKF.transX = okf[j].transX*okf[j].transVel;
				newKF.transY = okf[j].transY*okf[j].transVel;
				newKF.transZ = okf[j].transZ*okf[j].transVel;
				newKF.transAngX = okf[j].origX;
				newKF.transAngY = okf[j].origY;
				newKF.transAngZ = okf[j].origZ;
				nkf.push_back(newKF);
			}
			j-=8;
			sh.push_back({offset, (j)});
			offset += (j)*sizeof(newKeyFrame);
		}
	}
}

float timeSec;
    float angleVel;
    float angleX, angleY, angleZ;
    float transVel;
    float transX, transY, transZ;

int main(int argc, char* argv[])
{
	mainHeader mh = {*(uint32_t*)headerMagicNumber, 53, sizeof(mainHeader), 0};
	std::vector<subHeader> sh;
	std::vector<newKeyFrame> nkf;
	std::ofstream file("anim.ah", std::ios::binary);

	file.write(reinterpret_cast<char*>(&mh), sizeof(mainHeader));

	oldKeyFrame keys[] = {	{360.f/90.f,	90.f,		0.f,	0.f,	1.0f,	0.f,	15.f,			0.f,	0.f,		0.f,	0.f,	0.f},
							{1.f,			0.0f,		0.0f,	0.f,	0.0f,	0.f,	0.f,			0.f,	0.f,		0.f,	0.f,	0.f},
							//{360.f/90.f,	-90.f,		0.f,	0.f,	1.0f,	0.f,	15.f,			0.f,	0.f,		0.f,	0.f,	0.f},
							{2.f,			-90.f/2.f,	1.f,	0.f,	0.0f,	0.f,	9.4f/2.f,		0.f,	0.f,		0.f,	0.f,	0.f},
							{4.f,			0.f,		1.f,	0.f,	0.0f,	0.f,	9.4f/2.f,		0.f,	10.f,		1.f,	0.f,	0.f},
							{0.8f,			0.f,		0.f,	0.f,	0.0f,	0.f,	9.4f/2.f,		0.f,	10.f,		1.f,	0.f,	0.f},
							{1.f,			-180.f,		0.0f,	1.f,	0.0f,	0.f,	-5.f+9.4f/2.f,	0.f,	5.f,		-1.f,	0.f,	0.f},
							{0.8f,			0.f,		0.f,	1.f,	0.0f,	0.f,	-5.f+9.4f/2.f,	0.f,	-10.f,		1.f,	-0.5f,	0.f},
							//{0.01f,			0.f,		0.f,	1.f,	0.0f,	0.f,	0.f,			0.f,	(-5.f+9.4f/2.f)*100,		0.f,	1.f,	0.f},
							{0.0f,			0.f,		0.f,	1.f,	0.0f,	0.f,	-5.f+9.4f/2.f,			0.f,	0.f,		0.f,	0.f,	0.f},

							//camera
							{2.f,			15.f,		0.f,	0.f,	1.f,	0.f,	0.f,			0.f,	-20.f,		0.f,	0.75f,		1.f},
							{2.f,			0.f,		0.f,	0.f,	.0f,	0.f,	0.f,			0.f,	0.f,		0.f,	0.f,		0.f},
							{2.f,			-15.f,		0.f,	0.f,	1.f,	0.f,	0.f,			0.f,	20.f,		1.f,	1.5f,		1.f},
							{4.f,			0.f,		0.f,	0.f,	0.f,	0.f,	0.f,			0.f,	5.f,		1.f,	0.f,		0.f},
							{0.f,			0.f,		0.f,	0.f,	0.f,	0.f,	0.f,			0.f,	0.f,		0.f,	0.f,		0.f},};

	/*oldKeyFrame keys[] = {	{0.8f,			0.f,		0.f,	0.f,	0.0f,	0.f,	0.f,	0.f,	10.f,		1.f,	0.f,	0.f},
							{1.f,			180.f,		0.0f,	1.f,	0.0f,	5.f,	5.f,	0.f,	0.f,		-5.f,	-5.f,	0.f},
							{0.8f,			0.f,		0.f,	0.f,	0.0f,	0.f,	0.f,	0.f,	-10.f,		1.f,	0.f,	0.f},
							/*{360.f/90.f,	-90.f,		0.f,	0.f,	1.0f,	0.f,	15.f,	0.f,	0.f,		0.f,	0.f,	0.f},
							{2.f,			-90.f/2.f,	1.f,	0.f,	0.0f,	0.f,	9.4f/2,	0.f,	0.f,		0.f,	0.f,	0.f},
							{4.f,			0.f,		1.f,	0.f,	0.0f,	0.f,	0.f,	0.f,	20.f,		1.f,	0.f,	0.f},*/
							/*{0.f,			0.0f,		0.0f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f,		0.f,	0.f,	0.f},*/
								//};

	/*oldKeyFrame anim[] = {	{4.f,	0.f,	0.f,	0.f,	0.0f,	0.f,	0.f,	0.f,	20.f,	-1.f,	0.f,	0.f},
							//{4.f,	0.f,	0.f,	0.f,	0.0f,	0.f,	0.f,	0.f,	20.f,	-1.f,	0.f,	0.f},
	}*/

	transformToTime(keys, sizeof(keys)/sizeof(oldKeyFrame), nkf, sh, 53, 4.f/51.f);
	/*for(size_t i = 0; i < 52; ++i)
	{
		sh.push_back({sizeof(mainHeader) + 52*sizeof(subHeader) + i*(sizeof(keys)/sizeof(oldKeyFrame)*2)*sizeof(newKeyFrame), (sizeof(keys)/sizeof(oldKeyFrame)*2)});
	}*/

	file.write(reinterpret_cast<char*>(sh.data()), sh.size()*sizeof(subHeader));
	file.write(reinterpret_cast<char*>(nkf.data()), nkf.size()*sizeof(newKeyFrame));

	return 0;
}
