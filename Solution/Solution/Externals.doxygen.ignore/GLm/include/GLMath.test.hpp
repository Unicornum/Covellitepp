
#pragma once

// ���� ��� ������������� � �������� ����������� (����������� ��-�� ����, ���
// ��� ����������� ��������� �������� ::glm::mat4::operator==() ���������� true
// ��� ������ ������).

#pragma warning(push)
#pragma warning(disable: 4127)
#pragma warning(disable: 4201)
#pragma warning(disable: 4324)

#define GLM_FORCE_DEPTH_ZERO_TO_ONE // z == [0...1]
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#pragma warning(pop)
