{% extends "base/software_design_specification.md" %}

{% block definitions %}
# Definitions

The **Food and Drug Administration (FDA)** is a United State government agency responsible for protecting the public health by ensuring the safety, efficacy, and security of human and veterinary drugs, biological products, and medical devices.

The **Health Insurance Portability and Accountability Act** (HIPAA) is a United States law designed to provide privacy standards to protect patients' medical records and other health information provided to health plans, doctors, hospitals and other healthcare providers.

**Protected Health Information** (PHI) means individually identifiable information that is created by {{ system.project_name }} and relates to the past, present, or future physical or mental health or condition of any individual, the provision of health care to an individual, or the past, present, or future payment for the provision of health care to an individual.

**UI** is an acronym for user interface.

{%- endblock %}
{% block project_scope %}
# Purpose

> TODO define purpose of document

{%- endblock %}

{% block use_cases %}
# Use Cases

## Problem X

Brief description.

## Problem Y

Brief description.
{%- endblock %}

{% block architecture %}
# Software Items

## Architecture Design Chart

Add a block diagram showing a detailed depiction of functional units and software items.  You may also want to include state diagrams as well as flow charts [[:This is required by the FDAGeneralSoftwareGuidance]]

## Software Item A

## Software Item B
{%- endblock %}

{% block ui_mockups %}
# User Interface Mockups

If you have user interface mockups, this is a good place to put them.  One strategy is to include a sub-section for each screen, along with its own image file.  Here are some examples of various image file formats that RDM supports.

## Screen One (PNG)

Use something like: `![Screen One](../images/uimockups/example-ui-mockup-001.png)`

Which produces:

![Screen One](../images/uimockups/example-ui-mockup-001.png)

## Screen Two (SVG)

Use something like: `![Screen Two](../images/uimockups/example-ui-mockup-002.svg)`

Which produces:

![Screen Two](../images/uimockups/example-ui-mockup-002.svg)

## Screen Three (JPG)

Use something like: `![Screen Three](../images/uimockups/example-ui-mockup-003.jpg)`

Which produces:

![Screen Three](../images/uimockups/example-ui-mockup-003.jpg)

## Screen Four (PNG via URL)

Use something like: `![Screen Four](https://innolitics.com/img/home/image-acquisition.svg)`

Which produces:

![Screen Four](https://innolitics.com/img/home/image-acquisition.svg)

{%- endblock %}
