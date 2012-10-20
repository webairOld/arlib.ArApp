#ifndef AR_ENVIROMENT_DATA
#define AR_ENVIROMENT_DATA

#include <opencv2/core/core.hpp>
#include <GLES2/gl2.h>
#include <vector>
#include "arLib/model/Model.h"
#include "arLib/Definitions.h"


using namespace std;
using namespace glm;
using namespace cv;

class EnvironmentData {

public:
	EnvironmentData();
	~EnvironmentData();

	//gles2 helpers
	GLuint cameraTextrueRef;

	void setVerticalAngle(float angle);
	float getVerticalAngle();

	void setImageDimension(Dimension size);
	Dimension getImageDimension();
	float getImageRatio();
	int getImageRGBASize();

	//device states
	void setDeviceOrientation(Orientation orientation);
	Orientation getDeviceOrientation();

	vector<Model*>* getModels();
	mat4 getProjection();

private:
	float frustumNear;
	float frustumFar;
	float frustumDistanceRatio;
	float frustumAngle;

	mat4* baseProjection;

	//image stuff
	Dimension imageDimension;
	float imageRatio;
	int imageRGBASize;

	Orientation deviceOrientation;

	void createBaseProjection();

};
#endif