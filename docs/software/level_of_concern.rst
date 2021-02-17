.. level_of_concern:

Level of Concern
================

Purpose
-------

This document states and justifies our Level of Concern for the software within |PROJECT_NAME|.

[[FDA-CPSSCMD:level-of-concern]]

Scope
-----

This document applies to |PROJECT_NAME| release |PROJECT_VERSION|.

Level of Concern
----------------

The Level of Concern for the software system is class |SW_CLASSIFICATION|.

Justification for Stated Level of Concern
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. todo:: Describe how you arrived at the stated Level of Concern.  Below is a template for a device that has a minor Level of Concern, hence the "No" response for all of the questions. Be sure to read the questions and update the responses as appropriate.

See the 2005 "Guidance for the Content of Premarket Submissions for Software Contained in Medical Devices" for details.

We believe the software system inside {{ system.project_name }} has a {{ system.level_of_concern }} Level of Concern because ...

Role of the Software
^^^^^^^^^^^^^^^^^^^^

.. todo:: Describe the role of the software in causing, controlling, and/or mitigating hazards that could result in injury to the patient or the operator.

Major Level of Concern Key Questions
""""""""""""""""""""""""""""""""""""

When evaluating our Software Device's Level of Concern, we considered the Key Questions provided in [1].  Our answers to these key questions for Major Level of Concern Devices are presented below:

**1. Does the Software Device qualify as Blood Establishment Computer Software?**

No.

**2. Is the Software Device intended to be used in combination with a drug or biologic?**

No.

**3. Is the Software Device an accessory to a medical device that has a Major Level of Concern?**

Yes.

**4. Prior to mitigation of hazards, could a failure of the Software Device result in death or serious injury, either to a patient or to a user of the device?**

Yes.

**a. Does the Software Device control a life supporting or life sustaining function?**

Yes.

**b. Does the Software Device control the delivery of potentially harmful energy that could result in death or serious injury, such as radiation treatment systems, defibrillators, and ablation generators?**

No.

**c. Does the Software Device control the delivery of treatment or therapy such that an error or malfunction could result in death or serious injury?**

Yes.

**d. Does the Software Device provide diagnostic information that directly drives a decision regarding treatment or therapy, such that if misapplied it could result in serious injury or death?**

No.

**e. Does the Software Device provide vital signs monitoring and alarms for potentially life threatening situations in which medical intervention is necessary?**

Yes.

Moderate Level of Concern Key Questions
"""""""""""""""""""""""""""""""""""""""

When evaluating our Software Device's Level of Concern, we considered the Key Questions provided in [1].  Our answers to these key questions for Moderate Level of Concern Devices are presented below:

**1. Is the Software Device an accessory to a medical device that has a Moderate Level of Concern?**

No.

**2. Prior to mitigation of hazards, could a failure of the Software Device result in Minor Injury, either to a patient or to a user of the device?**

Yes.

**3. Could a malfunction of, or a latent design flaw in, the Software Device lead to an erroneous diagnosis or a delay in delivery of appropriate medical care that would likely lead to Minor Injury? **

No.

References
----------

[1]: "Guidance for the Content of Premarket Submissions for Software Contained in Medical Devices", 2005.
