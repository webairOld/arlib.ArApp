#include "arLib/EnvironmentData.h"
#include "util/Logger.h"
#include <glm/gtx/euler_angles.hpp>

EnvironmentData::EnvironmentData()
{

}

void EnvironmentData::setImageDimension(Dimension dimension)
{
	imageDimension = dimension;
	imageRatio = imageDimension.width / imageDimension.height;
	imageRGBASize = (int)dimension.width * (int)dimension.height * 4;
}

Dimension EnvironmentData::getImageDimension()
{
	return imageDimension;
}

float EnvironmentData::getImageRatio()
{
	return imageRatio;
}

int EnvironmentData::getImageRGBASize()
{
	return imageRGBASize;
}

void EnvironmentData::setVerticalAngle(float angle) {
	frustumAngle = angle;
	// TODO set this on other place
	frustumNear = 1.0f;
	frustumFar = 100.0f;
	frustumDistanceRatio = tan((frustumAngle*3.141592f/180.0f) * 0.5f);
	this->createBaseProjection();
}

float EnvironmentData::getVerticalAngle()
{
	return frustumAngle;
}

void EnvironmentData::createBaseProjection()
{
	frustumNear = 1.0f;
	frustumFar = 100.0f;
	frustumDistanceRatio = tan((frustumAngle*3.141592f/180.0f) * 0.5f);

    LOGI("setup graphics:");
    LOGI("frustum near: %f", frustumNear);
    LOGI("frustum far: %f", frustumFar);
    LOGI("frustum angle: %f", frustumAngle);
    LOGI("frustum distance ratio: %f", frustumDistanceRatio);

    mat4 projection = perspective(this->getVerticalAngle(), this->getImageRatio(), frustumNear, frustumFar);
    baseProjection = new mat4(projection);
}

void EnvironmentData::setDeviceOrientation(Orientation orientation)
{
	deviceOrientation = orientation;
}

Orientation EnvironmentData::getDeviceOrientation()
{
	return deviceOrientation;
}

glm::mat4 EnvironmentData::getProjectionMatrix()
{
	return *this->baseProjection;
	//return proj;
}

glm::mat4 EnvironmentData::getViewMatrix()
{

	/*
	//vec4 lookAtTemp = vec4(0.0f,0.0f,1.0f,1.0f) * yawPitchRoll(
	//		deviceOrientation.azimuth,
	//		deviceOrientation.pitch,
	//		deviceOrientation.roll);
	//vec3 lookAt = vec3(lookAtTemp.x, lookAtTemp.y, lookAtTemp.z);
	vec3 lookAt = vec3(sin(-deviceOrientation.azimuth),tan(-deviceOrientation.pitch),cos(-deviceOrientation.azimuth));



	//headUp = headUp * rotate(mat4(), (-deviceOrientation.roll*180.0f)/3.14159265f, vec3(0.0f, 0.0f, 1.0f));
	//glm::vec3 lookAt =  vec3(0.0f,0.0f,1.0f) * glm::rotate( mat4(1.0f), -deviceOrientation.azimuth, vec3(0.0f,1.0f,0.0f));

	//float pitch = deviceOrientation.pitch;
	//float roll = deviceOrientation.roll;



	mat4 eulerRotationMatrix = glm::yawPitchRoll(
			deviceOrientation.azimuth,
			deviceOrientation.pitch,
			deviceOrientation.roll);
	*/
	//mat4 sensorRotation = mat4();
	//sensorRotation = rotate(sensorRotation, deviceOrientation.roll, glm::vec3(0.0f, 0.0f, 1.0f));
	//sensorRotation = rotate(sensorRotation, deviceOrientation.azimuth, vec3(0.0f, 1.0f, 0.0f));
	//sensorRotation = rotate(sensorRotation, -deviceOrientation.pitch, vec3(1.0f, 0.0f, 0.0f));

	//lookAt = lookAt*eulerRotationMatrix;
	//headUp = headUp*eulerRotationMatrix;
	vec3 camPos = vec3(0.0f, -2.0f, -2.0f);

	vec3 lookAt =  vec3( cos(deviceOrientation.pitch) * sin(deviceOrientation.azimuth),
						 cos(deviceOrientation.pitch),
					 sin(deviceOrientation.pitch) * sin(deviceOrientation.azimuth)
	);
	lookAt.x += camPos.x;
	lookAt.y += camPos.y;
	lookAt.z += camPos.z;

	//vec4 headUp = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	vec3 headUp = vec3(0.0f, 1.0f, 0.0f);
	//vec3 headUp = cross(normalize(lookAt), vec3(0.0f, 1.0f, 0.0f));
	//headUp = cross(headUp, lookAt);
	//vec4 tmp = vec4(headUp.x, headUp.y, headUp.z, 1.0f) * rotate(mat4(), -cos(deviceOrientation.azimuth) * ((deviceOrientation.roll*180.0f)/3.14159265f), vec3(0.0f, 0.0f, 1.0f));
	//headUp.x = tmp.x;
	//headUp.y = tmp.y;
	//headUp.z = tmp.z;

	//vec3 headUp = vec3(sin(deviceOrientation.roll),cos(deviceOrientation.roll),0.0f);
	/*
	headUp.x = cos(deviceOrientation.azimuth)*sin(deviceOrientation.pitch)*sin(deviceOrientation.roll) - sin(deviceOrientation.azimuth)*cos(deviceOrientation.roll);
	headUp.y = sin(deviceOrientation.azimuth)*sin(deviceOrientation.pitch)*sin(deviceOrientation.roll) + cos(deviceOrientation.azimuth)*sin(deviceOrientation.roll);
	headUp.z = cos(deviceOrientation.pitch)*sin(deviceOrientation.roll);
	*/
	//LOGI("up vector: %f, %f, %f", headUp.x, headUp.y, headUp.z);



	glm::mat4 view = glm::lookAt(
		camPos, // camera position
	    lookAt, // look at position
	    headUp
	    //glm::vec3(0.0f, 1.0f, 0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	//mat4 view = glm::yawPitchRoll(-deviceOrientation.azimuth, -deviceOrientation.pitch, -deviceOrientation.roll);





	//mat4 view = translate(mat4(1.0f),vec3(-5.0f, 0.0f, 0.0f));
	return view;
}
