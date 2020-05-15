/**********************************************************************************************************************
 * (C) Copyright 2020 Integrated Computer Solutions, Inc. - All rights reserved.
 **********************************************************************************************************************/

#ifndef FILTEREDSAMPLE_H
#define FILTEREDSAMPLE_H

class FilteredSample
{
public:
    explicit FilteredSample(int leadValue, int trailValue, int minValue, int maxValue,
                            unsigned int status);
    FilteredSample();

    int leadValue() const { return m_lead_value; }
    int trailValue() const { return m_trail_value; }
    int minValue() const { return m_min_value; }
    int maxValue() const { return m_max_value; }

    unsigned int status() const { return m_status; }

private:
    int m_lead_value;
    int m_trail_value;
    int m_min_value;
    int m_max_value;

    unsigned int m_status;
};

#endif // FILTEREDSAMPLE_H
