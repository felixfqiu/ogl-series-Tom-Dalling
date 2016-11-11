#pragma once
#include <glm/glm.hpp>

namespace tdogl
{

	class Camera
	{
	public:
		Camera();

		const glm::vec3& pos() const;
		void setPos(const glm::vec3& pos);

		glm::mat4 orientation() const;
		void lookAt(glm::vec3 target);
		void offsetOrientation(float upAngle, float rightAngle);

		static void normalizeAngles(float& horAngle, float& verAngle, float verAngleMax);

		float fieldOfView() const;
		void setFieldOfView(float fieldOfView);

		float nearPlane() const;
		float farPlane() const;
		void setNearAndFarPlane(float nearPlane, float farPlane);

		float viewportAspectRatio() const;
		void setViewportAspectRatio(float viewportAspectRatio);

		glm::mat4 matrix() const;

		glm::mat4 projection() const;

		glm::mat4 view() const;

		// camera move pos
		void move(const glm::vec3& offset);

		// camera direction
		glm::vec3 directionFacing() const;
		glm::vec3 directionRight () const;
		glm::vec3 directionUp    () const;

	private:
		glm::vec3 _pos;                  // position
		float     _horAngle, _verAngle;  // orientation
		float     _fieldOfView;          // zoom
		float     _nearPlane, _farPlane; // max/min viewing distance
		float     _viewportAspectRatio;  // aspect ratio of viewport/window
	};

}