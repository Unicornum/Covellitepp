
#pragma once

using float4 = ::glm::vec4;

#define COVELLITE_MAX_LIGHT_POINT_OBJECT_COUNT 8

struct Ambient_t
{
  float4 Color;
  int IsValid, align1, align2, align3;
};

struct Direction_t
{
  float4 Color;
  float4 Direction;
  int IsValid, align1, align2, align3;
};

struct Point_t
{
  float4 Color;
  float4 Position;
  float4 Attenuation; // Const, Linear, Exponent, Radius;
};

struct Points_t
{
  Point_t Lights[COVELLITE_MAX_LIGHT_POINT_OBJECT_COUNT];
  int     UsedSlotCount;
};

struct Lights_t
{
  Ambient_t   Ambient;
  Direction_t Direction;
  Points_t    Points;
};
