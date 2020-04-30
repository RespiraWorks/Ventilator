{% extends "base/software_requirements_specification.md" %}

{% block definitions %}
# Definitions

The **Food and Drug Administration (FDA)** is a United State government agency responsible for protecting the public health by ensuring the safety, efficacy, and security of human and veterinary drugs, biological products, and medical devicese

The **Health Insurance Portability and Accountability Act** (HIPAA) is a United States law designed to provide privacy standards to protect patients' medical records and other health information provided to health plans, doctors, hospitals and other healthcare providers.

**Protected Health Information** (PHI) means individually identifiable information that is created by {{ system.project_name }} and relates to the past, present, or future physical or mental health or condition of any individual, the provision of health care to an individual, or the past, present, or future payment for the provision of health care to an individual.

**UI** is an acronym for user interface.

{%- endblock %}
{% block project_scope %}
# Project Purpose

The purpose of document is to outline the requirements of the ventilator software.

{%- endblock %}

{% block use_cases %}
# Use Cases

## Problem X

Brief description.

## Problem Y

Brief description.
{%- endblock %}
{% block ui_mockups %}
# User Interface Mockups

If you have user interface mockups, this is a good place to put them.  One strategy is to include a sub-section for each screen, along with its own SVG file.

## Screen One

Use something like: `![Screen One](../images/mockups/screen-one.svg)`

## Screen Two

Use something like: `![Screen One](../images/mockups/screen-two.svg)`
{%- endblock %}
