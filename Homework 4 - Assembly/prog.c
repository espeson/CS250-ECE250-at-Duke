#include "stdio.h"
#include "stdlib.h"
int m_w = 123; /* must not be zero */
int m_z = 234; /* must not be zero */
int get_random() {
  m_z = 36969 * (m_z & 65535) + (m_z >> 16);
  m_w = 18000 * (m_w & 65535) + (m_w >> 16);
  return (m_z << 16) + m_w; /* 32-bit result */
}
