---
category: SRS
id: SRS-001
revision: 1
title: Software Requirements Specification
manufacturer_name: MANUFACTURER
---

# 1 Purpose

The purpose of this document is to list the requirements that describe *what* the RespiraWorks Ventillator Software v1.0.0 software must fulfill.

This document is meant to be read and agreed-upon by the project owners and by software developers during design and construction.

The document also provides traceability between system requirements and software requirements.

# 2 Scope

The scope of this SRS applies in its entirety to the RespiraWorks Ventillator Software v1.0.0 product.

# 3 Definitions

The **Food and Drug Administration (FDA)** is a United State government agency responsible for protecting the public health by ensuring the safety, efficacy, and security of human and veterinary drugs, biological products, and medical devicese

The **Health Insurance Portability and Accountability Act** (HIPAA) is a United States law designed to provide privacy standards to protect patients' medical records and other health information provided to health plans, doctors, hospitals and other healthcare providers.

**Protected Health Information** (PHI) means individually identifiable information that is created by RespiraWorks Ventillator Software and relates to the past, present, or future physical or mental health or condition of any individual, the provision of health care to an individual, or the past, present, or future payment for the provision of health care to an individual.

**UI** is an acronym for user interface.

# 4 Project Purpose

The purpose of document is to outline the requirements of the ventilator software.


# 5 Use Cases

## 5.1 Problem X

Brief description.

## 5.2 Problem Y

Brief description.

# 6 Requirement Details

## 6.1 GUI System Requirements

Definitions of the system requirements

## 6.2 GUI Screen Resolution

GUI display shall run on a screen with resolution of 1200x600

## 6.3 Alarm Requirements

Requirements describe what the software needs to do, and not how.

## 6.4 Low Pressure Alarm

Software shall alert the user to high pressure when the pressure of the system goes above pressure alarm threshold (P_Alarm_Threshold).  Alarm shall generate a high level alert.


# 7 Traceability Tables

## 7.1 Software Requirements Table

| Soft. Req. ID | System Req. IDs | Title |
| --- | --- | --- |
| r-1 | SR-X | GUI System Requirements |
| r-1-2 | SR-X | GUI Screen Resolution |
| r-2 | SR-X | Alarm Requirements |
| r-2-1 | SR-111 | Low Pressure Alarm |

## 7.2 System Requirements Mapping

| System Req. ID | Soft. Req. IDs |
| --- | --- |
| SR-111 | r-2-1 |
| SR-X | r-1, r-1-2, r-2 |
