/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#include "filteredsample.h"

FilteredSample::FilteredSample(int leadValue, int trailValue, int minValue, int maxValue,
                               unsigned int status)
  : m_lead_value(leadValue)
  , m_trail_value(trailValue)
  , m_min_value(minValue)
  , m_max_value(maxValue)
  , m_status(status)
{
}

FilteredSample::FilteredSample()
  : m_lead_value(0)
  , m_trail_value(0)
  , m_min_value(0)
  , m_max_value(0)
  , m_status(0)
{
}
