
/**
* \page CovelliteApiInstancing ����������
*  
*  ����������
*  ==========
*  
*  ���������� ��������� ������������ ������� ���������� (����� ����� � ��������
*  � ����������� �� ������������ ������������) ���������� �������� (���� � �� ��
*  ������� + �������� + ���������� + ��������� ������) � ������ ���������� ��
*  ���� ����� ���������.
*  
*  ���� ����������� � ������������ ������ ������ ����������� ������ ������� �
*  ������������ ������������ ������, ����������� ���������� ������ �������
*  ����������� �������.
*  
*  �������������
*  -------------
*  
* ### ���������� ������
*  
*  ���������� ����������� ������� �������� �������� \b instance �� ���������,
*  ����������� ��������� ������ �������-������. ��������� - ������ ����
*  'f4f4i4', ��� \b f4 ������������� ���� float4, � \b i4 - int4
*  (�������������� ������ ��� ����).
*  
*  � ���������� ��������� \b Vertex ������� ������ ����������� ������� �����
*  ��������� �������������� ���������� � ��� ����� ��������� ��������� �������:
*  
* \code
struct Vertex
{
  float4 Position;
  float2 TexCoord;
  float4 Extra;
  float4 iValue1;
  float4 iValue2;
  int4   iValue3;
};
* \endcode
*  
* \note
*  ������� ����������� ������� ����� ������� ������� ���, �������
*  �������������� ��������, ��� ���� ������ iValue1-3 ��� ������ ������
*  ����� ��������������� ���������� �������, ������� ������� �������
*  ������� ������ ��� ��� ��������� ������ �������.
*  
* ### ������ ����������
*  
*  ���������� ���������� ����������� �������, ����������� ������, �������,
*  ����� ������������� (��������, Transform.Billboard ��� ������) �������
*  �������.
*  
* ### Present
*  
*  ���������� Present ������ ���������� ������ �������� ��������� Data.Buffer
*  �������-������, ������� ����� ��������� ���������� ������ ������� �������.
*  
* \code
  
// ��������� ������ ������ �������, ��������������� ��������� instance
// ����������� �������
class Instance
{
public:
  float x, y, z, w;       // iValue1
  float r, g, b, a;       // iValue2
  int32_t i1, i2, i3, i4; // iValue3
};
  
// ���������� ��������, ������� ����� ����������
const ::std::size_t InstanceCount = 100000;
  
using cbBufferMapper_t = ::std::function<bool(void *)>;
  
// ������� ��������� ������, ������� ����� ���������� � ������ ����� �����
// ����������
const cbBufferMapper_t Mapper = [=](void * _pData)
{
  // ������ ������������� ���������� ������; ������� false, ���� ����������
  // �������-������ � ���� ����� �� ���������
  if (_pData == nullptr) return true;
    
  auto * Data = reinterpret_cast<Instance *>(_pData);
    
  for (int i = 0; i < InstanceCount; i++)
  {
    // ��������� �������-�����
    Data[i].x = ...;
    ...
  }
    
  return false;
};
  
// ����� ����������� ��� ��������� � �������������� �����������
  
Component_t::Make( // �������-�����
{
  { uT("type"), uT("Data") },
  { uT("kind"), uT("Buffer") },
  { uT("mapper"), Mapper },
  { uT("count"), InstanceCount },
  { uT("size"), InstanceCount * sizeof(Instance) },
}),
Component_t::Make( // ��������� �����
{
  { uT("type"), uT("Data") },
  { uT("kind"), uT("Buffer") },
  { uT("content"), Indices },
}),
Component_t::Make( // Present.Instance
{
  { uT("id"), uT("Present.UniqueId") },
  { uT("type"), uT("Present") },
  { uT("kind"), uT("Instance") },
})
* \endcode
*  
* \note
*  - ��� ������ ������ Mapper'a �������-����� ������� ��������� �������,
*  � ���������� ��� ����� ��������� ��������.
*  - �������� InstanceCount ������ ���� ������ 16.
*  
*  ���� ���������� �������������� �������� ���������� �������� � ��������
*  ���������� (������ � ������� �������), �� ������� ��������� ������ ������
*  ��������� ��������� �������:
* \code
  
using cbBufferMapper_t = ::std::function<bool(void *, ::std::size_t &)>;
  
const cbBufferMapper_t Mapper = [=](void * _pData, ::std::size_t & _Count)
{
  // ������ ������������� ���������� ������; ������� false, ���� ����������
  // �������-������ � ���� ����� �� ���������
  if (_pData == nullptr) return true;
    
  auto * Data = reinterpret_cast<Instance *>(_pData);
  _Count = 0;
    
  for (int i = 0; i < InstanceCount; i++)
  {
    // ��������� �������-�����
    ...

    _Count++;
    if (...) break;
  }

  // ���� _Count ����� ��������� ��������, �������, ��� InstanceCount, �� ���
  // ���������� ����� ������������ �������� InstanceCount.
    
  return false;
};
* \endcode
*/
