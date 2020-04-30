---
category: SDS
id: SDS-001
revision: 1
title: Software Design Specification
manufacturer_name: MANUFACTURER
---

# 1 Purpose

The purpose of this document is to describe *how* the RespiraWorks Ventillator Software v1.0.0 software shall fulfill the software requirements.  It discusses the environment that that software will run in, the software system's architecture, functional specifications associated with each software requirement, and user interface mockups.

This document is written primarily for software and hardware engineers working on RespiraWorks Ventillator Software, who have the source code available in addition to this document.

# 2 Scope

The scope of this SDS applies in its entirety to the RespiraWorks Ventillator Software v1.0.0 product.

# 3 Definitions

The **Food and Drug Administration (FDA)** is a United State government agency responsible for protecting the public health by ensuring the safety, efficacy, and security of human and veterinary drugs, biological products, and medical devices.

The **Health Insurance Portability and Accountability Act** (HIPAA) is a United States law designed to provide privacy standards to protect patients' medical records and other health information provided to health plans, doctors, hospitals and other healthcare providers.

**Protected Health Information** (PHI) means individually identifiable information that is created by RespiraWorks Ventillator Software and relates to the past, present, or future physical or mental health or condition of any individual, the provision of health care to an individual, or the past, present, or future payment for the provision of health care to an individual.

**UI** is an acronym for user interface.

# 4 Software Items

## 4.1 Architecture Design Chart

Add a block diagram showing a detailed depiction of functional units and software items.  You may also want to include state diagrams as well as flow charts

## 4.2 Software Item A

## 4.3 Software Item B

# 5 SOUP Software Items

This section enumerates the SOUP software items present within RespiraWorks Ventillator Software.


## 5.1 Example Camera SDK

**Manufacturer:**

Basler AG

**Version:**

`5.3.1`

**Functional and Performance Requirements:**

To allow us to configure, control, and retrieve images from our cameras.


**Hardware & Software Requirements:**

Linux kernel version 3.x or higher


**Known Anomalies:**


No anomalies found that would result in incorrect behaviour for RespiraWorks Ventillator Software leading to a hazardous situation.

**Open Anomaly List (Reference Only):**

`https://example.url/pointing/to/issue/list`

## 5.2 FFTW

**Manufacturer:**

SOUP was developed collaboratively by the free open-source software community, and does not have a manufacturer in the traditional sense.

**Version:**

`3.3.5`

**Functional and Performance Requirements:**

To calculate the Fast Fourier Transform.


**Hardware & Software Requirements:**

No noteworthy software or hardware requirements.

**Known Anomalies:**


No anomalies found that would result in incorrect behaviour for RespiraWorks Ventillator Software leading to a hazardous situation.

**Open Anomaly List (Reference Only):**

`https://github.com/FFTW/fftw3/issues`


# 6 Functional Specifications

## 6.1 GUI System Requirements

*Requirement:* Definitions of the system requirements

## 6.2 GUI Screen Resolution

*Requirement:* GUI display shall run on a screen with resolution of 1200x600

## 6.3 Alarm Requirements

*Requirement:* Requirements describe what the software needs to do, and not how.


*Functional Specifications:*
Another specification goes here.

## 6.4 Low Pressure Alarm

*Requirement:* Software shall alert the user to high pressure when the pressure of the system goes above pressure alarm threshold (P_Alarm_Threshold).  Alarm shall generate a high level alert.


*Functional Specifications:*
Implementation details here


# 7 User Interface Mockups

If you have user interface mockups, this is a good place to put them.  One strategy is to include a sub-section for each screen, along with its own image file.  Here are some examples of various image file formats that RDM supports.

## 7.1 Screen One (PNG)

Use something like: `![Screen One](../images/uimockups/example-ui-mockup-001.png)`

Which produces:

![Screen One](../images/uimockups/example-ui-mockup-001.png)

## 7.2 Screen Two (SVG)

Use something like: `![Screen Two](../images/uimockups/example-ui-mockup-002.svg)`

Which produces:

![Screen Two](../images/uimockups/example-ui-mockup-002.svg)

## 7.3 Screen Three (JPG)

Use something like: `![Screen Three](../images/uimockups/example-ui-mockup-003.jpg)`

Which produces:

![Screen Three](../images/uimockups/example-ui-mockup-003.jpg)

## 7.4 Screen Four (PNG via URL)

Use something like: `![Screen Four](https://innolitics.com/img/home/image-acquisition.svg)`

Which produces:

![Screen Four](https://innolitics.com/img/home/image-acquisition.svg)
