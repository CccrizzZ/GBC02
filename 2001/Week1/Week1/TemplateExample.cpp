#include <iostream>
using namespace std;

template <class T>
T min_func(T lVal, T rVal)
{
  if (lVal > rVal)
  {
    return rVal;
  }
  return lVal;
}


template <class T>
T max_func(T lVal, T rVal)
{
  if (lVal < rVal)
  {
    return rVal;
  }
  return lVal;
}


template <class T>
class TemplateClass
{
public:
  TemplateClass(T val)
  {
    m_val = val;
  }

  bool operator<(TemplateClass &rVal)
  {
    return m_val < rVal.GetVal();
  }

  bool operator>(TemplateClass &rVal)
  {
    return m_val > rVal.GetVal();
  }

  // returns the value
  T GetVal()
  {
    return m_val;
  }
private:
  T m_val;
};
