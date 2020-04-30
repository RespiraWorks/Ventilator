{% extends "base/level_of_concern.md" %}

{% block level_of_concern %}
# Level of Concern

The Level of Concern for the software system within {{ system.project_name }} is **{{ system.level_of_concern }}**.

# Justification for Stated Level of Concern

See the 2005 "Guidance for the Content of Premarket Submissions for Software Contained in Medical Devices" for details.

To determine the level of concern the ventilator software was analyzed per the guidance document for level of concern.  In addition, other FDA cleared devices were checked for level of concern. We believe the software system inside {{ system.project_name }} has a {{ system.project_name }} Level of Concern because the role of the software is a life sustaining device.  Failure to provide therapy or correctly measure patient vitals could result in a permanent injury of death to the patient.  The following sections outline the analysis.

## Role of the Software

The role of the software is to provide ventillation therapy.  This therapy is life sustaining for patients who are unable to breath voluntarily.  In addition the software provide vital indications to the clinician including patient inspiratory pressure, tidal volume, and apnea time.  These metrics are used to make decisions for treatment.

## Major Level of Concern Key Questions

When evaluating our Software Device's Level of Concern, we considered the Key Questions provided in [1].  Our answers to these key questions for Major Level of Concern Devices are presented below:

**1. Does the Software Device qualify as Blood Establishment Computer Software?**

No.

**2. Is the Software Device intended to be used in combination with a drug or biologic?**

No.

**3. Is the Software Device an accessory to a medical device that has a Major Level of Concern?**

No.

**4. Prior to mitigation of hazards, could a failure of the Software Device result in death or serious injury, either to a patient or to a user of the device?**

Yes.

**a. Does the Software Device control a life supporting or life sustaining function?**

Yes

**b. Does the Software Device control the delivery of potentially harmful energy that could result in death or serious injury, such as radiation treatment systems, defibrillators, and ablation generators?**

No.

**c. Does the Software Device control the delivery of treatment or therapy such that an error or malfunction could result in death or serious injury?**

Yes

**d. Does the Software Device provide diagnostic information that directly drives a decision regarding treatment or therapy, such that if misapplied it could result in serious injury or death?**

Yes

**e. Does the Software Device provide vital signs monitoring and alarms for potentially life threatening situations in which medical intervention is necessary?**

Yes

## Moderate Level of Concern Key Questions

When evaluating our Software Device's Level of Concern, we considered the Key Questions provided in [1].  Our answers to these key questions for Moderate Level of Concern Devices are presented below:

**1. Is the Software Device an accessory to a medical device that has a Moderate Level of Concern?**

No.

**2. Prior to mitigation of hazards, could a failure of the Software Device result in Minor Injury, either to a patient or to a user of the device?**

No.

**3. Could a malfunction of, or a latent design flaw in, the Software Device lead to an erroneous diagnosis or a delay in delivery of appropriate medical care that would likely lead to Minor Injury? **

No.

# References

[1]: "Guidance for the Content of Premarket Submissions for Software Contained in Medical Devices", 2005.

{% endblock %}
